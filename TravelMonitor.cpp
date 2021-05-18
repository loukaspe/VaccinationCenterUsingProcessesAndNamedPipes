#include <sys/wait.h>
#include "TravelMonitor.h"

const char *TravelMonitor::FORK_ERROR = "ERROR: fork() failed to create a new process";
const char *TravelMonitor::MALLOC_FAIL_ERROR_MESSAGE = "ERROR: malloc() failed to allocate memory";

TravelMonitor::TravelMonitor(
        int numberOfMonitors,
        int bufferSize,
        int bloomSizeInKiloBytes,
        char *inputDirectory
) :
        numberOfMonitors(numberOfMonitors),
        bufferSize(bufferSize),
        bloomSizeInKiloBytes(bloomSizeInKiloBytes),
        inputDirectory(inputDirectory)
{
    this->createPipeNamesForTravelMonitorWrite();
    this->createPipeNamesForTravelMonitorRead();
    this->createPipeReaders();
    this->createPipeWriters();
}

void TravelMonitor::createPipeNamesForTravelMonitorWrite() {
    this->pipeNamesForTravelMonitorWrite = (char **) malloc(numberOfMonitors * sizeof(char *));

    for (int i = 0; i < numberOfMonitors; i++) {

        // Hardcoded 40 means that we can have up to 9999 pipe names without crashing.
        // Did not have time to make it dynamic depending on number Of Monitors
        this->pipeNamesForTravelMonitorWrite[i] = (char *) malloc(40 * sizeof(char));
        sprintf(
            this->pipeNamesForTravelMonitorWrite[i],
            "pipeTravelMonitorWriteToMonitorRead%d",
            i
        );
    }
}

void TravelMonitor::createPipeNamesForTravelMonitorRead() {
    this->pipeNamesForTravelMonitorRead = (char **) malloc(numberOfMonitors * sizeof(char *));

    for (int i = 0; i < numberOfMonitors; i++) {

        // Hardcoded 40 means that we can have up to 9999 pipe names without crashing.
        // Did not have time to make it dynamic depending on number Of Monitors
        this->pipeNamesForTravelMonitorRead[i] = (char *) malloc(30 * sizeof(char));
        sprintf(
            this->pipeNamesForTravelMonitorRead[i],
            "pipeTravelMonitorReadToMonitorWrite%d",
            i
        );
    }
}

void TravelMonitor::createMonitors() {
    for (int i = 0; i < this->numberOfMonitors; i++) {
        pid_t id = fork();

        if (id == -1) {
            perror(FORK_ERROR);
            exit(1);
        }

        if (id == 0) {
            execlp(
                "./monitor",
                "./monitor",
                this->pipeNamesForTravelMonitorWrite[i],
                this->pipeNamesForTravelMonitorRead[i],
                NULL
            );

            exit(0);
        }

        this->pipeWriters[i]->writeNumber(this->bufferSize);
        wait(NULL);
    }
}

void TravelMonitor::createPipeReaders() {
    /* We want a pipe reader for each monitor to be connected with the travelMonitor.
    * So we create an array of 'pipes' */
    int fd[this->numberOfMonitors];

    this->pipeReaders = (PipeReader **) malloc(
        this->numberOfMonitors * sizeof(PipeReader)
    );
    if (pipeReaders == NULL) {
        Helper::handleError(MALLOC_FAIL_ERROR_MESSAGE);
    }

    for (int i = 0; i < this->numberOfMonitors; i++) {
        pipeReaders[i] = new PipeReader(
            fd[i],
            this->pipeNamesForTravelMonitorRead[i]
        );
    }
}

void TravelMonitor::createPipeWriters() {
    /* We want a pipe reader for each monitor to be connected with the travelMonitor.
    * So we create an array of 'pipes' */
    int fd[this->numberOfMonitors];

    this->pipeWriters = (PipeWriter **) malloc(
        this->numberOfMonitors * sizeof(PipeWriter)
    );
    if (pipeWriters == NULL) {
        Helper::handleError(MALLOC_FAIL_ERROR_MESSAGE);
    }

    for (int i = 0; i < this->numberOfMonitors; i++) {
        pipeWriters[i] = new PipeWriter(
            fd[i],
            this->pipeNamesForTravelMonitorWrite[i]
        );
    }
}
