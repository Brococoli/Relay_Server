#ifndef AGENT_H_
#define AGENT_H_

#ifndef DATAGRAM_H_
#include "datagram.h"
#endif

#ifndef AUTO_BUFFER_H
#include "auto_buffer.h"
#endif

#ifndef NORMAL_BUFFER_H_
#include "normal_buffer.h"
#endif

class Agent
{
public:
    Agent();
    virtual ~Agent() {}

    int fd();
    void set_fd(int fd);

    bool read_eof();
    void set_read_eof(bool read_eof);

    virtual int SendDatagram(Datagram* datagram);
    virtual int RecvDatagram(Datagram* datagram);

protected:
    bool read_eof_;
    int fd_;

};

#endif

