#ifndef HEADER_H_
#define HEADER_H_

#include "datagram.h"
#include "normal_buffer.h"
#include <assert.h>

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

    int my_user_id() const { return my_user_id_; }
    void set_my_user_id(int my_user_id) { my_user_id_ = my_user_id; }

    int left_to_read() const { return left_to_read_; }
    void set_left_to_read(int left_to_read) { left_to_read_ = left_to_read; }

    int left_to_write() const { return left_to_write_; }
    void set_left_to_write(int left_to_write) { left_to_write_ = left_to_write; }
    /* int data_type(); */
    /* void set_data_type(int data_type); */

    int ToNormalBuffer();
    int ToHeader() ;

    static  int header_size() { return header_size_; }

    /* virtual int SendDatagram(Datagram* datagram); */
    /* virtual int RecvDatagram(Datagram* datagram); */

    virtual int Send(int fd);
    virtual int Recv(int fd);
    virtual int Send(int fd, size_t read_size);
    virtual int Recv(int fd, size_t write_size);
    
    virtual int Clear();

private:
    int byte_size_;
    int my_user_id_;
    int to_user_id_;
    int reserved_position_;
    NormalBuffer* normal_buffer_;

    /* int left_to_read_; */
    /* int left_to_write_; */
    static const int header_size_ = 55;
    

};

#endif
