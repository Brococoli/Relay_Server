#include "../include/normal_buffer.h"

NormalBuffer::NormalBuffer(){
    NormalBuffer(4096);
}

NormalBuffer::NormalBuffer(size_t size){
    ptr_ = NULL;
    front_ = rear_ = 0;
    size_ = size;
    Init(size);
}

NormalBuffer::NormalBuffer(char* buf, size_t size){
    ptr_ = buf;
    front_ = rear_ = 0;
    size_ = size;
}

NormalBuffer::~NormalBuffer(){
    if(ptr_) delete ptr_;
}

int NormalBuffer::Full(){
    return front_ == size_;
    //because the last position is '\0';
}
int NormalBuffer::Empty(){
    return front_ == rear_;
}

Buffer* NormalBuffer::Init(size_t size){
    if(size_ > MAXBUFSIZE) {
        fprintf(stderr, "size_ is larger than %d", MAXBUFSIZE);
        return NULL;
    }
    NormalBuffer* buf = (NormalBuffer*)malloc(sizeof(NormalBuffer));
    size_ = size;
    ptr_ = (char*)malloc(sizeof(char)*size_);
    if(ptr_ == NULL) {
        err_sys("InitBuf error: malloc error");
        return NULL;
    }
    front_ = rear_ = 0;
    return buf;
}
void NormalBuffer::Free(){
    if(ptr_) free(ptr_);
    front_ = rear_ = 0;
}

int NormalBuffer::WriteToFd(int fd){
    // same as the read, The function return the byte number which actually read from fd.
    if(Empty()) return 0;
    int status = write(fd, ptr_ + rear_, front_ - rear_);
    if(status > 0) rear_ += status;
    if(rear_ != 0 && rear_ == front_)
        rear_ = front_ = 0;
    return status;
}
int NormalBuffer::ReadFromFd(int fd){
    // same as the write, The function return the byte number which actually write to file.
    if(Full()) return 0;
    int status = read(fd, ptr_ + front_, size_ - front_);
    if(status > 0) front_ += status; //in case of the status is -1;
    return status;
}

int NormalBuffer::Resize(size_t size){
    if(size <= size_) return -1;
    void* ret = realloc(ptr_, size);
    if(ret == NULL) return -1;
    else return 0;
}

int NormalBuffer::ReadFromCharArray(char* from, size_t size){
    if(size > size_) Resize(rear_ - front_ + size);
    void* ret = memcpy(ptr_ + rear_, from, size);
    if(ret == NULL) return -1;
    else 
        return 0;
} 
