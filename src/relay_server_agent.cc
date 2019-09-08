#include "relay_server_agent.h"
#include "header.h"

inline
RelayServerAgent::RelayServerAgent(){
    header_ = NULL;
    data_ = NULL;
}

inline
RelayServerAgent::~RelayServerAgent(){
    if(header_ != NULL) delete header_;
    if(data_ != NULL) delete data_;
}

inline 
Datagram* RelayServerAgent::header(){
    return header_;
}

inline 
void RelayServerAgent::set_header(Datagram* header){
    header_ = header;
}

inline
Datagram* RelayServerAgent::data(){
    return data_;
}

inline
void RelayServerAgent::set_data(Datagram* data){
    data_ = data;
}

inline
void RelayServerAgent::SendError(int err){
    Header header(err);
    header.Send(fd_);
}


