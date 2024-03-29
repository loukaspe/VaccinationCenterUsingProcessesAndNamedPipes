/* This class will contain the functions to be used for reading from Named Pipes in
 * the Project.
 * Source for Named Pipes and implementing them in my processes your given:
 * http://cgi.di.uoa.gr/~ad/k22/Rec2-ProcsIPC.pdf
 * and http://cgi.di.uoa.gr/~ad/k22/named-pipes.pdf */

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "Helper.h"
#include "BloomFilter.h"

#ifndef VACCINATIONCENTERUSINGPROCESSESANDNAMEDPIPES_PIPEREADER_H
#define VACCINATIONCENTERUSINGPROCESSESANDNAMEDPIPES_PIPEREADER_H

class PipeReader {
public:
    PipeReader(int, const char*, int = sizeof(int));
    ~PipeReader();
    char* readStringInChunksWithBlock(int);
    BloomFilter* readBloomFilterInChunksWithBlock();
    int readNumberWithBlock();
    void setBufferSize(int);
    void openPipe();
    void closePipe();
    int getFd() const;
private:
    int fd;

private:
    int bufferSize;
    const char* filename;
    static const int FIFO_MODE;
    static const int OPEN_MODE_WITH_BLOCK;
    static const char* MKFIFO_ERROR;
    static const char* OPEN_PIPE_ERROR;
    static const char* READING_ERROR;
    void handlePipeError(const char*);
    static char* MALLOC_FAIL_ERROR_MESSAGE;
};

#endif //VACCINATIONCENTERUSINGPROCESSESANDNAMEDPIPES_PIPEREADER_H
