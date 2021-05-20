#include "PipeReader.h"
#include "PipeWriter.h"

using namespace std;

int main(int argc, char **argv) {
    char* pipeNameForReading = argv[1];
    char* pipeNameForWriting = argv[2];
    int fd;

    cout << "--" << pipeNameForReading << endl;
    cout << "--" << pipeNameForWriting << endl;

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

    cout << "oo " << bufferSize << endl;

    int expectedCountryNames = pipeReader->readNumberWithBlock();
    cout << "ee " << expectedCountryNames << endl;
    for(int i = 0; i < expectedCountryNames; i++) {
        int countryNameLength = pipeReader->readNumberWithBlock();
        cout << "aa " << countryNameLength << endl;
        char* countryName = pipeReader->readStringInChunksWithBlock(countryNameLength);
        printf("read %s\n", countryName);
    }
    pipeReader->closePipe();

    return 0;
}