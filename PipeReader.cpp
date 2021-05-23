#include "PipeReader.h"

const int PipeReader::FIFO_MODE = 0666;
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

/* Function for reading a number but with BLOCKING, in order to wait on this.
 * I assume for simplicity that bufferSize >= sizeOf(int)
 */
int PipeReader::readNumberWithBlock() {
    int *number = (int *) malloc(sizeof(int));

    if (number == NULL) {
        Helper::handleError(MALLOC_FAIL_ERROR_MESSAGE);
    }

    if (
        ::read(this->fd, number, sizeof(int)) < 0
    ) {
        handlePipeError(READING_ERROR);
    }

    return *number;
}

char *PipeReader::readStringInChunksWithBlock(int totalBytes) {
    char* string = (char*) malloc(totalBytes * sizeof(char));
    if (string == NULL) {
        Helper::handleError(MALLOC_FAIL_ERROR_MESSAGE);
    }

    if(totalBytes < this->bufferSize) {
        if (::read(this->fd, string, totalBytes) < 0) {
            handlePipeError(READING_ERROR);
        }

        return string;
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
    }

    return string;
}

void PipeReader::handlePipeError(const char *errorMessage) {
    perror(errorMessage);
    exit(1);
}

void PipeReader::setBufferSize(int bufferSize) {
    this->bufferSize = bufferSize;
}

void PipeReader::openPipe() {
    if(
        ( this->fd = open(this->filename, OPEN_MODE_WITH_BLOCK) ) < 0
    ) {
        handlePipeError(OPEN_PIPE_ERROR);
    }
}

void PipeReader::closePipe() {
    close(this->fd);
}

BloomFilter *PipeReader::readBloomFilterInChunksWithBlock() {
    int bloomFilterSize = sizeof(BloomFilter);

    BloomFilter* bloomFilter = (BloomFilter*) malloc(bloomFilterSize);
    if (bloomFilter == NULL) {
        Helper::handleError(MALLOC_FAIL_ERROR_MESSAGE);
    }

    if(bloomFilterSize < this->bufferSize) {
        if (::read(this->fd, bloomFilter, bloomFilterSize) < 0) {
            handlePipeError(READING_ERROR);
        }

        return bloomFilter;
    }

    char rawBytes[this->bufferSize];
    int readBytes = 0;
    int chunk;

    while(readBytes < bloomFilterSize) {
        chunk = ::read(this->fd, rawBytes, this->bufferSize);
        if (chunk < 0) {
            handlePipeError(READING_ERROR);
        }

        memcpy(bloomFilter + readBytes, rawBytes, chunk);
        readBytes += chunk;
    }

    return bloomFilter;
}

int PipeReader::getFd() const {
    return fd;
}
