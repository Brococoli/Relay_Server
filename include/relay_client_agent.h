#ifndef RELAY_CLIENT_AGENT_H_
#define RELAY_CLIENT_AGENT_H_
#include "relay_server_agent.h"
#include "normal_buffer.h"

class RelayClientAgent : public RelayServerAgent
{
public:
    explicit RelayClientAgent(int fd, int user_id, int data_size):RelayServerAgent(fd), user_id_(user_id) {
        Data* data = dynamic_cast<Data*>(data_);
        if(data->auto_buffer()) free(data->auto_buffer());
            data->set_auto_buffer(new NormalBuffer(data_size));
    }
    ~RelayClientAgent() {}

    int user_id() const { return user_id_; }

private:
    int user_id_;

};
#endif
