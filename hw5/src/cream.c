#include "cream.h"
#include "utils.h"
#include "csapp.h"
#include "queue.h"

// create global variables for the map and the queue
queue_t *fd_queue;
hashmap_t *client_map;

void *thread(void *file_descriptor);
void destroy(map_key_t key, map_val_t val);

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
    // TODO: is this how to pass the hash function?
    client_map = create_map(atoi(argv[3]), jenkins_one_at_a_time_hash, destroy);
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
        request_header_t my_request;
        response_header_t my_reponse;

        char buf[MAXLINE];
        read(*connfd, &buf, sizeof(request_header_t));

        memcpy(&my_request, buf, sizeof(request_header_t));
        //store the request code
        uint8_t request_code = my_request.request_code;
        //store the key size
        //store the value size
        //store key
        read(*connfd, &buf, my_request.key_size);

        char *key = malloc(sizeof(my_request.key_size) + 1);
        memcpy(key, buf, sizeof(my_request.key_size));
        *(key + my_request.key_size) = '\0';

        printf("The key was: %s\n", key);

        // store value
        read(*connfd, &buf, my_request.value_size);
        char *value = malloc(sizeof(my_request.value_size) + 1);
        memcpy(value, buf, sizeof(my_request.value_size));
        *(value + my_request.value_size ) = '\0';

        printf("The value was: %s\n", value);

        uint32_t my_key_size = my_request.key_size;
        uint32_t my_val_size = my_request.value_size;
        // setting up for hashmap
        map_key_t my_key;
        my_key.key_base = (void *)key;
        my_key.key_len = my_key_size;

        map_val_t my_val;
        my_val.val_base = (void *)value;
        my_val.val_len = my_val_size;


        // deal with request
        if(request_code == PUT)
        {
            // Make sure to hash put!!
            printf("%s\n", "PUT");
            put(client_map, my_key, my_val, true);
            my_reponse.response_code = 0xc8;
            Rio_writen(*connfd, &my_reponse, sizeof(response_header_t));
        }
        else if(request_code == GET)
        {
            printf("%s\n", "GET");
            map_val_t value_returned =  get(client_map, my_key);
            my_reponse.value_size = value_returned.val_len;
            printf("returned value: %s\n", (char *)value_returned.val_base);
            if ((char *)value_returned.val_base != NULL)
            {
                my_reponse.response_code = 0xc8;
                Rio_writen(*connfd, &my_reponse, sizeof(response_header_t));
                Rio_writen(*connfd, value_returned.val_base, sizeof(value_returned.val_len));
            }
            else
            {
                my_reponse.response_code = 404;
                Rio_writen(*connfd, &my_reponse, sizeof(response_header_t));
            }

        }
        else if(request_code == EVICT)
        {
            printf("%s\n", "EVICT");
            map_node_t node_returned = delete(client_map, my_key);
            my_reponse.value_size = node_returned.val.val_len;
            my_reponse.response_code = 0xc8;
            printf("deleted value: %s\n", (char *)node_returned.val.val_base);
            Rio_writen(*connfd, &my_reponse, sizeof(response_header_t));
            Rio_writen(*connfd, node_returned.val.val_base, sizeof(my_reponse.value_size));

        }
        else if(request_code == CLEAR)
        {
            printf("%s\n", "CLEAR");
        }
        // respond




        //my_reponse.response_code = 200;
        //Rio_writen(*connfd, &my_reponse, sizeof(response_header_t));

        Close(*connfd);
        free (connfd);
    }
}

// Copied destroy function from test files
void destroy(map_key_t key, map_val_t val)
{
    free(key.key_base);
    free(val.val_base);
}