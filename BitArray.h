#ifndef UNTITLED_BITARRAY_H
#define UNTITLED_BITARRAY_H

#include <iostream>
#include <cmath>
#include "Helper.h"

class BitArray {
public:
    BitArray(int);
    ~BitArray();
    void setBit(int);
    int isSet(int);
private:
    int size;
    int* bitArray;
    const static char *WRONG_SIZE_ERROR;
    const static int IS_SET;
    const static int IS_NOT_SET;
};

#endif //UNTITLED_BITARRAY_H