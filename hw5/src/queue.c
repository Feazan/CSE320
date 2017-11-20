#include "queue.h"
#include <errno.h>

queue_t *create_queue(void)
{
    // TODO: How many should there be in the beginning? -- Assume 1
    queue_t *my_queue = calloc(1, sizeof(queue_t));

    if (my_queue == NULL
        || pthread_mutex_init(&my_queue->lock, 0) != 0
        || sem_init(&my_queue->items, 0, 0) != 0)
    {
        return NULL;
    }

    return my_queue;
}

bool invalidate_queue(queue_t *self, item_destructor_f destroy_function)
{
    // Error case -- TODO: under what condition is destroy function invalid?
    if (self == NULL || destroy_function == NULL)
    {
        errno = EINVAL;
        return false;
    }

    self->invalid = true;
    queue_node_t *temp;
    while(self->front != NULL)
    {
        destroy_function(self->front->item);
        temp = self->front->next;
        free(self->front);
        self->front = temp;
    }

    // TODO: Does this need to be NULL?
    self->rear = NULL;
    // If self equals NULL, or destroy_function is invalid
    return true;
}

bool enqueue(queue_t *self, void *item)
{
    if (self == NULL || self->invalid == true || item == NULL)
    {
        errno = EINVAL;
        return false;
    }

    queue_node_t *node_to_insert;
    node_to_insert = calloc(1, sizeof(queue_node_t));
    node_to_insert->item = item;

    pthread_mutex_lock(&self->lock);
    // Insert to queue
    if (self->front == NULL)
    {
        self->front = node_to_insert;
        self->rear = node_to_insert;
    }
    else
    {
        self->rear->next = node_to_insert;
        self->rear = node_to_insert;
    }
    sem_post(&self->items);
    pthread_mutex_unlock(&self->lock);

    return true;
}

void *dequeue(queue_t *self) {
    return NULL;
}
