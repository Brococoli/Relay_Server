#include "../include/header.h"

/* inline */
Header::Header(){
    byte_size_ = 0;
    my_user_id_ = -1;
    to_user_id_ = -1;
    datagram_type_ = 0;
    reserved_position_ = 0;
    normal_buffer_ = new NormalBuffer(header_size_);

    left_to_read_ = 0;
    left_to_write_ = 0;
}

/* inline */
Header::Header(int err){
    byte_size_ = 0;
    my_user_id_ = -1;
    to_user_id_ = -1;
    reserved_position_ = 0;
    normal_buffer_ = new NormalBuffer(header_size_);

    datagram_type_ = err;
    left_to_read_ = 0;
    left_to_write_ = header_size_;

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
    left_to_write_ = 0;
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


/* int Header::ToNormalBuffer(){ */
/*     normal_buffer_->Clear(); */
/*     normal_buffer_->AddByte(header_size_); */ 
/*     return sprintf(normal_buffer_->ptr(), "%d %d %d %d %d\n", byte_size_, my_user_id_, to_user_id_, datagram_type_, reserved_position_); */
/* } */
int Header::ToNormalBuffer(){
    normal_buffer_->Clear();

    int * pint = (int*)normal_buffer_->ptr();
    pint[0] = my_user_id_;
    pint[1] = byte_size_;
    pint[2] = to_user_id_;
    normal_buffer_->AddByte(header_size_); 

    return 1;
}

/* int Header::ToHeader(){ */
/*     /1* Header* header = static_cast<Header*>(datagram); *1/ */
/*     int byte_size, my_user_id, to_user_id, data_type, reserved_position; */
/*     sscanf(normal_buffer_->ptr(), "%d %d %d %d %d", &byte_size, &my_user_id, &to_user_id, &data_type, &reserved_position); */
/*     normal_buffer_->Clear(); */

/*     byte_size_ = byte_size; */
/*     /1* header->set_byte_size(byte_size); *1/ */
/*     my_user_id_ = my_user_id; */
/*     to_user_id_ = to_user_id; */
/*     datagram_type_ = data_type; */
/*     reserved_position_ = reserved_position; */
/*     return 0; */
/* } */

int Header::ToHeader(){
    /* Header* header = static_cast<Header*>(datagram); */

    int * pint = (int*)normal_buffer_->ptr();

    byte_size_ = *(pint + 1);
    my_user_id_ = *(pint);
    to_user_id_ = *(pint+2);
    datagram_type_ = 0;
    reserved_position_ = 0;

    normal_buffer_->Clear();
    return 0;
}
int Header::Send(int fd){
    /* assert(left_to_write_ != 0); */
    if(normal_buffer_->Empty())
        ToNormalBuffer();
    int status = normal_buffer_->WriteToFd(fd);
    if(status < 0){
        if(errno == EWOULDBLOCK)
            return EWOULDBLOCK;
        else
            err_sys("header send error");
    }
    left_to_write_ -= status;

    if(left_to_write_ == 0){
        normal_buffer_->Clear();
        return SUCCESS;
    }
    else
        return ENOTDONE;
}
int Header::Send(int fd, size_t write_size){
    /* assert(left_to_write_ != 0); */
    if(normal_buffer_->Empty())
        ToNormalBuffer();
    int status = normal_buffer_->WriteToFd(fd, write_size);
    if(status < 0){
        if(errno == EWOULDBLOCK)
            return EWOULDBLOCK;
        else
            err_sys("header send error");
    }

    if(write_size == static_cast<size_t>(status)){
        return SUCCESS;
    }
    else
        return status;
}

int Header::Recv(int fd){
    /* normal_buffer_->Clear(); */
    if(!normal_buffer_->Empty())
        normal_buffer_->Clear();
    int status = normal_buffer_->ReadFromFd(fd, left_to_read_);
    if(status < 0){
        if(errno == EWOULDBLOCK)
            return EWOULDBLOCK;
        else
            err_sys("header recv error");
    } 

    left_to_read_ -= status;
    if(left_to_read_ == 0){
        ToHeader();
        normal_buffer_->Clear();
        return SUCCESS;
    }
    else
        return ENOTDONE;
}
int Header::Recv(int fd, size_t read_size){
    /* normal_buffer_->Clear(); */
    if(!normal_buffer_->Empty()) 
        normal_buffer_->Clear();

    int status = normal_buffer_->ReadFromFd(fd, read_size);
    if(status < 0){
        if(errno == EWOULDBLOCK)
            return EWOULDBLOCK;
        else
            err_sys("header recv error");
    } 

    if(read_size == static_cast<size_t>(status)){
        return SUCCESS;
    }
    else
        return status;
}
