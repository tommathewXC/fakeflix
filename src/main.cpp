#include <iostream>
#include "lib/utils.h"
#include "lib/webserver.h"

int main(int argc, char ** argv) {
    const AppInput inp = getInput(argc, argv);
    FakeFlixWebServer server = FakeFlixWebServer(8080);
    server.start();
    return 0;
}