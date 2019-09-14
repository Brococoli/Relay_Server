#include <iostream>
#include "../include/user.h"
#include "../include/header.h"
#include "../include/data.h"
#include "../include/socket_utils.h"
#include <string.h>

void str_cli(FILE* fp, int fd){
    Header header;
    Data data;
    char sendline[2000]= {};

    
    /* int my_id, to_id, datagram_type, data_size = 0, reserved_byte_; */
    for(;;){

        printf("data_size, my_id, to_id, datagram_size, reserved_byte_:");
        fflush(stdout); 
    
        /* scanf("%s", sendline); */
        gets(sendline);
        /* header.Recv(STDIN_FILENO); */
        header.ToDatagram(&header, sendline, sizeof(sendline));
        /* sscanf(sendline, "%d %d %d %d %d", &data_size, &my_id, &to_id, &datagram_type, &reserved_byte_); */
        header.Send(fd);

        sleep(1);
        header.Recv(fd);
        header.Send(STDOUT_FILENO);

        if(header.byte_size()&&header.datagram_type()>=0){
            scanf("%s", sendline);
            write(fd, sendline, header.byte_size());
            sleep(1);
            data.Recv(fd);
            printf("data:");
            fflush(stdout);
            data.Send(STDOUT_FILENO);
        }
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

