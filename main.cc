#include <iostream>
#include "epoll.h"
#include "relay_server_agent.h"
#include "utils.h"
#include "user_manager.h"

#include "sys/socket.h"
#include "netinet/in.h"

int main()
{
    int listenfd, fd, event;
    RelayServerAgent* agent;

    struct sockaddr_in servaddr;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(9888);
    servaddr.sin_addr.s_addr = htonl(0);

    AddFL(listenfd, O_NONBLOCK);

    bind(listenfd, (struct sockaddr*) &servaddr, sizeof(servaddr));

    listen(listenfd, 1024);

    UserManager user_manager;
    

    Epoll epoll = Epoll();
    epoll.AddFd(listenfd, EPOLLIN, new RelayServerAgent(listenfd));

    for(;;){
        int ndfs = epoll.Wait(1024, -1);
        for(int i=0; i< ndfs; ++i){
            agent = static_cast<RelayServerAgent*>(epoll.events[i].data.ptr);
            event = epoll.events[i].events;
            fd = agent->fd();
            if(fd == listenfd){
                struct sockaddr sa;
                socklen_t socklen = sizeof(sa);
                while((fd = accept(listenfd, &sa, &socklen)) > 0){
                    AddFL(fd, O_NONBLOCK);
                    epoll.AddFd(fd, EPOLLIN, new RelayServerAgent(fd));
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
                                epoll.ModFdsEvent(fd, EPOLLIN | EPOLLOUT, agent);
                            }
                        }
                    }
                    if(agent->recv_header() && !agent->recv_data()){
                        if(agent->data()->Recv(agent->fd()) == SUCCESS)
                            agent->set_recv_data(true);
                    }
                    
                }
                if(fd == EPOLLOUT){
                    if(agent->recv_data()&&!agent->send_data()){
                        int to_user_id = dynamic_cast<Header*>(agent->header())->to_user_id();
                        if(user_manager.Exist(to_user_id) == false){
                            agent->SendError(Agent::ENOTEXIST); //send error but not close the connect
                        }
                        if(user_manager.Online(to_user_id) == true){
                            if(agent->data()->Send(to_user_id) == SUCCESS){
                                agent->set_send_data(true);
                            }
                        }
                        else{
                            agent->SendError(Agent::ENOTONLINE);
                        }
                    }
                }
            }

        }

        
    }


    return 0;
}

