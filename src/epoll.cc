#include "../include/epoll.h"

inline
Epoll::Epoll(){
    epoll_fd_ = epoll_create1(0);
}

inline
Epoll::~Epoll(){
    close(epoll_fd_);
}

inline
int Epoll::epoll_fd(){
    return epoll_fd_;
}

inline
int Epoll::Create(){
    epoll_fd_ = epoll_create1(0);
    return 1;
}

inline
int Epoll::Wait(int maxevents, int timeout){
    return epoll_wait(epoll_fd_, events, maxevents, timeout);
}

inline
int Epoll::AddFd(int fd, struct epoll_event* event, Agent* agent){
    event->data.ptr = agent;
    return epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, event);
}

inline
int Epoll::DeleteFd(int fd){
    return epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, NULL);
}

inline
int Epoll::ModFdsEvent(int fd, struct epoll_event* event){
    return epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, fd, event);
}
