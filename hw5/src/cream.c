#include "cream.h"
#include "utils.h"
#include "csapp.h"

void echo(int connfd);

int main(int argc, char *argv[])
{
    request_header_t my_request;
    response_header_t my_reponse;

    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    char client_hostname[MAXLINE], client_port[MAXLINE];
    char buf[MAXLINE];

    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }

    listenfd = Open_listenfd(argv[1]);
    while(1)
    {
        printf("%s\n", "Waiting for connection request from client");
        clientlen = sizeof(struct sockaddr_storage);
        connfd = Accept(listenfd, (SA *)&clientaddr, & clientlen);
        Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE,
            client_port, MAXLINE, 0);

        printf("Connected to (%s, %s)\n", client_hostname, client_port);
        printf("The code is: %d\n", my_request.request_code);
        printf("Value of connfd is: %d\n", connfd);

        read(connfd, buf, sizeof(request_header_t));
        printf("request code: %X\n", buf[0]);

        my_reponse.response_code = 200;
        Rio_writen(connfd, &my_reponse, sizeof(response_header_t));
        echo(connfd);
        Close(connfd);
    }

    exit(0);
}

void echo(int connfd)
{
    size_t n;
    char buf[MAXLINE];
    rio_t rio;

    Rio_readinitb(&rio, connfd);
    while((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0)
    {
        printf("server recieved %d bytes\n",  (int)n);
        Rio_writen(connfd, buf, n);
    }
}
