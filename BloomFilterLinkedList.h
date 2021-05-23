/* A simple Linked List to hold the BloomFilter in the TravelMonitor. There is also
 * a findByVirusAndCountry() to be used to satisfy the User's request. Don't care
 * about List's complexity since the BloomFilters' number is limited */

#ifndef VACCINATIONCENTERUSINGPROCESSESANDNAMEDPIPES_BLOOMFILTERLINKEDLIST_H
#define VACCINATIONCENTERUSINGPROCESSESANDNAMEDPIPES_BLOOMFILTERLINKEDLIST_H

#include "./BloomFilterLinkedListNode.h"

class BloomFilterLinkedList {
public:
    BloomFilterLinkedList();

    void addAtStart(BloomFilter *);

    BloomFilterLinkedListNode *findByVirusAndCountry(char *, char *);

private:
    BloomFilterLinkedListNode *head;
    int size;
};


#endif //VACCINATIONCENTERUSINGPROCESSESANDNAMEDPIPES_BLOOMFILTERLINKEDLIST_H
