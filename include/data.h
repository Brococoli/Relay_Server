#ifndef DATA_H_
#define DATA_H_

#ifndef AUTO_BUFFER_H_
#include "auto_buffer.h"
#endif

#ifndef DATAGRAM_H_
#include "datagram.h"
#endif

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
