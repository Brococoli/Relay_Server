#include "../include/header.h"

/* inline */
Header::Header(){
    byte_size_ = 16;
    to_user_id_ = -1;
    datagram_type_ = 0;
    reserved_position_ = 0;
    normal_buffer_ = new NormalBuffer(20);
}

/* inline */
Header::Header(int err){
    byte_size_ = 16;
    to_user_id_ = -1;
    reserved_position_ = 0;
    normal_buffer_ = new NormalBuffer(20);

    datagram_type_ = err;

}
Header::~Header(){
    if(normal_buffer_) delete normal_buffer_;
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
    if(array_size < 16) return -1;
    return sprintf(char_array, " %d %d %d %d ", byte_size_, to_user_id_, datagram_type_, reserved_position_);
}

int Header::ToDatagram(Datagram* datagram, char* char_array, size_t size){
    /* Header* header = static_cast<Header*>(datagram); */
    Header* header = dynamic_cast<Header*>(datagram);
    if(size < 16) return -1;

    int byte_size, to_user_id, data_type, reserved_position;
    sscanf(char_array, " %d %d %d %d ", &byte_size, &to_user_id, &data_type, &reserved_position);
    header->byte_size_ = byte_size;
    /* header->set_byte_size(byte_size); */
    header->to_user_id_ = to_user_id;
    header->datagram_type_ = data_type;
    header->reserved_position_ = reserved_position;
    return 0;
}

int Header::Send(int fd){
    char mess[20] = {};
    ToCharArray(mess, 20);
    normal_buffer_->ReadFromCharArray(mess, sizeof(mess));
    return normal_buffer_->WriteToFd(fd);
}

int Header::Recv(int fd){
    int status = normal_buffer_->ReadFromFd(fd);
    if(status == EWOULDBLOCK) return EWOULDBLOCK;
    char mess[20] = {};
    normal_buffer_->WriteToCharArray(mess, 20);
    ToDatagram(this, mess, sizeof(mess));
    return 0;
}
