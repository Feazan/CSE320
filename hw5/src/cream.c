#include "cream.h"
#include "utils.h"
#include "csapp.h"
#include "queue.h"

// create global variables for the map and the queue
queue_t *fd_queue;
hashmap_t *client_map;

void *thread(void *file_descriptor);

int main(int argc, char *argv[])
{
    int listenfd;
    //int *connfd = calloc(1, sizeof(int *));
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    char client_hostname[MAXLINE], client_port[MAXLINE];
    pthread_t tid;

    // error checking and help handle
    if (argc < 2)
    {
        // TODO: display help screen
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }

    // if the client asks for help
    if (argc >= 2 && strcmp(argv[1], "-h") == 0)
    {
        printf("%s\n", "Help Screen");
        exit(0);
    }

    fd_queue = create_queue();
    int num_threads = atoi(argv[1]);
    listenfd = Open_listenfd(argv[2]);

    // create worker threads
    for (int i = 0; i < num_threads; i++)
    {
        Pthread_create(&tid, NULL, thread, NULL);
    }

    while (1)
    {
        int *connfd = calloc(1, sizeof(int));
        printf("%s\n", "Waiting for connection request from client");
        clientlen = sizeof(struct sockaddr_storage);
        *connfd = Accept(listenfd, (SA *) &clientaddr, &clientlen);
        Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE,
            client_port, MAXLINE, 0);
        printf("Connected to (%s, %s)\n", client_hostname, client_port);
        // I think  this is where enqueue happens;
        // enqueue connfd
        printf("value of connfd in MAIN: %d\n", *connfd);
        enqueue(fd_queue, connfd);
    }
    exit(0);
}

void *thread(void *file_descriptor)
{
    //Pthread_detach(pthread_self());
    while (1)
    {
        // dequeue needs to happen here
        int *connfd = (int*) dequeue(fd_queue);
        response_header_t my_reponse;

        printf("value of connfd in WORKER THREAD: %d\n", *connfd);
        char buf[1000];
        read(*connfd, buf, sizeof(request_header_t));
        printf("%s\n", "inside of while loop");
        printf("request code: %X\n", buf[0]);

        my_reponse.response_code = 200;
        Rio_writen(*connfd, &my_reponse, sizeof(response_header_t));

        Close(*connfd);
        free (connfd);
    }
}