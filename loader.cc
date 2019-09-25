#include <iostream>
#include <ctime>
#include "include/user.h"
#include "include/epoll.h"
#include "include/socket_utils.h"
#include "include/relay_client_agent.h"
#include "include/utils.h"

clock_t start[10000000+5];
clock_t end[10000000+5];

int main(int argc, char* argv[])
{
	int fd, talk_links, msg_size, trans_times,all_login;
    int ret;
	struct sockaddr_in servaddr;
	int n;
	int con_n = 0;
	all_login = 0;
	if (argc != 6 || atoi(argv[5])>10000000) {
		printf("Usage: %s <ip> <port> <talk_links> <msg_size> <times(<10000000)>\n", argv[0]);
		exit(0);
	}

	talk_links = atoi(argv[3]);
	msg_size = atoi(argv[4]);
	trans_times = atoi(argv[5]);
	con_n = talk_links;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	if((n = inet_pton(AF_INET, argv[1], &servaddr.sin_addr)) < 0){
        err_sys("inet_pton error");
	}
	servaddr.sin_port = htons(atoi(argv[2]));

    Epoll epoll;
    epoll.Create();
    
    Header header;
    for(int i = 0; i < talk_links; i++){
        fd = socket(AF_INET, SOCK_STREAM, 0);
        if(fd < 0) 
            err_sys("socket error");

        if(connect(fd, (SA*)&servaddr, sizeof(servaddr)) < 0){
            err_sys("connect error");
        }
        AddFL(fd, O_NONBLOCK);

        header.set_my_user_id(i);
        header.set_to_user_id(i);
        header.set_byte_size(0);
        header.set_left_to_write(Header::header_size());
        while((ret = header.Send(fd))!=SUCCESS) {}
    
        header.set_left_to_read(Header::header_size());
        while((ret = header.Recv(fd))!=SUCCESS) {}

        if(header.datagram_type() < 0){
            err_sys("recv error");
        }

        epoll.AddFd(fd, EPOLLIN | EPOLLOUT | EPOLLHUP, new RelayClientAgent(fd, i , msg_size));
    }

    
    printf("connect and register successed: %d\n", talk_links);
    
    int cnt=0;
    clock_t start_total, end_total;
    start_total = std::clock();
    while(cnt < trans_times)
    {
        int ndfs = epoll.Wait(1024, 10);
        for(int i=0; i<ndfs; i++){
            /* std::cout<< cnt << std::endl; */
            int event = epoll.events[i].events;
            /* fprintf(stdout, "fd: %d, event: %s, %s, %s\n", fd, (event&EPOLLIN)?"EPOLLIN":"NOTIN", (event&EPOLLOUT)?"EPOLLOUT":"NOTOUT", (event&EPOLLHUP)?"EPOLLRDHUP": "NOEPOLLHUB"); */
            RelayClientAgent* agent = static_cast<RelayClientAgent*>(epoll.events[i].data.ptr);
            assert(agent);

            int from_id = agent->user_id();
            Header* header = dynamic_cast<Header*>(agent->header());
            Data* data = dynamic_cast<Data*>(agent->data());
            fd = agent->fd();

            if(event & EPOLLHUP){
                printf("%d close", fd);
                close(agent->fd());
                epoll.DeleteFd(agent->fd());
                delete agent;
                continue;

            }
            if(event & EPOLLIN){
                if(!agent->recv_header()){
                    if(header->left_to_read()==0)
                        header->set_left_to_read(Header::header_size());

                    ret = header->Recv(fd);
                    if(ret < 0)
                        err_sys("header recv error");
                    if(ret == SUCCESS){
                        if(header->datagram_type() < 0){
                            err_sys("recv error");
                        }
                        if(header->to_user_id()!=from_id)
                            err_sys("user error");
                        agent->set_recv_header(true);
                    }
                }
                if(agent->recv_header()&&!agent->recv_data()){
                    if(data->left_to_read() == 0)
                        data->set_left_to_read(header->byte_size());
                    ret = data->Recv(fd);
                    if(ret < 0)
                        err_sys("data recv error");
                    if(ret == SUCCESS){
                        agent->set_recv_data(true);
                        if((from_id & 1) == 0){
                            end[cnt++] = std::clock();
                            agent->Clear();
                        }
                    }
                }

            }
            if(event & EPOLLOUT){
                int to_id;

                if(from_id & 1){
                    to_id = from_id^1;
                    if(!agent->recv_header()) continue;
                    if(!agent->send_header()){
                        if(header->left_to_write() == 0)
                            header->set_left_to_write(Header::header_size());
                        header->set_my_user_id(from_id);
                        header->set_to_user_id(to_id);
                        ret = header->Send(fd);
                        if(ret < 0)
                            err_sys("header send error");
                        if(ret == SUCCESS)
                            agent->set_send_header(true);
                    }
                    if(agent->send_header()&&!agent->send_data()){

                        if(data->left_to_write() == 0)
                            data->set_left_to_write(header->byte_size());

                        ret = data->Send(fd);
                        if(ret < 0)
                            err_sys("data send error");
                        if(ret == SUCCESS){
                            agent->set_send_data(true);
                            agent->Clear();
                        }
                    }

                }
                else{
                    to_id = from_id^1;
                    if(!agent->send_header()){
                        if(header->left_to_write() == 0)
                            header->set_left_to_write(Header::header_size());

                        header->set_my_user_id(from_id);
                        header->set_to_user_id(to_id);
                        header->set_byte_size(msg_size);

                        start[cnt] = std::clock();
                        ret = header->Send(fd);
                        if(ret < 0)
                            err_sys("header send error");
                        if(ret == SUCCESS){
                            agent->set_send_header(true);
                        }
                    }
                    if(agent->send_header()&&!agent->send_data()){
                        if(data->left_to_write() == 0){
                            dynamic_cast<NormalBuffer*>(data->auto_buffer())->AddByte(msg_size);
                            data->set_left_to_write(msg_size);
                        }

                        ret = data->Send(fd);
                        if(ret < 0)
                            err_sys("data send error");
                        if(ret == SUCCESS){
                            agent->set_send_data(true);
                        }

                    }
                }

            }

        }
    }
    end_total = std::clock();

    printf("cnt: %d, times: %d\n", cnt, trans_times);
    /* assert(cnt==trans_times); */
    clock_t total_delay = 0;
    for(int i=0;i<cnt;i++){
        total_delay += end[i] - start[i];
    }
    /* std::cout << (end_total - start_total) << std::endl; */
    /* std::cout << (CLOCKS_PER_SEC) << std::endl; */
    printf("total time: %lfms, ", ((double)(end_total - start_total)/(CLOCKS_PER_SEC/1000)));
    printf("average delay time is %lfus\n", ((double)total_delay/cnt/(CLOCKS_PER_SEC/1000000)));
    return 0;
}

