
#include <stdio.h>
#include "unp/unp.h"

void str_cli(FILE* fp, int sockfd){
    char sendline[MAXLINE];
    fd_set rset;
    FD_ZERO(&rset);

    int stdineof = 0;
    int n;
    int my_id, to_id, header, data_size = 0, reser;
    if(data_size == 0) {
        printf("data_size, my_id, to_id, datagram_size, reserved_byte_:");
        fflush(stdout); 
    }
    for(;;){
        if(stdineof == 0)
            FD_SET(fileno(fp), &rset);
        FD_SET(sockfd, &rset);

        select(max(fileno(fp), sockfd) + 1, &rset, NULL, NULL, NULL);

        if(FD_ISSET(fileno(fp), &rset)){
            if((n = Read(fileno(fp), sendline, sizeof(sendline))) ==  0){
                stdineof = 1;
                FD_CLR(fileno(fp), &rset);  // I forgot it !;
                Shutdown(sockfd, SHUT_WR);
                continue;
            }
            if(data_size == 0){
                Writen(sockfd, sendline, 55);
                sscanf(sendline, "%d %d %d %d %d", &data_size, &my_id, &to_id, &header, &reser);

            }
            else{
                Writen(sockfd, sendline, data_size);
                data_size = 0;
            }
            bzero(sendline, sizeof(sendline));
        }

        if(FD_ISSET(sockfd, &rset)){
            if((n = Read(sockfd, sendline, sizeof(sendline))) == 0){
                if(stdineof == 1)
                    return;
                else
                    err_quit("str_cli: server terminated prematurely");
            }
            /* Fputs(sendline, stdout); */
            if(data_size == 0)
                sscanf(sendline, "%d %d %d %d %d", &data_size, &my_id, &to_id, &header, &reser);
            printf("recv:");
            fflush(stdout);
            Write(fileno(stdout), sendline, n);
            if(data_size == 0) {
                printf("data_size, my_id, to_id, datagram_size, reserved_byte_:");
                fflush(stdout); 
            }
            else {
                printf("data:");
                fflush(stdout);
            }
        }

    }
}
int main(int argc, char **argv)
{

    int sockfd;
    struct sockaddr_in servaddr;

    if(argc != 2)
        err_quit("usage: tcpcli <IPaddress>");

    sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(9888);
    Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    Connect(sockfd, (SA*) &servaddr, sizeof(servaddr));
    
    str_cli(stdin, sockfd);
    return 0;
}

