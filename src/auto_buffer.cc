#include "auto_buffer.h"

AutoBuffer::AutoBuffer(){
    AutoBuffer(4096);
}

AutoBuffer::AutoBuffer(size_t size){
    if(size > MAXBUFSIZE) return;
    front_ = rear_ = NULL;
    front_ptr_ = rear_ptr_ = 0;
    
    Init(size);
}

AutoBuffer::~AutoBuffer(){
    Free();
}

AutoBuffer::AutoBufferNode* AutoBuffer::InitBufNode(int size = 4096){
    if(size < 0 || size > MAXBUFSIZE) return NULL;
    AutoBufferNode* node = (AutoBufferNode*)malloc(sizeof(AutoBufferNode));
    if(node == NULL) return NULL;
    node->data_ptr_ = (char*)malloc(sizeof(char)*size);
    if(node->data_ptr_ == NULL) return NULL;
    node->next_ = NULL;
    return node;
}
Buffer* AutoBuffer::Init(size_t size){
    AutoBuffer* buf_ = (AutoBuffer*)malloc(sizeof(AutoBuffer));
    front_ = rear_ = InitBufNode(size);
    front_ptr_ = rear_ptr_ = 0;
    return buf_;
}
void AutoBuffer::PushBufNode(){
    rear_->next_ = InitBufNode();
    rear_ = rear_->next_;
    rear_ptr_ = 0;
}
void AutoBuffer::FreeBufNode(AutoBufferNode* buf_node){
    if(buf_node == NULL) return;
    if(buf_node->data_ptr_ != NULL){
        free(buf_node->data_ptr_);
    }
    free(buf_node);
}
void AutoBuffer::PopBufNode(){
    AutoBufferNode* temp = front_;
    front_ = front_->next_;
    front_ptr_ = 0;
    FreeBufNode(temp);
}
void AutoBuffer::Free(){
    AutoBufferNode* p, *q;
    for(p = front_; p!=NULL ; p = q){
        q = p->next_;
        FreeBufNode(p);
    }
    /* delete this; */
}
int AutoBuffer::Full(){
    return 0;
}
int AutoBuffer::Empty(){
    if(front_ != rear_) return 0;
    else if(front_ptr_ != rear_ptr_) return 0;
    else return 1;
}
int AutoBuffer::ReadFromFd(int fd){
    int status = read(fd, rear_->data_ptr_ + rear_ptr_, MAXBUFSIZE - rear_ptr_);
    if(status > 0) rear_ptr_ += status;
    if(rear_ptr_ == MAXBUFSIZE)
        PushBufNode();
    return status;
}
int AutoBuffer::WriteToFd(int fd){
    if(Empty()) return 0;
    int status;
    if(front_ == rear_){
        status = write(fd, front_->data_ptr_ + front_ptr_,  rear_ptr_ -  front_ptr_);
        if(status > 0)  front_ptr_ += status;
        if( front_ptr_ != 0 &&  front_ptr_ ==  rear_ptr_)
             front_ptr_ =  rear_ptr_ = 0;
    }
    else{
        status = write(fd,  front_->data_ptr_ +  front_ptr_, MAXBUFSIZE -  front_ptr_);
        if(status > 0)  front_ptr_ += status;
        if( front_ptr_ == MAXBUFSIZE){
            PopBufNode();
        }
    }
    return status;
}
int AutoBuffer::ReadFromCharArray(char* from, size_t read_size){
    if(read_size == 0) return 0;

    char* from_start = from, *from_end = from + read_size;
    while(from_end != from_start){
        size_t copy_size = min(static_cast<size_t>(from_end - from_start), MAXBUFSIZE - rear_ptr_);
        void* status = memcpy(rear_->data_ptr_ + rear_ptr_, from_start, copy_size);
        if(status != NULL) from_start += copy_size;
        else
            return -1;
        if(rear_ptr_ == MAXBUFSIZE)
                PushBufNode();
    }
    return 0;
}

int AutoBuffer::WriteToCharArray(char* to, size_t write_size){
    if(write_size == 0) return 0;

    char* to_start = to, *to_end = to + write_size;
    while(!Empty() && write_size > 0){
        size_t copy_size = 0;
        void* ret = NULL;
        if(front_ == rear_){
            copy_size = min(rear_ptr_ - front_ptr_, write_size);
            ret = memcpy(to_start, front_->data_ptr_ + front_ptr_,  copy_size);
            if(ret != NULL)  {
                front_ptr_ += copy_size;
                to_start += copy_size;
                write_size -= copy_size;
            }
            else 
                return -1;
            if(front_ptr_ != 0 &&  front_ptr_ ==  rear_ptr_)
                front_ptr_ =  rear_ptr_ = 0;
        }
        else{
            copy_size = min(MAXBUFSIZE - front_ptr_, write_size);
            ret = memcpy(to_start, front_->data_ptr_ + front_ptr_,  copy_size);
            if(ret != NULL)  {
                front_ptr_ += copy_size;
                to_start += copy_size;
                write_size -= copy_size;
            }
            else 
                return -1;
            if(front_ptr_ == MAXBUFSIZE){
                PopBufNode();
            }
        }
    }
    return 0;
}
