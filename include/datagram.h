#ifndef DATAGRAM_H_
#define DATAGRAM_H_

#include <stddef.h>

#include "auto_buffer.h"

class Datagram
{
public:
    Datagram() { datagram_type_ = -1; }
    virtual ~Datagram() {}

    /* virtual int ToCharArray(char* char_array, size_t array_size) = 0; */
    /* virtual int ToDatagram(Datagram* Datagram, char* char_array, size_t array_size) = 0; */
    /* virtual Datagram* ToDatagram(Buffer* buf) = 0; */
    /* virtual void ToBuffer(Buffer* buf) = 0; */

    virtual int Send(int fd) = 0;
    virtual int Recv(int fd) = 0;
    virtual int Send(int fd, size_t read_size) = 0;
    virtual int Recv(int fd, size_t write_size) = 0;
    virtual int Clear() = 0;

    int datagram_type() const ;
    void set_datagram_type(int);
    int left_to_read() const { return left_to_read_; }
    void set_left_to_read(int left_to_read) { left_to_read_ = left_to_read; }
    int left_to_write() const { return left_to_write_; }
    void set_left_to_write(int left_to_write) { left_to_write_ = left_to_write; }

protected:
    int datagram_type_;
    int left_to_read_;
    int left_to_write_;

};

#endif
