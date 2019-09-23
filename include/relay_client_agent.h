#ifndef RELAY_CLIENT_AGENT_H_
#define RELAY_CLIENT_AGENT_H_
#include "relay_server_agent.h"
#include "normal_buffer.h"

class RelayClientAgent : public RelayServerAgent
{
public:
    explicit RelayClientAgent(int fd, int user_id, int data_size):RelayServerAgent(fd), user_id_(user_id) {
        Data* data = dynamic_cast<Data*>(data_);
        if(data->auto_buffer()) delete data->auto_buffer();
            data->set_auto_buffer(new NormalBuffer(data_size));
    }
    virtual ~RelayClientAgent() {
        if(data_) delete data_;
        if(header_) delete header_;
        data_ = header_ = 0;
    }

    int user_id() const { return user_id_; }

private:
    int user_id_;

};
#endif
