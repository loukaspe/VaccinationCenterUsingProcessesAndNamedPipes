/* IMPORTANT DISCLAIMER:

My classes BloomFilter and BitArray are copied from another university project, and
specifically from Operating Systems with A.Delis.
Only change made was about the size that you give in the Description of the Exercise (100KBytes).
No other change made since their use was exactly what I needed for here too. */

#ifndef UNTITLED_BLOOMFILTER_H
#define UNTITLED_BLOOMFILTER_H

#include "BitArray.h"
#include <cassert>

class BloomFilter {
public:
    explicit BloomFilter(int = 819200);
    void add(char*);
    bool check(char*);
private:
    const static int NUMBER_OF_HASH_FUNCTIONS;
    int size;
    BitArray* bitArray;
    unsigned long firstHashFunction(char*);
    unsigned long secondHashFunction(char*);
    unsigned long* getHashDigests(char*);
};

#endif //UNTITLED_BLOOMFILTER_H