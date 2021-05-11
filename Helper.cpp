#include "Helper.h"

const char* Helper::STRING_COPY_ERROR = "ERROR: Something went wrong with Helper Copy\n";

void Helper::handleError(const char * errorMessage) {
    fprintf(
            stderr,
            "%s",
            errorMessage
    );
    exit(EXIT_FAILURE);
}


char* Helper::copyString(char* source) {
    char* destination;
    int stringLength = strlen(source);

    destination = (char*) malloc(
            stringLength * sizeof(char)
    );

    if(destination == NULL) {
        fprintf(stderr, "%s",STRING_COPY_ERROR);
        exit(EXIT_FAILURE);
    }

    strcpy(destination, source);
    return destination;
}

bool Helper::inArray(char* needle, char** haystack, int haystackSize) {
    int i;
    for( i = 0; i < haystackSize; i++) {
        if( strcmp( needle, haystack[i] ) == 0 ) {
            return true;
        }
    }

    return false;
}

bool Helper::hasStringNewLineCharacterInTheEnd(char* string) {
    int length = strlen(string);
    return string[length - 1] == '\n';
}

char* Helper::removeNewLineCharacterFromString(char* string) {
    if( hasStringNewLineCharacterInTheEnd(string) ) {
        int length = strlen(string);
        char* stringWithoutNewLine = (char*) malloc(
                length * sizeof(char)
        );

        int i;
        for( i = 0; i < length; i++) {
            if(string[i] == '\n') {
                stringWithoutNewLine[i] = '\0';
                continue;
            }

            stringWithoutNewLine[i] = string[i];
        }

        return stringWithoutNewLine;
    }

    return string;
}