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
        inputDirectory(inputDirectory) {
    this->createPipeNamesForTravelMonitorWrite();
    this->createPipeNamesForTravelMonitorRead();
    this->createPipeReaders();
    this->createPipeWriters();
    this->bloomFilters = new BloomFilterLinkedList();
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
                this->pipeNamesForTravelMonitorRead[i],
                this->bufferSize
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
                this->pipeNamesForTravelMonitorWrite[i],
                this->bufferSize
        );
    }
}

void TravelMonitor::createMonitorsAndPassThemData() {
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

        this->pipeWriters[i]->openPipe();
        this->pipeWriters[i]->writeNumber(this->bufferSize);

        int inputDirectorySize = strlen(this->inputDirectory);
        this->pipeWriters[i]->writeNumber(inputDirectorySize);
        this->pipeWriters[i]->writeStringInChunks(inputDirectory);
    }

    this->passCountriesSubdirectoriesToMonitors();
}

/*
 * This function takes all countriesSubdirectories' names and then counts
 * how many names exist for each monitor and then sends through the pipes:
 * 1. the number of expected names, 2. foreach name: it's number of bytes and the
 * actual string
 */
void TravelMonitor::passCountriesSubdirectoriesToMonitors() {
    char **countrySubdirectories = Helper::getAllSubdirectoriesNames(this->inputDirectory);
    int numberOfCountrySubdirectories = Helper::getAllSubdirectoriesNumber(this->inputDirectory);
    int remainingNumberOfCountrySubdirectories = numberOfCountrySubdirectories;
    int divisionRemainder;
    int subdirectoryNameLength;
    int numberOfCountriesPassedToMonitor;
    int numberOfMonitorWithSentCountries = numberOfMonitors;

    for (int i = 0; i < numberOfMonitors; i++) {
        numberOfCountriesPassedToMonitor = Helper::getCeilingOfDividedInts(
                remainingNumberOfCountrySubdirectories,
                numberOfMonitorWithSentCountries
        );

        this->pipeWriters[i]->writeNumber(numberOfCountriesPassedToMonitor);

        for (int j = numberOfCountrySubdirectories - 1; j >= 0; j--) {
            divisionRemainder = j % numberOfMonitors;

            if (divisionRemainder == i) {
                subdirectoryNameLength = strlen(countrySubdirectories[j]);
                this->pipeWriters[i]->writeNumber(subdirectoryNameLength);

                this->pipeWriters[i]->writeStringInChunks(countrySubdirectories[j]);
            }
        }

        remainingNumberOfCountrySubdirectories -= numberOfCountriesPassedToMonitor;
        numberOfMonitorWithSentCountries--;
        this->pipeWriters[i]->closePipe();
    }
}

void TravelMonitor::getBloomFiltersFromMonitors() {
    // Initialize fdArray for named pipe reading with poll()
    struct pollfd fdArray[numberOfMonitors];
    int numberOfFds = numberOfMonitors;
    int rc;
    int numberOfTotalExpectedBloomFiltersFromAllMonitors = 0;
    int numberOfBloomFiltersPerMonitor[numberOfMonitors];

    // We open each pipe
    for (int i = 0; i < numberOfMonitors; i++) {
        this->pipeReaders[i]->openPipe();
    }

    // Read expected bloomFilterPerMonitor and count total bloomFilters
    for (int i = 0; i < numberOfMonitors; i++) {
        numberOfBloomFiltersPerMonitor[i] = this->pipeReaders[i]->readNumberWithBlock();
        numberOfTotalExpectedBloomFiltersFromAllMonitors += numberOfBloomFiltersPerMonitor[i];
    }

    for (int i = 0; i < numberOfFds; i++) {
        fdArray[i].fd = pipeReaders[i]->getFd();
        fdArray[i].events = POLLIN;
    }

    while (this->bloomFilters->getSize() != numberOfTotalExpectedBloomFiltersFromAllMonitors) {
        rc = poll(fdArray, numberOfFds, POLL_TIMEOUT);

        if (rc == 0) {
            Helper::handleError("ERROR: poll() timeout.");
        }
        else if (rc < 1) {
            Helper::handleError("ERROR: Unknown poll() error.");
        }
        else if (rc == 1) {
            for (int i = 0; i < numberOfFds; i++) {
                if (
                    fdArray[i].fd == this->pipeReaders[i]->getFd()
                    && fdArray[i].revents == POLLIN
                ) {
                    for (int j = 0; j < numberOfBloomFiltersPerMonitor[i]; j++) {
                        bloomFilters->addAtStart(
                            this->pipeReaders[i]->readBloomFilterInChunksWithBlock()
                        );
                    }
                }
            }
        }
    }

    for (int i = 0; i < numberOfMonitors; i++) {
        pipeReaders[i]->closePipe();
    }

    // Wait for all child processes to end
    while (wait(NULL) > 0);
}