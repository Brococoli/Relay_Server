#ifndef NORMAL_BUFFER_H_
#define NORMAL_BUFFER_H_

#include "buffer.h"
#include "utils.h"
#include <algorithm>
#include <string.h>

class NormalBuffer: public Buffer
{
public:
    explicit NormalBuffer(size_t size = 2048);
    NormalBuffer(char* buf, size_t size);

    virtual ~NormalBuffer();

    char* ptr() const { return ptr_; }

    virtual int Full() const ;
    virtual int Empty() const ;
    virtual size_t Size() const;

    virtual int ReadFromFd(int fd);
    virtual int ReadFromFd(int fd, size_t read_size);
    virtual int WriteToFd(int fd);
    virtual int WriteToFd(int fd, size_t write_size);
    virtual int AddByte(size_t size);
    virtual int ReadByte(size_t size);
    virtual int Clear() ;

protected:
    void Init(size_t size);
    void Free();

    int Resize(size_t size);

private:
    char* ptr_;
    size_t front_, rear_;
    size_t size_;

};

#endif

