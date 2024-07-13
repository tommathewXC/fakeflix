#ifndef UTIL_FUNCTIONS_FOR_FAKEFLIX
#include <string>

struct AppInput
{
    std::string logFileLocation;
    std::string contentFolder;
};

AppInput getInput(const int arg_count, char ** arguments);

#endif // !UTIL_FUNCTIONS_FOR_FAKEFLIX

