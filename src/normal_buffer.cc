#include "../include/normal_buffer.h"

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
    Free();
    /* if(ptr_) delete ptr_; */
}

int NormalBuffer::Full() const {
    return front_ == size_;
    //because the last position is '\0';
}
int NormalBuffer::Empty() const {
    return front_ == rear_;
}

size_t NormalBuffer::Size() const{
    return front_ - rear_;
}
                                
int NormalBuffer::Clear(){
    bzero(ptr_, size_*sizeof(char));
    front_ = rear_ = 0;
    return 1;
}

void NormalBuffer::Init(size_t size){
    if(size_ > MAXBUFSIZE) {
        fprintf(stderr, "size_ is larger than %d", MAXBUFSIZE);
        return ;
    }
    size_ = size;
    ptr_ = (char*)malloc(sizeof(char)*size_);
    if(ptr_ == NULL) {
        err_sys("InitBuf error: malloc error");
        return ;
    }
    front_ = rear_ = 0;
}
void NormalBuffer::Free(){
    if(ptr_) free(ptr_);
    front_ = rear_ = 0;
}
int NormalBuffer::AddByte(size_t size){
    if(size + front_ > size_) return -1;
    front_ += size;
    return 1;
}

int NormalBuffer::ReadByte(size_t size){
    if(size > Size()) return -1;
    rear_ += size;
    if(rear_ && rear_ == front_)
        rear_ = front_ = 0;
    return 1;
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
int NormalBuffer::WriteToFd(int fd, size_t write_size){
    if(Empty()) return 0;
    int size = min(write_size, Size());
    int status = write(fd, ptr_ + rear_, size);
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
int NormalBuffer::ReadFromFd(int fd, size_t read_size){
    // same as the write, The function return the byte number which actually write to file.
    if(front_ + read_size > MAXBUFSIZE) return -1;
    if(size_ - front_ < read_size) Resize(read_size + front_);
    int status = read(fd, ptr_ + front_, read_size);
    if(status > 0) front_ += status; //in case of the status is -1;
    return status;
}

int NormalBuffer::Resize(size_t size){
    if(size <= size_) return -1;
    void* ret = realloc(ptr_, size);
    size_ = size;
    if(ret == NULL) return -1;
    else return 0;
}

