#include <iostream>
#include "lib/utils.h"

int main(int argc, char ** argv) {
    const AppInput inp = getInput(argc, argv);
    std::cout << inp.logFileLocation << std::endl;
    std::cout << "Content in: " << inp.contentFolder << std::endl;
    return 0;
}