#ifndef BUFFER_H_
#define BUFFER_H_
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "utils.h"

static const int MAXBUFSIZE = 8192;

class Buffer{
public:
    Buffer() {};
    explicit Buffer(size_t size);
    virtual ~Buffer() {}

    virtual int Full() = 0;
    virtual int Empty() = 0;
    virtual int ReadFromFd(int fd) = 0;
    virtual int ReadFromFd(int fd, int read_size) = 0;
    virtual int WriteToFd(int fd) = 0;
    virtual int ReadFromCharArray(char* from, size_t size) = 0;
    virtual int WriteToCharArray(char* to, size_t size) = 0;
    virtual int Clear() = 0;

protected:
    virtual void Init(size_t size) = 0;
    virtual void Free() = 0;

};

#endif // BUFFER_H_

