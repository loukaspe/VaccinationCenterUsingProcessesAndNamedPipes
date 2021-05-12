#include "PipeReader.h"
#include "PipeWriter.h"

using namespace std;

char **getPipeNamesFromCommandLineArguments(int, char **);

int main(int argc, char **argv) {
    char **pipeNames = getPipeNamesFromCommandLineArguments(argc, argv);
}

char **getPipeNamesFromCommandLineArguments(int argc, char **argv) {
    // number of pipes passed from TravelMonitor equals to the argc - 1 (the
    // executable file name)
    int numberOfPipeNames = argc - 1;
    char **pipeNames = (char **) malloc(numberOfPipeNames * sizeof(char *));

    for (int i = 1; i < argc; i++) {
        pipeNames[i - 1] = argv[i];
        printf("%s\n", pipeNames[i-1]);
    }

    return pipeNames;
}