#ifndef AGENT_H_
#define AGENT_H_

#include "datagram.h"
#include "auto_buffer.h"
#include "normal_buffer.h"

class Agent
{
public:
    Agent();
    Agent(int fd);
    virtual ~Agent() {}

    int fd();
    void set_fd(int fd);

    bool read_eof();
    void set_read_eof(bool read_eof);

    virtual int SendDatagram(Datagram* datagram);
    static int SendDatagramToFd(int fd, Datagram* datagram);
    virtual int RecvDatagram(Datagram* datagram);
    static int RecvDatagramFromFd(int fd, Datagram* datagram);

protected:
    bool read_eof_;
    int fd_;

};

#endif

