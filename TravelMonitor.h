/* Class to operate the application of Travel Monitor */

#ifndef VACCINATIONCENTERUSINGPROCESSESANDNAMEDPIPES_TRAVELMONITOR_H
#define VACCINATIONCENTERUSINGPROCESSESANDNAMEDPIPES_TRAVELMONITOR_H

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include "PipeReader.h"
#include "PipeWriter.h"

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
};


#endif //VACCINATIONCENTERUSINGPROCESSESANDNAMEDPIPES_TRAVELMONITOR_H
