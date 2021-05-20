#include "PipeReader.h"

const int PipeReader::FIFO_MODE = 0666;
const int PipeReader::OPEN_MODE = O_RDONLY | O_NONBLOCK;
const int PipeReader::OPEN_MODE_WITH_BLOCK = O_RDONLY;
const char *PipeReader::MKFIFO_ERROR = "ERROR: mkfifo() failed to create a new named pipe";
const char *PipeReader::OPEN_PIPE_ERROR = "ERROR: open() failed to open a named pipe";
const char *PipeReader::READING_ERROR = "ERROR: read() failed to read from a named pipe";
char *PipeReader::MALLOC_FAIL_ERROR_MESSAGE = "ERROR: malloc() failed to "
                                              "allocate memory";

PipeReader::PipeReader(
        int fd,
        const char *filename,
        int bufferSize
) {
    this->fd = fd;
    this->filename = filename;
    this->bufferSize = bufferSize;

    if (
        (mkfifo(filename, FIFO_MODE) == -1)
        && errno != EEXIST
    ) {
        handlePipeError(MKFIFO_ERROR);
    }
}

PipeReader::~PipeReader() {

}

// I assume for simplicity that bufferSize >= sizeOf(int)
int PipeReader::readNumber() {
    if(
        ( this->fd = open(this->filename, OPEN_MODE) ) == -1
    ) {
        handlePipeError(OPEN_PIPE_ERROR);
    }

    int *number = (int *) malloc(sizeof(int));

    if (number == NULL) {
        Helper::handleError(MALLOC_FAIL_ERROR_MESSAGE);
    }

    int bufferSize = sizeof(int);

    if (
        ::read(this->fd, number, bufferSize) < 0
    ) {
        handlePipeError(READING_ERROR);
    }

    close(fd);
    return *number;
}

/* Function for reading a number but with BLOCKING, in order to wait on this.
 * I assume for simplicity that bufferSize >= sizeOf(int)
 */
int PipeReader::readNumberWithBlock() {
    if(
        ( this->fd = open(this->filename, OPEN_MODE_WITH_BLOCK) ) == -1
    ) {
        handlePipeError(OPEN_PIPE_ERROR);
    }

    int *number = (int *) malloc(sizeof(int));

    if (number == NULL) {
        Helper::handleError(MALLOC_FAIL_ERROR_MESSAGE);
    }

    int bufferSize = sizeof(int);

    if (
        ::read(this->fd, number, bufferSize) < 0
    ) {
        handlePipeError(READING_ERROR);
    }

    close(fd);
    return *number;
}

char *PipeReader::readStringInChunks(int totalBytes) {
    if(
        ( this->fd = open(this->filename, OPEN_MODE) ) == -1
    ) {
        handlePipeError(OPEN_PIPE_ERROR);
    }

    char* string = (char*) malloc(totalBytes * sizeof(char));
    if (string == NULL) {
        Helper::handleError(MALLOC_FAIL_ERROR_MESSAGE);
    }

    char rawBytes[this->bufferSize];
    int readBytes = 0;
    int chunk;

    while(readBytes < totalBytes) {
        chunk = ::read(this->fd, rawBytes, this->bufferSize);
        if (chunk < 0) {
            handlePipeError(READING_ERROR);
        }

        strncat(string, rawBytes, chunk);
        readBytes += chunk;
        // TODO: is this needed ?? totalBytes -= chunk;
    }

    close(fd);
    return string;
}

char *PipeReader::readStringInChunksWithBlock(int totalBytes) {
    if(
        ( this->fd = open(this->filename, OPEN_MODE_WITH_BLOCK) ) == -1
    ) {
        handlePipeError(OPEN_PIPE_ERROR);
    }

    char* string = (char*) malloc(totalBytes * sizeof(char));
    if (string == NULL) {
        Helper::handleError(MALLOC_FAIL_ERROR_MESSAGE);
    }

    char rawBytes[this->bufferSize];
    int readBytes = 0;
    int chunk;

    while(readBytes < totalBytes) {
        chunk = ::read(this->fd, rawBytes, this->bufferSize);
        if (chunk < 0) {
            handlePipeError(READING_ERROR);
        }

        strncat(string, rawBytes, chunk);
        readBytes += chunk;
        totalBytes -= chunk;
    }

    close(fd);
    return string;
}

void PipeReader::handlePipeError(const char *errorMessage) {
    perror(errorMessage);
    exit(1);
}

void PipeReader::setBufferSize(int bufferSize) {
    this->bufferSize = bufferSize;
}
