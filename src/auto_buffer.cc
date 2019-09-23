#include "../include/auto_buffer.h"


AutoBuffer::AutoBuffer(){
    front_ = rear_ = NULL;
    front_ptr_ = rear_ptr_ = 0;
    
    Init(MAXBUFBLOCKSIZE);
}

AutoBuffer::~AutoBuffer(){
    Free();
}


size_t AutoBuffer::Size() const{

    size_t ans = 0;
    for(AutoBufferNode* p = front_; p!=rear_ ; p=p->next_){
        ans ++;
    }
    ans = rear_ptr_ + MAXBUFBLOCKSIZE - front_ptr_ + (ans-1 > 0 ? ans-1:0)*MAXBUFBLOCKSIZE;
    return ans;
}
AutoBuffer::AutoBufferNode* AutoBuffer::InitBufNode(int size = MAXBUFBLOCKSIZE){
    if(size < 0 || size > MAXBUFBLOCKSIZE) return NULL;
    AutoBufferNode* node = (AutoBufferNode*)malloc(sizeof(AutoBufferNode));
    if(node == NULL) return NULL;
    node->data_ptr_ = (char*)malloc(sizeof(char)*size);
    if(node->data_ptr_ == NULL) return NULL;
    node->next_ = NULL;
    return node;
}
void AutoBuffer::Init(size_t size){
    front_ = rear_ = InitBufNode(size);
    front_ptr_ = rear_ptr_ = 0;
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
    front_ = rear_ = NULL;
}
int AutoBuffer::Clear(){
    AutoBufferNode* p, *q;
    for(p = front_->next_; p!=NULL ; p = q){
        q = p->next_;
        FreeBufNode(p);
    }
    rear_ = front_;
    front_ptr_ = rear_ptr_ = 0;
    return 1;
}
int AutoBuffer::Full() const {
    return 0;
}
int AutoBuffer::Empty() const {
    if(front_ != rear_) return 0;
    else if(front_ptr_ != rear_ptr_) return 0;
    else return 1;
}
int AutoBuffer::ReadFromFd(int fd){
    int status = read(fd, rear_->data_ptr_ + rear_ptr_, MAXBUFBLOCKSIZE - rear_ptr_);
    if(status > 0) rear_ptr_ += status;
    if(rear_ptr_ == MAXBUFBLOCKSIZE)
        PushBufNode();
    return status;
}
int AutoBuffer::ReadFromFd(int fd, size_t read_size){
    int size = min(read_size, MAXBUFBLOCKSIZE - rear_ptr_);
    int status = read(fd, rear_->data_ptr_ + rear_ptr_, size);
    if(status > 0) rear_ptr_ += status;
    if(rear_ptr_ == MAXBUFBLOCKSIZE)
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
        status = write(fd,  front_->data_ptr_ +  front_ptr_, MAXBUFBLOCKSIZE -  front_ptr_);
        if(status > 0)  front_ptr_ += status;
        if( front_ptr_ == MAXBUFBLOCKSIZE){
            PopBufNode();
        }
    }
    return status;
}

int AutoBuffer::WriteToFd(int fd, size_t write_size){
    if(Empty()) return 0;
    int status;
    size_t size = 0;
    if(front_ == rear_){
        size = min(write_size, rear_ptr_ - front_ptr_);
        status = write(fd, front_->data_ptr_ + front_ptr_,  size);
        if(status > 0)  front_ptr_ += status;
        if( front_ptr_ != 0 &&  front_ptr_ ==  rear_ptr_)
             front_ptr_ =  rear_ptr_ = 0;
    }
    else{
        size = min(write_size, MAXBUFBLOCKSIZE - front_ptr_);
        status = write(fd,  front_->data_ptr_ +  front_ptr_, MAXBUFBLOCKSIZE -  front_ptr_);
        if(status > 0)  front_ptr_ += status;
        if( front_ptr_ == MAXBUFBLOCKSIZE){
            PopBufNode();
        }
    }
    return status;
}
