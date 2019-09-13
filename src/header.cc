#include "../include/header.h"

/* inline */
Header::Header(){
    byte_size_ = 0;
    my_user_id_ = -1;
    to_user_id_ = -1;
    datagram_type_ = 0;
    reserved_position_ = 0;
    normal_buffer_ = new NormalBuffer(header_size_);

    left_to_read_ = header_size_;
}

/* inline */
Header::Header(int err){
    byte_size_ = 0;
    to_user_id_ = -1;
    reserved_position_ = 0;
    normal_buffer_ = new NormalBuffer(header_size_);

    datagram_type_ = err;
    left_to_read_ = 0;

}
Header::~Header(){
    if(normal_buffer_) delete normal_buffer_;
}
int Header::Clear(){
    byte_size_ = 0;
    my_user_id_ = -1;
    to_user_id_ = -1;
    datagram_type_ = 0;
    reserved_position_ = 0;
    left_to_read_ = header_size_;
    normal_buffer_->Clear();
    return 1;
}
int Header::byte_size(){
    return byte_size_;
}
void Header::set_byte_size(int byte_size){
    byte_size_ = byte_size;
}

int Header::to_user_id(){
    return to_user_id_;
}
void Header::set_to_user_id(int to_user_id){
    to_user_id_ = to_user_id;

}


int Header::ToCharArray(char* char_array, size_t array_size){
    if(array_size < header_size_) return -1;
    return sprintf(char_array, "%d %d %d %d %d", byte_size_, my_user_id_, to_user_id_, datagram_type_, reserved_position_);
}

int Header::ToDatagram(Datagram* datagram, char* char_array, size_t size){
    /* Header* header = static_cast<Header*>(datagram); */
    Header* header = dynamic_cast<Header*>(datagram);
    if(size < header_size_) return -1;

    int byte_size, my_user_id, to_user_id, data_type, reserved_position;
    sscanf(char_array, "%d %d %d %d %d", &byte_size, &my_user_id, &to_user_id, &data_type, &reserved_position);
    header->byte_size_ = byte_size;
    /* header->set_byte_size(byte_size); */
    header->my_user_id_ = my_user_id;
    header->to_user_id_ = to_user_id;
    header->datagram_type_ = data_type;
    header->reserved_position_ = reserved_position;
    return 0;
}

int Header::Send(int fd){
    /* char mess[header_size_] = {}; */
    /* ToCharArray(mess, header_size_); */
    /* normal_buffer_->ReadFromCharArray(mess, sizeof(mess)); */
    return normal_buffer_->WriteToFd(fd);
}

int Header::Recv(int fd){
    /* normal_buffer_->Clear(); */
    int status = normal_buffer_->ReadFromFd(fd, left_to_read_);
    if(status < 0){
        if(errno == EWOULDBLOCK)
            return EWOULDBLOCK;
        else
            err_sys("header recv error");
    } 

    left_to_read_ -= status;
    if(left_to_read_ == 0){
        char mess[header_size_] = {};
        normal_buffer_->WriteToCharArray(mess, header_size_);
        ToDatagram(this, mess, sizeof(mess));
        /* normal_buffer_->Clear(); */
        return SUCCESS;
    }
    return 1;
}
