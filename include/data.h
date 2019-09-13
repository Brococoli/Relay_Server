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

    int left_to_read() const { return left_to_read_; }
    void set_left_to_read(int left_to_read) { left_to_read_ = left_to_read; }

    virtual int Send(int fd);
    virtual int Recv(int fd);

    virtual int Clear();

private:
    /* int data_type_;   //to show whether is header or data, moreover if is -1, then reserved_position_ will show the errno */
    int left_to_read_;
    int reserved_position_;
    Buffer* auto_buffer_;

};

#endif
