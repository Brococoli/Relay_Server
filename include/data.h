#ifndef DATA_H_
#define DATA_H_

#include "auto_buffer.h"

#include "datagram.h"

class Data : public Datagram
{
public:
    Data();
    virtual ~Data();

    Buffer* auto_buffer();
    void set_auto_buffer(Buffer* auto_buffer);

    virtual int Send(int fd);
    virtual int Recv(int fd);

private:
    Buffer* auto_buffer_;

};

#endif
