#include "PipeReader.h"
#include "PipeWriter.h"
#include "Monitor.h"

using namespace std;

int main(int argc, char **argv) {
    char* pipeNameForReading = argv[1];
    char* pipeNameForWriting = argv[2];
    int fd;

    PipeWriter* pipeWriter = new PipeWriter(
        fd,
        pipeNameForWriting
    );

    PipeReader* pipeReader = new PipeReader(
        fd,
        pipeNameForReading
    );

    pipeReader->openPipe();
    int bufferSize = pipeReader->readNumberWithBlock();
    pipeReader->setBufferSize(bufferSize);
    pipeWriter->setBufferSize(bufferSize);

    int inputDirectorySize = pipeReader->readNumberWithBlock();
    char* inputDirectory = pipeReader->readStringInChunksWithBlock(inputDirectorySize);

    int expectedCountryNames = pipeReader->readNumberWithBlock();
    char** countriesSubdirectories = (char**) malloc(
            expectedCountryNames * sizeof(char*)
    );

    for(int i = 0; i < expectedCountryNames; i++) {
        int countryNameLength = pipeReader->readNumberWithBlock();
        countriesSubdirectories[i] = pipeReader->readStringInChunksWithBlock(countryNameLength);
    }
    pipeReader->closePipe();

    PersonLinkedList *people = new PersonLinkedList();
    VirusLinkedList *viruses = new VirusLinkedList();
    CountryLinkedList *countries = new CountryLinkedList();

    VaccinationCenter *vaccinationCenter = new VaccinationCenter(
            people,
            viruses,
            countries
    );

    CitizenRecordsFileReader *fileReader = new CitizenRecordsFileReader(
            vaccinationCenter
    );

    Monitor* monitor = new Monitor(
        people,
        viruses,
        countries,
        vaccinationCenter,
        fileReader
    );

    char* directoryCharacter = "/";
    int directoryCharacterSize = strlen(directoryCharacter);

    for(int i = 0; i < expectedCountryNames; i++) {
        // We build the path for reading the files with the records
        int countryNameSize = strlen(countriesSubdirectories[i]);
        char *path = (char*) malloc(
            inputDirectorySize + countryNameSize + directoryCharacterSize + 1
        );
        strcpy(path, inputDirectory);
        strcat(path, directoryCharacter);
        strcat(path, countriesSubdirectories[i]);

        int numberOfFiles = Helper::getAllFilesNumber(path);
        char** countriesFile = Helper::getAllFilesNames(path);

        delete path;
        for(int j = 0; j < numberOfFiles; j++) {
            // We build the path for each records' file
            int inputFileSize = strlen(countriesFile[j]);
            path = (char*) malloc(
                    inputDirectorySize
                    + countryNameSize
                    + 2 * directoryCharacterSize
                    + inputFileSize
                    + 1
            );
            strcpy(path, inputDirectory);
            strcat(path, directoryCharacter);
            strcat(path, countriesSubdirectories[i]);
            strcat(path, directoryCharacter);
            strcat(path, countriesFile[j]);

            fileReader->readAndUpdateStructures(path);
            delete path;
        }
    }

    // Every Virus in the Monitor has one BloomFilter. So the Monitor will send
    // to the TravelMonitor numberOfViruses BloomFilters.
    int numberOfViruses = viruses->getSize();
    int numberOfBloomFiltersSent = numberOfViruses;

    pipeWriter->openPipe();

    // Send through pipes the number of expected bloom filters
    pipeWriter->writeNumber(numberOfBloomFiltersSent);

    VirusLinkedListNode *current = viruses->getHead();
    while(current != NULL) {
        // Send through pipes the bloom filters of the Monitor
        BloomFilter* temp = current->getVirus()->getVaccinatedPeopleBloomFilter();
        pipeWriter->writeBloomFilterInChunks(temp);
        current = current->next;
    }

    pipeWriter->closePipe();

    return 0;
}