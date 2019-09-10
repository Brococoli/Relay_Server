#include "../include/epoll.h"

inline
Epoll::Epoll(){
    epoll_fd_ = epoll_create1(0);
    ev_ = new struct epoll_event;
}

inline
Epoll::~Epoll(){
    close(epoll_fd_);
    delete ev_;
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
int Epoll::AddFd(int fd, int event, Agent* agent){
    agent->set_fd(fd);
    ev_->data.ptr = agent;
    ev_->events = event;
    return epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, ev_);
}

inline
int Epoll::DeleteFd(int fd){
    return epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, NULL);
}

inline
int Epoll::ModFdsEvent(int fd, struct epoll_event* event){
    return epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, fd, event);
}
