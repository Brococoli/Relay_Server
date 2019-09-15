#include <iostream>
#include "epoll.h"
#include "relay_server_agent.h"
#include "utils.h"
#include "socket_utils.h"
#include "user_manager.h"

#include "sys/socket.h"
#include "netinet/in.h"

int main()
{
    int listenfd, fd, event, ret;
    RelayServerAgent* agent;

    struct sockaddr_in servaddr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    SetSocketAddr(&servaddr, AF_INET, INADDR_ANY, 9888);

    AddFL(listenfd, O_NONBLOCK);

    bind(listenfd, (struct sockaddr*) &servaddr, sizeof(servaddr));

    listen(listenfd, LISTENQ);

    UserManager user_manager;


    Epoll epoll = Epoll();
    epoll.AddFd(listenfd, EPOLLIN, new RelayServerAgent(listenfd));
    for(;;){
        int ndfs = epoll.Wait(1024, -1);
        for(int i=0; i< ndfs; ++i){
            agent = static_cast<RelayServerAgent*>(epoll.events[i].data.ptr);
            event = epoll.events[i].events;
            fd = agent->fd();
            fprintf(stdout, "fd: %d, event: %s, %s, %s\n", fd, (event&EPOLLIN)?"EPOLLIN":"NOTIN", (event&EPOLLOUT)?"EPOLLOUT":"NOTOUT", (event&EPOLLRDHUP)?"EPOLLRDHUP": "NOEPOLLRDHUB");
            if(fd == listenfd){
                struct sockaddr sa;
                socklen_t socklen = sizeof(sa);
                while((fd = accept(listenfd, &sa, &socklen)) > 0){
                    AddFL(fd, O_NONBLOCK);
                    epoll.AddFd(fd, EPOLLIN | EPOLLOUT | EPOLLRDHUP, new RelayServerAgent(fd));
                }
            }
            else{
                Header* header = dynamic_cast<Header*>(agent->header());
                Data* data = dynamic_cast<Data*>(agent->data());

                if(event & EPOLLRDHUP){
                    close(agent->fd());
                    epoll.DeleteFd(agent->fd());
                    delete agent;
                    continue;

                }

            
                if(event & EPOLLIN){
                    if(!agent->recv_header()){
                        if(header->left_to_read()==0)
                            header->set_left_to_read(Header::header_size());

                        ret = agent->header()->Recv(agent->fd());
                        if(ret == SUCCESS){
                            agent->set_recv_header(true);
                            if(!user_manager.Exist(header->my_user_id()))
                                user_manager.Register(new User(header->my_user_id(),
                                                               agent->fd(),
                                                               true));
                            else{
                                user_manager.Login(header->my_user_id());
                            }

                        }
                    }
                    else{
                        if(!agent->recv_data()){
                            if(data->left_to_read() == 0)
                                data->set_left_to_read(header->byte_size());
                            if(user_manager.Exist(header->to_user_id())){
                                ret = data->Recv(fd);
                                if(ret == SUCCESS)
                                    agent->set_recv_data(true);
                            }
                        }
                    }
                }
                if(event & EPOLLOUT){
                    if(!agent->recv_header()) continue;
                    if(!user_manager.Exist(header->to_user_id())){
                        agent->SendError(header->to_user_id(), Agent::ENOTEXIST);
                        agent->Clear();

                        close(agent->fd());
                        epoll.DeleteFd(agent->fd());
                        delete agent;

                        continue;
                    }
                    int to_fd = user_manager.GetUserFd(header->to_user_id());
                    if(!agent->send_header()){
                        if(header->left_to_write() == 0)
                            header->set_left_to_write(Header::header_size());
                        ret = header->Send(to_fd);
                        if(ret == SUCCESS)
                            agent->set_send_header(true);
                    }
                    if(agent->send_header()){

                        if(user_manager.Online(header->to_user_id())){

                            if(data->left_to_write() == 0)
                                data->set_left_to_write(header->byte_size());

                            ret = agent->data()->Send(to_fd);
                            if(ret == SUCCESS){
                                agent->set_send_data(true);
                                agent->Clear();

                            }
                        }
                        else{
                            agent->SendError(fd, Agent::ENOTONLINE);
                        }
                    }
                    /* if(event & EPOLLIN){ */
                    /*     if(!agent->recv_header()){ */

                    /*         if(agent->header()->Recv(agent->fd()) == SUCCESS){ */
                    /*             if(agent->header()->datagram_type() < 0){ */
                    /*                 close(agent->fd()); */
                    /*                 epoll.DeleteFd(agent->fd()); */
                    /*                 user_manager.Logout(header->my_user_id()); */
                    /*                 delete agent; */
                    /*                 continue; */
                    /*             } */
                    /*             else{ */
                    /*                 if(!user_manager.Exist(header->my_user_id())) */
                    /*                     user_manager.Register(new User(header->my_user_id(), agent->fd(), true)); */
                    /*                 else */
                    /*                     user_manager.Online(header->my_user_id()); */
                    /*                 agent->set_recv_header(true); */
                    /*                 data->set_left_to_read(header->byte_size()); */
                    /*             } */
                    /*         } */
                    /*     } */
                    /*     if(agent->recv_header() && !agent->recv_data()){ */
                    /*         if(agent->data()->Recv(agent->fd()) == SUCCESS)  //no matter the to_id is online, we just recv the data */
                    /*             agent->set_recv_data(true); */
                    /*     } */

                    /* } */
                    /* if(event & EPOLLOUT){ */
                    /*     if(agent->recv_header()&&!agent->send_header()){ */
                    /*         if(user_manager.Exist(header->to_user_id()) == false){ */
                    /*             agent->SendError(Agent::ENOTEXIST); //send error but not close the connect */
                    /*             agent->Clear(); */
                    /*             continue; */
                    /*         } */
                    /*         else{ */
                    /*             if(user_manager.Online(header->to_user_id()) == true){ */
                    /*                 if(agent->header()->Send(user_manager.GetUserFd(header->to_user_id())) == SUCCESS){ */
                    /*                     agent->set_send_header(true); */

                    /*                 } */
                    /*             } */
                    /*             else{ */
                    /*                 agent->SendError(Agent::ENOTONLINE); */
                    /*                 agent->Clear(); */
                    /*                 continue; */
                    /*             } */

                    /*         } */

                    /*     } */
                    /*     if(agent->send_header()&&!agent->send_data()){ */
                    /*         if(user_manager.Online(header->to_user_id()) == true){ */
                    /*             if(agent->data()->Send(user_manager.GetUserFd(header->to_user_id())) == SUCCESS){ */
                    /*                 if(agent->recv_data()){ */
                    /*                     agent->set_send_data(true); */
                    /*                     agent->Clear(); */
                    /*                 } */
                    /*             } */
                    /*         } */
                    /*     } */
                    /* } */
                }

            }


        }

    }
    return 0;
}

