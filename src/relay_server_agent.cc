#include "../include/relay_server_agent.h"

/* inline */
/* RelayServerAgent::RelayServerAgent(){ */
/*     /1* header_ = NULL; *1/ */
/*     /1* data_ = NULL; *1/ */
/*     header_ = new Header(); */
/*     data_ = new Data(); */
/* } */
inline 
RelayServerAgent::RelayServerAgent(int fd){
    /* header_ = new Header(); */
    /* data_ = new Data(); */
    header_ = data_ = NULL;
    fd_ = -1;
}

inline
RelayServerAgent::~RelayServerAgent(){
    if(header_ != NULL) delete header_;
    if(data_ != NULL) delete data_;
}

inline
bool RelayServerAgent::isHeaderEmpty(){
    return header_ == NULL;
}
inline
bool RelayServerAgent::isDataEmpty(){
    return data_ == NULL;
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


