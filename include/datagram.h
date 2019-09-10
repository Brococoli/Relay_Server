#ifndef DATAGRAM_H_
#define DATAGRAM_H_

#include <stddef.h>

#include "auto_buffer.h"

class Datagram
{
public:
    Datagram() {}
    virtual ~Datagram() {}

    /* virtual int ToCharArray(char* char_array, size_t array_size) = 0; */
    /* virtual int ToDatagram(Datagram* Datagram, char* char_array, size_t array_size) = 0; */
    /* virtual Datagram* ToDatagram(Buffer* buf) = 0; */
    /* virtual void ToBuffer(Buffer* buf) = 0; */

    virtual int Send(int fd) = 0;
    virtual int Recv(int fd) = 0;
private:

};

#endif
