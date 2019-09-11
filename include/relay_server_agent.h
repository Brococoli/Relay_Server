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

    bool recv_header();
    void set_recv_header(bool);
    bool send_header();
    void set_send_header(bool);
    bool recv_data();
    void set_recv_data(bool);
    bool send_data();
    void set_send_data(bool);

    void SendError(int err);

private:
    Datagram* header_, *data_;
    bool recv_header_, send_header_;
    bool recv_data_, send_data_;


};
#endif

