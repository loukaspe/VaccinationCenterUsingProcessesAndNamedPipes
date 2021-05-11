#ifndef UNTITLED_HELPER_H
#define UNTITLED_HELPER_H

/* Helper Class contains various function that should not technically be
 * part of other classes and have more general usage */

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cmath>

using namespace std;

class Helper {
public:
    static char* copyString(char*);
    static void handleError(const char*);
    static bool inArray(char*, char**, int);
    static char* removeNewLineCharacterFromString(char*);
private:
    static const char* STRING_COPY_ERROR;
    static bool hasStringNewLineCharacterInTheEnd(char*);
};

#endif //UNTITLED_HELPER_H