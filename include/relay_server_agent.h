#ifndef RELAY_SERVER_AGENT_H_
#define RELAY_SERVER_AGENT_H_

#include "agent.h"
#include "datagram.h"
#include "header.h"
#include "data.h"

class RelayServerAgent: public Agent
{
public:
    RelayServerAgent(int fd) ;
    virtual ~RelayServerAgent();

    Datagram* header();
    void set_header(Datagram* header);

    Datagram* data();
    void set_data(Datagram* data);

    /* virtual int SendDatagram(Datagram* datagram); */
    /* virtual int RecvDatagram(Datagram* datagram); */
    bool isHeaderEmpty();
    bool isDataEmpty();

    void SendError(int err);

private:
    Datagram* header_, *data_;


};
#endif

