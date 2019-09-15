#ifndef DATA_H_
#define DATA_H_

#include "normal_buffer.h"

#include "datagram.h"

#include "assert.h"

class Data : public Datagram
{
public:
    Data();
    virtual ~Data();

    Buffer* auto_buffer();
    void set_auto_buffer(Buffer* auto_buffer);

    virtual int Send(int fd);
    virtual int Recv(int fd);
    virtual int Send(int fd, size_t read_size);
    virtual int Recv(int fd, size_t write_size);

    virtual int Clear();

private:
    /* int data_type_;   //to show whether is header or data, moreover if is -1, then reserved_position_ will show the errno */
    /* int left_to_read_; */
    int reserved_position_;
    Buffer* auto_buffer_;

};

#endif
