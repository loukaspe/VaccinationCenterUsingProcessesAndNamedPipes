#include "PipeReader.h"

const int PipeReader::FIFO_MODE = 0666;
const int PipeReader::OPEN_MODE = O_RDONLY | O_NONBLOCK;
const char *PipeReader::MKFIFO_ERROR = "ERROR: mkfifo() failed to create a new named pipe";
const char *PipeReader::OPEN_PIPE_ERROR = "ERROR: open() failed to open a named pipe";
const char *PipeReader::READING_ERROR = "ERROR: read() failed to read from a named pipe";
char *PipeReader::MALLOC_FAIL_ERROR_MESSAGE = "ERROR: malloc() failed to "
                                              "allocate memory";

PipeReader::PipeReader(int fd, int bufferSize, const char *filename) {
    this->fd = fd;
    this->filename = filename;
    this->bufferSize = bufferSize;

    if (
            (mkfifo(filename, FIFO_MODE) == -1)
            && errno != EEXIST
            ) {
        handlePipeError(MKFIFO_ERROR);
    }

    if (
            (this->fd = open(this->filename, OPEN_MODE)) == -1
            ) {
        handlePipeError(OPEN_PIPE_ERROR);
    }
}

PipeReader::~PipeReader() {
    close(fd);
}

// I assume for simplicity that bufferSize >= sizeOf(int)
int PipeReader::readNumber() {
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

    return *number;
}

char *PipeReader::readStringInChunks(int totalBytes) {
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

    return string;
}

void PipeReader::handlePipeError(const char *errorMessage) {
    perror(errorMessage);
    exit(1);
}
