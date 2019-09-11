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
    int data_type_;   //to show whether is header or data, moreover if is -1, then reserved_position_ will show the errno
    int reserved_position_;
    Buffer* auto_buffer_;

};

#endif
