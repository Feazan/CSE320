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
    // Error case
    if (self == NULL)
    {
        errno = EINVAL;
        return false;
    }

    self->invalid = true;
    queue_node_t *temp;
    while(self->front != NULL)
    {
        destroy_function(&self->front->item);
        temp = self->front->next;
        free(self->front);
        self->front = temp;
    }

    self->rear = NULL;
    // If self equals NULL, or destroy_function is invalid
    return true;
}

bool enqueue(queue_t *self, void *item) {
    return false;
}

void *dequeue(queue_t *self) {
    return NULL;
}
