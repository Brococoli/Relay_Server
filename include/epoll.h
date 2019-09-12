#ifndef EPOLL_H_
#define EPOLL_H_

#include <sys/epoll.h>

#include "agent.h"

#define MAXEVENTS 1024

class Epoll
{
public:
    Epoll();
    virtual ~Epoll();
    struct epoll_event events[MAXEVENTS];
    struct epoll_event* ev_;


    int epoll_fd();
    void set_epoll_fd(int epol_fd_);

    int Create();
    int Wait(int maxevents, int timeout);
    int AddFd(int fd, int monitor_event, Agent* agent);
    int DeleteFd(int fd);
    int ModFdsEvent(int fd, int monitor_event, Agent* agent);

private:
    int epoll_fd_;


};
#endif

