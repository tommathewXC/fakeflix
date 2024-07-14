#include "lib/webserver.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <array>
#include <string_view>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>
#include <stdexcept>

const std::string VIDEO_PATH = "content/sample8k.mp4";

size_t FakeFlixWebServer::get_file_size(const std::string& filename) {
    struct stat st;
    if (stat(filename.c_str(), &st) != 0) {
        return 0;
    }
    return st.st_size;
}

void FakeFlixWebServer::send_file(int client_socket, const std::string& file_path, const std::string& range) {
    std::cout << "Sending file..." << std::endl;
    std::ifstream file(file_path, std::ios::binary);
    if (!file) {
        std::string response = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
        send(client_socket, response.c_str(), response.length(), 0);
        return;
    }

    size_t file_size = this->get_file_size(file_path);
    size_t start_pos = 0;
    size_t end_pos = file_size - 1;

    // Parse range header
    if (!range.empty()) {
        sscanf(range.c_str(), "bytes=%zu-%zu", &start_pos, &end_pos);
        if (end_pos == 0 || end_pos >= file_size) end_pos = file_size - 1;
    }

    size_t content_length = end_pos - start_pos + 1;

    std::string response = "HTTP/1.1 206 Partial Content\r\n";
    response += "Content-Type: video/mp4\r\n";
    response += "Accept-Ranges: bytes\r\n";
    response += "Content-Length: " + std::to_string(content_length) + "\r\n";
    response += "Content-Range: bytes " + std::to_string(start_pos) + "-" + std::to_string(end_pos) + "/" + std::to_string(file_size) + "\r\n";
    response += "Access-Control-Allow-Origin: *\r\n";  // CORS header
    response += "\r\n";

    send(client_socket, response.c_str(), response.length(), 0);

    file.seekg(start_pos);
    std::vector<char> buffer(1024 * 1024 );  // 1MB buffer
    size_t remaining = content_length;

    std::cout << "Starting stream" << std::endl;
    while (remaining > 0 && file) {
        size_t to_read = std::min(remaining, buffer.size());
        file.read(buffer.data(), to_read);
        size_t bytes_read = file.gcount();
        std::cout << "Attempting to send " << bytes_read << " bytes. Remaining: " << remaining << std::endl;
        
        ssize_t sent = send(client_socket, buffer.data(), bytes_read, 0);
        if (sent < 0) {
            std::cerr << "Send error: " << strerror(errno) << std::endl;
            // Handle specific errors
            if (errno == EPIPE || errno == ECONNRESET) {
                std::cerr << "Client disconnected." << std::endl;
                break;
            } else {
                // For other errors, you might want to retry or break depending on the error
                std::cerr << "Unrecoverable error. Stopping transmission." << std::endl;
                break;
            }
        } else if (sent == 0) {
            std::cerr << "Send returned 0. Client might have closed the connection." << std::endl;
            break;
        } else if (static_cast<size_t>(sent) < bytes_read) {
            std::cout << "Partial send. Sent " << sent << " out of " << bytes_read << " bytes." << std::endl;
            file.seekg(-(bytes_read - sent), std::ios_base::cur);
            remaining -= sent;
        } else {
            remaining -= bytes_read;
        }
        std::cout << "Successfully sent " << sent << " bytes. Remaining: " << remaining << std::endl;
    }
}


FakeFlixWebServer::FakeFlixWebServer(const short port) {
    this->port = port;
}

void FakeFlixWebServer::start() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(this->port);
    bind(server_socket, (struct sockaddr *) &address, sizeof(address));
    listen(server_socket, SOMAXCONN);
    std::cout << "Starting server on port " << this->port << std::endl;
    while (true) {
        int client_socket = accept(server_socket, nullptr, nullptr);
        std::array<char, 1024> buffer {};
        recv(client_socket, buffer.data(), buffer.size(), 0);
        std::string request(buffer.data());
        if (request.find("GET /video") != std::string::npos) {
            std::cout << "Got video request" << std::endl;
            size_t range_pos = request.find("Range: ");
            std::string range;
            if (range_pos != std::string::npos) {
                size_t range_end = request.find("\r\n", range_pos);
                range = request.substr(range_pos + 7, range_end - range_pos - 7);
            }
            send_file(client_socket, VIDEO_PATH, range);
        }
    }
    std::cout << "Stopping server..." << std::endl;
    close(server_socket);
}