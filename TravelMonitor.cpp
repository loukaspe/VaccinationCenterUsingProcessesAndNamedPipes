#include <sys/wait.h>
#include "TravelMonitor.h"

const char *TravelMonitor::FORK_ERROR = "ERROR: fork() failed to create a new process";

TravelMonitor::TravelMonitor(
        int numberOfMonitors,
        int bufferSize,
        int bloomSizeInKiloBytes,
        char *inputDirectory
) :
        numberOfMonitors(numberOfMonitors),
        bufferSize(bufferSize),
        bloomSizeInKiloBytes(bloomSizeInKiloBytes),
        inputDirectory(inputDirectory) {
    this->createPipeNames();

}

void TravelMonitor::createPipeNames() {
    this->pipeNames = (char **) malloc(numberOfMonitors * sizeof(char *));

    for (int i = 0; i < numberOfMonitors; i++) {

        // Hardcoded 30 means that we can have up to 9999 pipe names without crashing.
        // Did not have time to make it dynamic depending on number Of Monitors
        this->pipeNames[i] = (char *) malloc(30 * sizeof(char));
        sprintf(this->pipeNames[i], "pipeTravelMonitorToMonitor%d", i);

        //TODO: remove, debugging reasons
        printf("%s\n", this->pipeNames[i]);
    }
}

/* Function that creates a 2D string array that holds the arguments' array for
 * each one of the numberOfMonitor Monitors */
void TravelMonitor::createMonitorArguments() {
    this->monitorArguments = (char ***) malloc((numberOfMonitors) * sizeof(char **));
    // Each monitor argv array will have 3 strings: ./monitor, thePipeName, NULL
    for (int i = 0; i < numberOfMonitors; i++) {
        this->monitorArguments[i] = (char **) malloc(3 * sizeof(char *));
        this->monitorArguments[i][0] = "./monitor";
        this->monitorArguments[i][1] = this->pipeNames[i];
        this->monitorArguments[i][0] = NULL;
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
            execvp(
                    "./monitor",
                    this->monitorArguments[i]
            );

            exit(0);
        }

        wait(NULL);
    }
}
