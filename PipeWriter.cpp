#include "PipeWriter.h"

const int PipeWriter::FIFO_MODE = 0666;
const int PipeWriter::OPEN_MODE = O_WRONLY | O_NONBLOCK;
const char* PipeWriter::MKFIFO_ERROR = "ERROR: mkfifo() failed to create a new named pipe";
const char* PipeWriter::OPEN_PIPE_ERROR = "ERROR: open() failed to open a named pipe";
const char* PipeWriter::WRITING_ERROR = "ERROR: write() failed to write to a named pipe";

PipeWriter::PipeWriter(int fd, int bufferSize, const char* filename) {
    this->fd = fd;
    this->filename = filename;
    this->bufferSize = bufferSize;

    if(
        ( mkfifo(filename, FIFO_MODE) == -1 )
        && errno != EEXIST
    ) {
        handlePipeError(MKFIFO_ERROR);
    }

    if(
        ( this->fd = open(this->filename, OPEN_MODE) ) < 0
    ) {
        handlePipeError(OPEN_PIPE_ERROR);
    }
}

PipeWriter::~PipeWriter() {
    close(fd);
}

// I assume for simplicity that bufferSize >= sizeOf(int)
void PipeWriter::writeNumber(int number) {
    int bufferSize = sizeof(int);

    if(
        ::write( this->fd, &number, bufferSize ) < 0
    ) {
        handlePipeError(WRITING_ERROR);
    }
}

void PipeWriter::writeStringInChunks(char* string) {
    int totalBytes = sizeof(string);

    int writtenBytes = 0;
    int chunk;

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
        totalBytes -= chunk;
    }
}

/* Function to handle errors of pipes. It is different from the one in Helper, as
 * it printf also what the error is */
void PipeWriter::handlePipeError(const char* errorMessage) {
    perror(errorMessage);
    exit(1);
}