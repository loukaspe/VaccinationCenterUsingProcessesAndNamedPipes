#ifndef VACCINATIONCENTERUSINGPROCESSESANDNAMEDPIPES_BLOOMFILTERLINKEDLISTNODE_H
#define VACCINATIONCENTERUSINGPROCESSESANDNAMEDPIPES_BLOOMFILTERLINKEDLISTNODE_H

#include "BloomFilter.h"

class BloomFilterLinkedListNode {
public:
    BloomFilterLinkedListNode *next;
    BloomFilterLinkedListNode(BloomFilter*);
    BloomFilter *getBloomFilter() const;
private:
    BloomFilter* bloomFilter;
};


#endif //VACCINATIONCENTERUSINGPROCESSESANDNAMEDPIPES_BLOOMFILTERLINKEDLISTNODE_H
