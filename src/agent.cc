#include "../include/agent.h"
Agent::Agent(){
    read_eof_ = false;
    fd_ = -1;
}
inline
Agent::Agent(int fd){
    fd_ = fd;
}

inline
int Agent::fd(){
    return fd_;
}

inline
void Agent::set_fd(int fd){
    fd_ = fd;
}

inline
bool Agent::read_eof(){
    return read_eof_;
}

inline
void Agent::set_read_eof(bool read_eof){
    read_eof_ = read_eof;
}

/* int Agent::SendDatagram(Datagram* datagram){ */
/*     Buffer* buf = new AutoBuffer(); */
/*     datagram->ToBuffer(buf); */
/*     buf->WriteToFd(fd_); */
/*     delete buf; */
/* } */

/* int Agent::RecvDatagram(Datagram* datagram){ */
/*     Buffer* buf = new AutoBuffer(); */
/*     buf->ReadFromFd(fd_); */
/*     datagram->ToDatagram(buf); */
/*     delete buf; */
/* } */

int Agent::SendDatagram(Datagram* datagram){
    datagram->Send(fd_);
}
int Agent::SendDatagramToFd(int fd, Datagram* datagram){
    datagram->Send(fd);
}
int Agent::RecvDatagram(Datagram* datagram){
    datagram->Recv(fd_);
}
int Agent::RecvDatagramFromFd(int fd, Datagram* datagram){
    datagram->Recv(fd);
}
