#include "../include/data.h"

/* inline */
Data::Data(){
    reserved_position_ = 0;
    left_to_read_ = 0;
    left_to_write_ = 0;
    auto_buffer_ = new AutoBuffer();
}

/* inline */ 
Data::~Data(){
    if(auto_buffer_) 
        delete auto_buffer_;
    auto_buffer_ = NULL;
}
int Data::Clear(){
    reserved_position_ = 0;
    left_to_write_ = 0;
    left_to_read_ = 0;
    auto_buffer_->Clear();
    return 1;
}

/* inline */
int Data::Send(int fd){
    int status = auto_buffer_->WriteToFd(fd);

    if(status < 0){
        if(errno == EWOULDBLOCK)
            return EWOULDBLOCK;
        err_sys("Data::Send error");
    }
    left_to_write_ -= status;
    if(left_to_write_ == 0){
        auto_buffer_->Clear();
        return SUCCESS;
    }
    else{
        return ENOTDONE;
    }
}
int Data::Send(int fd, size_t write_size){
    int status = auto_buffer_->WriteToFd(fd, write_size);
    if(status < 0){
        if(errno == EWOULDBLOCK)
            return EWOULDBLOCK;
        err_sys("Data::Send error");
    }
    if(write_size == static_cast<size_t>(status)){
        return SUCCESS;
    }
    else{
        return status;
    }
}

/* inline */ 
int Data::Recv(int fd){
    /* assert(left_to_read_ != 0); */
    int status = auto_buffer_->ReadFromFd(fd, left_to_read_);
    if(status < 0){
        if(errno == EWOULDBLOCK)
            return EWOULDBLOCK;
        err_sys("data recv error");
    }

    left_to_read_ -= status;
    if(left_to_read_ == 0){
        return SUCCESS;
    }
    else{
        return ENOTDONE;
    }
}
int Data::Recv(int fd, size_t read_size){
    int status = auto_buffer_->ReadFromFd(fd, read_size);
    if(status < 0){
        if(errno == EWOULDBLOCK)
            return EWOULDBLOCK;
        err_sys("data recv error");
    }
    if(read_size == static_cast<size_t>(status))
        return SUCCESS;
    else
        return status;

}
