#ifndef FAKEFLIX_WEBSERVER

#include <stdexcept>

class FakeFlixWebServer {

private:
short port;
size_t get_file_size(const std::string& filename);
void send_file(int client_socket, const std::string& file_path, const std::string& range);

public:
    FakeFlixWebServer(const short port);
    void start();

};

#endif // !FAKEFLIX_WEBSERVER
