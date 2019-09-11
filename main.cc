#include <iostream>
#include "epoll.h"
#include "relay_server_agent.h"
#include "utils.h"
#include "user_manager.h"

#include "sys/socket.h"


int main()
{
    int listenfd, fd, event;
    RelayServerAgent* agent;

    UserManager user_manager;
    

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
                    if(!agent->recv_header()){

                        if(agent->header()->Recv(agent->fd()) == SUCCESS){
                            if(agent->header()->datagram_type() < 0){
                                close(agent->fd());
                                epoll.DeleteFd(agent->fd());
                                delete agent;
                                continue;
                            }
                            else{
                                user_manager.Register(new User(dynamic_cast<Header*>(agent->header())->to_user_id(),
                                                               agent->fd(),
                                                               true));
                                agent->set_recv_header(true);
                            }
                            

                        }
                        
                    }
                    else{
                        if(agent->data()->Recv(agent->fd()) == SUCCESS)
                            agent->set_recv_data(true);
                    }
                    
                }
                if(fd == EPOLLOUT){
                    if(agent->recv_header()&&!agent->send_data()){
                        int to_user_id = dynamic_cast<Header*>(agent->header())->to_user_id();
                        if(user_manager.Online(to_user_id) == true){
                            if(agent->data()->Send(to_user_id) == SUCCESS){
                                agent->set_send_data(true);
                            }
                        }

                    }

                }
            }

        }

        
    }


    return 0;
}

