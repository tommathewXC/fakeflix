#ifndef FAKEFLIX_WEBSERVER

class FakeFlixWebServer {

private:
short port;

public:
    FakeFlixWebServer(const short port);
    void start();
};

#endif // !FAKEFLIX_WEBSERVER
