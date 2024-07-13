#include <string.h>
#include <iostream>
#include "lib/utils.h"

AppInput getInput(const int arg_count, char ** arguments) {
    std::string location;
    std::string contentFolder;
    bool foundLogFile = false;
    bool foundContentFolder = false;
    if (arg_count > 0) {
        for (int i =0 ; i < arg_count; i ++ ){
            if (foundLogFile) {
                location = std::string(arguments[i]);
            }
            if (foundContentFolder) {
                contentFolder = std::string(arguments[i]);
            }
            if (strcmp(arguments[i], "-logfile") == 0) {
                foundLogFile = true;
            }
            if (strcmp(arguments[i], "-contentFolder") == 0) {
                foundContentFolder = true;
            }
        }
    }
    AppInput output {
        location,
        contentFolder
    };
    return output;
}