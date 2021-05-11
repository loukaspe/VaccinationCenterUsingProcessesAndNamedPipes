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

/* PseudoCode for reading in chunks
{
    rawBytes = char[total h bufSize];
    while(read < total ) {
        memcpy(rawBytes, BF)
        append
        read += n;
        total -= n;
    }
}
*/

//int PipeReader::readNumber() {
//    int *number = (int *) malloc(sizeof(int));
//    if (number == NULL) {
//        Helper::handleError(MALLOC_FAIL_ERROR_MESSAGE);
//    }
//
//    int bufferSize = sizeof(int);
//
//    if (
//            ::read(this->fd, number, bufferSize) < 0
//            ) {
//        handlePipeError(READING_ERROR);
//    }
//
//    return *number;
//}
//
//double PipeReader::readDoubleNumber() {
//    double *number = (double *) malloc(sizeof(double));
//    if (number == NULL) {
//        Helper::handleError(MALLOC_FAIL_ERROR_MESSAGE);
//    }
//
//    int bufferSize = sizeof(double);
//
//    if (
//            ::read(this->fd, number, bufferSize) < 0
//            ) {
//        handlePipeError(READING_ERROR);
//    }
//
//    return *number;
//}
//
//MyRecord *PipeReader::readRecords(long bufferSize) {
//    MyRecord *records = (MyRecord *) malloc(bufferSize);
//
//    if (
//            ::read(this->fd, records, bufferSize) < 0
//            ) {
//        handlePipeError(READING_ERROR);
//    }
//
//    return records;
//}

void PipeReader::handlePipeError(const char *errorMessage) {
    perror(errorMessage);
    exit(1);
}