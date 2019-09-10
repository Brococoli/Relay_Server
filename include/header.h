#ifndef HEADER_H_
#define HEADER_H_

#ifndef DATAGRAM_H_
#include "datagram.h"
#endif

#ifndef NORMAL_BUFFER_H_
#include "normal_buffer.h"
#endif

class Header : public Datagram
{
public:
    Header();
    explicit Header(int err);
    virtual ~Header();

    int byte_size();
    void set_byte_size(int byte_size);

    int to_user_id();
    void set_to_user_id(int to_user_id);

    int data_type();
    void set_data_type(int data_type);

    virtual int ToCharArray(char* char_array, size_t array_size);
    virtual int ToDatagram(Header& header, char* char_array, size_t size) ;

    virtual int SendDatagram(Datagram* datagram);
    virtual int RecvDatagram(Datagram* datagram);

    virtual int Send(int fd);
    virtual int Recv(int fd);
    

private:
    int byte_size_;
    int to_user_id_;
    int data_type_;
    int reserved_position_;
    Buffer* normal_buffer_;
    

};

#endif
