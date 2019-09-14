#include "../include/relay_server_agent.h"

/* inline */
/* RelayServerAgent::RelayServerAgent(){ */
/*     /1* header_ = NULL; *1/ */
/*     /1* data_ = NULL; *1/ */
/*     header_ = new Header(); */
/*     data_ = new Data(); */
/* } */
/* inline */ 
RelayServerAgent::RelayServerAgent(int fd){
    header_ = new Header();
    data_ = new Data();
    recv_header_ = send_header_ = recv_data_ = send_data_ = false;
    fd_ = fd;
}

/* inline */
RelayServerAgent::~RelayServerAgent(){
    if(header_ != NULL) delete header_;
    if(data_ != NULL) delete data_;
}
int RelayServerAgent::Clear(){
    recv_header_ = send_header_ = recv_data_ = send_data_ = false;
    header_->Clear();
    data_->Clear();
    return 1;
}

/* inline */ 
bool RelayServerAgent::recv_header(){
    return recv_header_;
}

/* inline */
bool RelayServerAgent::send_header(){
    return send_header_;
}

/* inline */
void RelayServerAgent::set_recv_header(bool recv_header){
    recv_header_ = recv_header;
}

/* inline */
void RelayServerAgent::set_send_header(bool send_header){
    send_header_ = send_header;
}
/* inline */ 
bool RelayServerAgent::recv_data(){
    return recv_data_;
}

/* inline */
bool RelayServerAgent::send_data(){
    return send_data_;
}

/* inline */
void RelayServerAgent::set_recv_data(bool recv_data){
    recv_data_ = recv_data;
}

/* inline */
void RelayServerAgent::set_send_data(bool send_data){
    send_data_ = send_data;
}

/* inline */ 
Datagram* RelayServerAgent::header(){
    return header_;
}

/* inline */ 
void RelayServerAgent::set_header(Datagram* header){
    header_ = header;
}

/* inline */
Datagram* RelayServerAgent::data(){
    return data_;
}

/* inline */
void RelayServerAgent::set_data(Datagram* data){
    data_ = data;
}

/* inline */
void RelayServerAgent::SendError(int err){
    header_->set_datagram_type(err);  //need to modify
    dynamic_cast<Header*>(header_)->set_left_to_write(Header::header_size());
}


