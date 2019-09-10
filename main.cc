#include <iostream>
#include "epoll.h"
#include "relay_server_agent.h"

#include "sys/socket.h"

int main()
{
    int listenfd, fd, event;
    RelayServerAgent* agent;

    Epoll epoll = Epoll();
    epoll.AddFd(listenfd, EPOLLIN, new RelayServerAgent(listenfd));

    for(;;){
        int nfds = epoll.Wait(1024, -1);
        for(int i=0; i< nfds; ++i){
            agent = static_cast<RelayServerAgent*>(epoll.events[i].data.ptr);
            event = epoll.events[i].events;
            fd = agent->fd();
            if(fd == listenfd){
                struct sockaddr sa;
                socklen_t socklen = sizeof(sa);
                while((fd = accept(listenfd, &sa, &socklen)) > 0){
                    epoll.AddFd(fd, EPOLLIN | EPOLLOUT, new RelayServerAgent(fd));
                }
            }
            else{
                if(fd == EPOLLIN){
                    if(agent->isHeaderEmpty()){
                        
                        agent->RecvDatagram(agent->header());

                        
                    }
                    else{
                        agent->RecvDatagram(agent->data());
                    }
                    
                }
                if(fd == EPOLLOUT){
                    if(!agent->isHeaderEmpty()){
                        //i should add a flag in relay_server_agent to show if i have send header or recieve a header;
                        Agent::SendDatagram(agent->header());
                    }
                    if(agent->data() == NULL){
                        agent->set_data(new Data);
                        Agent::SendDatagram(agent->data());
                    }

                }
            }

        }

        
    }


    return 0;
}

