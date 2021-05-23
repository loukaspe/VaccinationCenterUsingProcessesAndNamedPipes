#include "PipeReader.h"
#include "PipeWriter.h"
#include "Monitor.h"

using namespace std;

int main(int argc, char **argv) {
    char* pipeNameForReading = argv[1];
    char* pipeNameForWriting = argv[2];
    int fd;

//    cout << "--" << pipeNameForReading << endl;
//    cout << "--" << pipeNameForWriting << endl;

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

//    cout << "--" << inputDirectory << endl;

//    cout << "oo " << bufferSize << endl;

    int expectedCountryNames = pipeReader->readNumberWithBlock();
    char** countriesSubdirectories = (char**) malloc(
            expectedCountryNames * sizeof(char*)
    );
//    cout << "ee " << expectedCountryNames << endl;
    for(int i = 0; i < expectedCountryNames; i++) {
        int countryNameLength = pipeReader->readNumberWithBlock();
//        cout << "aa " << countryNameLength << endl;
        countriesSubdirectories[i] = pipeReader->readStringInChunksWithBlock(countryNameLength);
//        printf("read %s\n", countriesSubdirectories[i]);
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
//        cout << path << endl;

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
//            cout << path << endl;
            fileReader->readAndUpdateStructures(path);
            delete path;
        }
    }



    return 0;
}