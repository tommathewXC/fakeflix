#include "lib/webserver.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <array>
#include <string_view>
#include <signal.h>

static bool loop = true;

constexpr std::string_view RESPONSE =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Content-Length: 130\r\n"
    "\r\n"
    "<!DOCTYPE html>"
    "<html>"
    "<head><title>Hello, World!</title></head>"
    "<body><h1>Hello, World!</h1><p>This is a simple C++ web server.</p></body>"
    "</html>";


constexpr std::string_view GET_RESPONSE =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Access-Control-Allow-Origin: *\r\n"
    "Content-Length: 130\r\n"
    "\r\n"
    "<!DOCTYPE html>"
    "<html>"
    "<head><title>Hello, World!</title></head>"
    "<body><h1>Hello, World!</h1><p>This is a simple C++ web server.</p></body>"
    "</html>";


bool starts_with(const std::string& str, const std::string& prefix) {
    return str.length() >= prefix.length() &&
           str.compare(0, prefix.length(), prefix) == 0;
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
    while (loop) {
        int client_socket = accept(server_socket, nullptr, nullptr);
        std::array<char, 1024> buffer {};
        recv(client_socket, buffer.data(), buffer.size(), 0);
        std::string request(buffer.data());
        if (starts_with(request, "GET")) {
            std::cout << "Get request! " << std::endl;
            send(client_socket, GET_RESPONSE.data(), GET_RESPONSE.size(), 0);
        }
    }
    std::cout << "Stopping server..." << std::endl;
    close(server_socket);
}