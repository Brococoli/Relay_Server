#include "../include/data.h"

/* inline */
Data::Data(){
    reserved_position_ = 0;
    auto_buffer_ = new AutoBuffer();
}

/* inline */ 
Data::~Data(){
    delete auto_buffer_;
}
int Data::Clear(){
    reserved_position_ = 0;
    left_to_read_ = 0;
}

/* inline */
int Data::Send(int fd){
    return auto_buffer_->WriteToFd(fd);
}

/* inline */ 
int Data::Recv(int fd){
    int status = auto_buffer_->ReadFromFd(fd, left_to_read_);
    if(status < 0){
        if(errno == EWOULDBLOCK)
            return EWOULDBLOCK;
        err_sys("data recv error");
    }

    left_to_read_ -= status;
    if(left_to_read_ == 0)
        return SUCCESS;
    return 1;
}
