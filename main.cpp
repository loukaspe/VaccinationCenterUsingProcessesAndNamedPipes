#include "TravelMonitor.h"
#include <getopt.h>

const char *PROGRAM_OPTIONS = "m:b:s:i:";
const char *WRONG_PROGRAM_USAGE_ERROR = "Usage %s -m [numMonitors] -b [bufferSize] "
                                        "-s [sizeOfBloom] -i [input_dir]\n";

int main(int argc, char *argv[]) {
    int opt;

    int numberOfMonitors;
    int bufferSize;
    int bloomSizeInKiloBytes;
    char *inputDirectory;

    while ((opt = getopt(argc, argv, PROGRAM_OPTIONS)) != -1) {
        switch (opt) {
            case 'm':
                numberOfMonitors = atoi(optarg);
                break;
            case 'b':
                bufferSize = atoi(optarg);
                break;
            case 's':
                bloomSizeInKiloBytes = atoi(optarg);
                break;
            case 'i':
                inputDirectory = Helper::copyString(optarg);
                break;
            default:
                Helper::handleError(WRONG_PROGRAM_USAGE_ERROR);
        }
    }

    TravelMonitor* travelMonitor = new TravelMonitor(
            numberOfMonitors,
            bufferSize,
            bloomSizeInKiloBytes,
            inputDirectory
    );

    travelMonitor->createMonitorsAndPassThemData();
    travelMonitor->getBloomFiltersFromMonitors();

//    MenuPromptCreator *menuPromptCreator = new MenuPromptCreator(
//            vaccinationCenter
//    );
//
//    menuPromptCreator->createAndExecute();

    return 0;
}
