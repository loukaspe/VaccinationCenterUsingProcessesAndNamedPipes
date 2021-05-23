/* Class to operate the application of Travel Monitor */

#ifndef VACCINATIONCENTERUSINGPROCESSESANDNAMEDPIPES_TRAVELMONITOR_H
#define VACCINATIONCENTERUSINGPROCESSESANDNAMEDPIPES_TRAVELMONITOR_H

static const int POLL_TIMEOUT = 180;

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <sys/wait.h>
#include <poll.h>
#include "PipeReader.h"
#include "PipeWriter.h"
#include "BloomFilterLinkedList.h"

using namespace std;

class TravelMonitor {
public:
private:
    int numberOfMonitors;
    int bufferSize;
    int bloomSizeInKiloBytes;
    char *inputDirectory;
    char** pipeNamesForTravelMonitorWrite;
    char** pipeNamesForTravelMonitorRead;
    PipeReader** pipeReaders;
    PipeWriter** pipeWriters;
    BloomFilterLinkedList* bloomFilters;

    void createPipeNamesForTravelMonitorRead();
    void createPipeNamesForTravelMonitorWrite();
    void createPipeReaders();
    void createPipeWriters();
    void passCountriesSubdirectoriesToMonitors();

    static const char* FORK_ERROR;
    static const char* MALLOC_FAIL_ERROR_MESSAGE;
public:
    TravelMonitor(int, int, int, char*);
    void createMonitorsAndPassThemData();
    void getBloomFiltersFromMonitors();
};


#endif //VACCINATIONCENTERUSINGPROCESSESANDNAMEDPIPES_TRAVELMONITOR_H
