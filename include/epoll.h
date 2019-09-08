#ifndef EPOLL_H_
#define EPOLL_H_

#include <sys/epoll.h>

#ifndef AGENT_H_
#include "agent.h"
#endif

#define MAXEVENTS 1024

class Epoll
{
public:
    Epoll();
    virtual ~Epoll();
    struct epoll_event events[MAXEVENTS];


    int epoll_fd();
    void set_epoll_fd(int epol_fd_);

    int Create();
    int Wait(int maxevents, int timeout);
    int AddFd(int fd, struct epoll_event* event, Agent* agent);
    int DeleteFd(int fd);
    int ModFdsEvent(int fd, struct epoll_event* event);

private:
    int epoll_fd_;

};
#endif

