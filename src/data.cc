#include "data.h"

inline
Data::Data(){
    auto_buffer_ = new AutoBuffer();
}

inline 
Data::~Data(){
    delete auto_buffer_;
}

inline
int Data::Send(int fd){
    return auto_buffer_->WriteToFd(fd);
}

inline 
int Data::Recv(int fd){
    return auto_buffer_->ReadFromFd(fd);
}
