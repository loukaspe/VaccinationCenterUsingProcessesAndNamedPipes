#include "PipeWriter.h"

const int PipeWriter::FIFO_MODE = 0666;
const int PipeWriter::OPEN_MODE = O_WRONLY;
const char* PipeWriter::MKFIFO_ERROR = "ERROR: mkfifo() failed to create a new named pipe";
const char* PipeWriter::OPEN_PIPE_ERROR = "ERROR: open() failed to open a named pipe";
const char* PipeWriter::WRITING_ERROR = "ERROR: write() failed to write to a named pipe";

PipeWriter::PipeWriter(
        int fd,
        const char *filename,
        int bufferSize
) {
    this->fd = fd;
    this->filename = filename;
    this->bufferSize = bufferSize;

    if(
        ( mkfifo(filename, FIFO_MODE) == -1 )
        && errno != EEXIST
    ) {
        handlePipeError(MKFIFO_ERROR);
    }
}

PipeWriter::~PipeWriter() {

}

// I assume for simplicity that bufferSize >= sizeOf(int)
void PipeWriter::writeNumber(int number) {
    if(
        ::write(this->fd, &number, sizeof(int) ) < 0
    ) {
        handlePipeError(WRITING_ERROR);
    }
}

void PipeWriter::writeStringInChunks(char* string) {
    int totalBytes = strlen(string);

    int writtenBytes = 0;
    int chunk;

    if(totalBytes < this->bufferSize) {
        if (::write(this->fd, string, totalBytes) < 0) {
            handlePipeError(WRITING_ERROR);
        }

        return;
    }

    while(writtenBytes < totalBytes) {
        chunk = ::write(this->fd, string, this->bufferSize);
        if (chunk < 0) {
            handlePipeError(WRITING_ERROR);
        }

        // TODO: Maybe DANGERRRRRR
        // We move the string pointer chunk chars ahead to continue the writing
        // from the point is was stopped
        string += chunk;

        writtenBytes += chunk;
    }
}

/* Function to handle errors of pipes. It is different from the one in Helper, as
 * it printf also what the error is */
void PipeWriter::handlePipeError(const char* errorMessage) {
    perror(errorMessage);
    exit(1);
}

void PipeWriter::setBufferSize(int bufferSize) {
    this->bufferSize = bufferSize;
}

void PipeWriter::openPipe() {
    if(
        ( this->fd = open(this->filename, OPEN_MODE) ) < 0
    ) {
        handlePipeError(OPEN_PIPE_ERROR);
    }
}

void PipeWriter::closePipe() {
    close(this->fd);
}

void PipeWriter::writeBloomFilterInChunks(BloomFilter *bloomFilter) {
    int totalBytes = sizeof(BloomFilter);

    int writtenBytes = 0;
    int chunk;

    if(totalBytes < this->bufferSize) {
        if (::write(this->fd, bloomFilter, totalBytes) < 0) {
            handlePipeError(WRITING_ERROR);
        }

        return;
    }

    while(writtenBytes < totalBytes) {
        chunk = ::write(this->fd, bloomFilter, this->bufferSize);
        if (chunk < 0) {
            handlePipeError(WRITING_ERROR);
        }

        // TODO: Maybe DANGERRRRRR
        // We move the string pointer chunk chars ahead to continue the writing
        // from the point is was stopped
        bloomFilter += chunk;

        writtenBytes += chunk;
    }
}
