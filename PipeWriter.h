/* This class will contain the functions to be used for writing in Named Pipes in
 * the Project.
 * Source for Named Pipes and implementing them in my processes your given:
 * http://cgi.di.uoa.gr/~ad/k22/Rec2-ProcsIPC.pdf
 * and http://cgi.di.uoa.gr/~ad/k22/named-pipes.pdf */

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "Helper.h"

#ifndef VACCINATIONCENTERUSINGPROCESSESANDNAMEDPIPES_PIPEWRITER_H
#define VACCINATIONCENTERUSINGPROCESSESANDNAMEDPIPES_PIPEWRITER_H

class PipeWriter {
public:
    PipeWriter(int, const char*, int = sizeof(int));
    ~PipeWriter();
    void writeNumber(int);
    void writeStringInChunks(char*);
    void setBufferSize(int);
    void openPipe();
    void closePipe();
private:
    int fd;
    int bufferSize;
    const char* filename;
    static const int FIFO_MODE;
    static const int OPEN_MODE;
    static const char* MKFIFO_ERROR;
    static const char* OPEN_PIPE_ERROR;
    static const char* WRITING_ERROR;
    void handlePipeError(const char*);
};
#endif //VACCINATIONCENTERUSINGPROCESSESANDNAMEDPIPES_PIPEWRITER_H
