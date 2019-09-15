#include <iostream>
#include "../include/user.h"
#include "../include/header.h"
#include "../include/data.h"
#include "../include/socket_utils.h"
#include <string.h>

void str_cli(FILE* fp, int fd){
    /* int my_id, to_id, datagram_type, data_size = 0, reserved_byte_; */

    Header header;
    Data data;
    pid_t pid;
    int ret;
    int to_id, my_id, data_size;
    
    if((pid = fork()) == 0){
        for(;;){
            header.set_left_to_read(Header::header_size());
            while( (ret = header.Recv(fd)) != SUCCESS) {}
            printf("recv from: %d, datagram type: %d\n", header.my_user_id(), header.datagram_type());
            data.set_left_to_read(header.byte_size());
            while( (ret = data.Recv(fd)) != SUCCESS) {}
            printf("recv from: %d, data: ", header.my_user_id());
            fflush(stdout);
            data.Send(STDOUT_FILENO);
        }

    }

    for(;;){

        sleep(1);
        printf("my_id, to_id, data_size: ");
        fflush(stdout);
        scanf("%d %d %d", &my_id, &to_id, &data_size);
        header.set_my_user_id(my_id);
        header.set_to_user_id(to_id);
        header.set_datagram_type(0);
        header.set_byte_size(data_size);

        header.set_left_to_write(Header::header_size());
        while( (ret = header.Send(fd)) != SUCCESS) {}
        printf("Header Send Success\n");

        sleep(1);
        printf("print data:");
        fflush(stdout);
        data.set_left_to_read(data_size);
        data.Recv(STDIN_FILENO);
        data.set_left_to_write(header.byte_size());
        while( (ret = data.Send(fd)) != SUCCESS) {}
        printf("Data Send Success\n");

    }

}

int main(int argc, char * argv[])
{
    int sockfd;
    struct sockaddr_in servaddr;

    if(argc != 2)
        err_quit("usage: tcpcli <IPaddress>");

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(9888);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    connect(sockfd, (SA*) &servaddr, sizeof(servaddr));
    str_cli(stdin, sockfd);
    return 0;
}

