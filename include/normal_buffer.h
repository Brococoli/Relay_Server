#ifndef NORMAL_BUFFER_H_
#define NORMAL_BUFFER_H_

#include "buffer.h"

class NormalBuffer: public Buffer
{
public:
    NormalBuffer();
    explicit NormalBuffer(size_t size);
    NormalBuffer(char* buf, size_t size);
    ~NormalBuffer();

    virtual int Full();
    virtual int Empty();
    virtual int ReadFromFd(int fd);
    virtual int WriteToFd(int fd);
    virtual int ReadFromCharArray(char* from, size_t size);
    virtual int WriteToCharArray(char* to, size_t size);
    int Resize(size_t size);

protected:
    virtual Buffer* Init(size_t size);
    virtual void Free();

private:
    char* ptr_;
    size_t front_, rear_;
    size_t size_;

};

#endif

