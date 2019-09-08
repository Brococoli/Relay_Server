#ifndef RELAY_SERVER_AGENT_H_
#define RELAY_SERVER_AGENT_H_

#ifndef AGENT_H_
#include "agent.h"
#endif

#ifndef DATAGRAM_H_
#include "datagram.h"
#endif

#ifndef HEADER_H_
#include "header.h"
#endif

#ifndef DATA_H_
#include "data.h"
#endif

class RelayServerAgent: public Agent
{
public:
    RelayServerAgent() ;
    virtual ~RelayServerAgent();

    Datagram* header();
    void set_header(Datagram* header);

    Datagram* data();
    void set_data(Datagram* data);

    virtual int SendDatagram(Datagram* datagram);
    virtual int RecvDatagram(Datagram* datagram);

    void SendError(int err);

private:
    Datagram* header_, *data_;


};
#endif

