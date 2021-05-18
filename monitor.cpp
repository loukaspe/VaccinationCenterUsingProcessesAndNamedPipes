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

    int bufferSize = pipeReader->readNumberWithBlock();
    pipeReader->setBufferSize(bufferSize);
    pipeWriter->setBufferSize(bufferSize);
    cout << "oo " << bufferSize << endl;

    return 0;
}