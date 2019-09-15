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

    virtual int Full() const = 0;
    virtual int Empty() const = 0;
    virtual size_t Size() const = 0;

    virtual int ReadFromFd(int fd) = 0;
    virtual int ReadFromFd(int fd, size_t read_size) = 0;
    virtual int WriteToFd(int fd) = 0;
    virtual int WriteToFd(int fd, size_t write_size) = 0;
    virtual int Clear() = 0;

protected:
    /* virtual void Init(size_t size) = 0; */
    /* virtual void Free() = 0; */

};

#endif // BUFFER_H_

