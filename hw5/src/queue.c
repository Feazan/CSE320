#include "queue.h"
#include <errno.h>

queue_t *create_queue(void)
{
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
    // Error case
    if (self == NULL || destroy_function == NULL)
    {
        errno = EINVAL;
        return false;
    }
    pthread_mutex_lock(&self->lock);
    self->invalid = true;
    queue_node_t *temp;
    while(self->front != NULL)
    {
        destroy_function(self->front->item);
        temp = self->front->next;
        free(self->front);
        self->front = temp;
    }

    self->rear = NULL;
    pthread_mutex_unlock(&self->lock);
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

void *dequeue(queue_t *self)
{
    // TODO: if all items were deleted
    if (self == NULL || self->invalid == true)
    {
        errno = EINVAL;
        return NULL;
    }

    queue_node_t *temp;
    queue_node_t *node_to_return;

    sem_wait(&self->items); // TODO: This may need to be on top
    pthread_mutex_lock(&self->lock);
    node_to_return = self->front;
    void *item = node_to_return->item;
    // set the item to be node to return
    temp = self->front->next;
    free(self->front);
    self->front = temp;
    pthread_mutex_unlock(&self->lock);

    // return the item
    return item;
}