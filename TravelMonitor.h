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
    char** pipeNames;
    char*** monitorArguments;

    void createMonitorArguments();
    void createPipeNames();

    static const char* FORK_ERROR;
public:
    TravelMonitor(int, int, int, char*);
    void createMonitors();
};


#endif //VACCINATIONCENTERUSINGPROCESSESANDNAMEDPIPES_TRAVELMONITOR_H
