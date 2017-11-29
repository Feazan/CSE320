#include <criterion/criterion.h>
#include <criterion/logging.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

#include "queue.h"
#define NUM_THREADS 2500

queue_t *global_queue;

/* Used in item destruction */
void queue_free_function(void *item) {
    free(item);
}

void queue_init(void) {
    global_queue = create_queue();
}

void *thread_enqueue(void *arg) {
    enqueue(global_queue, arg);
    return NULL;
}

void *thread_dequeue(void *arg) {
    dequeue(global_queue);
    return NULL;
}

void queue_fini(void) {
    invalidate_queue(global_queue, queue_free_function);
}


// Test if queue was initialized correctly
Test(queue_suite, 00_creation, .timeout = 2, .init = queue_init, .fini = queue_fini){
    cr_assert_not_null(global_queue, "Queue returned was null");
}


// Tests if items were added to the queue correctly
Test(queue_suite, 01_multithreaded, .timeout = 2, .init = queue_init, .fini = queue_fini) {
    pthread_t thread_ids[NUM_THREADS];

    // spawn NUM_THREADS threads to enqueue elements
    for(int index = 0; index < NUM_THREADS; index++) {
        int *ptr = malloc(sizeof(int));
        *ptr = index;

        if(pthread_create(&thread_ids[index], NULL, thread_enqueue, ptr) != 0)
            exit(EXIT_FAILURE);
    }

    // wait for threads to die before checking queue
    for(int index = 0; index < NUM_THREADS; index++) {
        pthread_join(thread_ids[index], NULL);
    }

    // get number of items in queue
    int num_items;
    if(sem_getvalue(&global_queue->items, &num_items) != 0)
        exit(EXIT_FAILURE);

    cr_assert_eq(num_items, NUM_THREADS, "Had %d items. Expected: %d", num_items, NUM_THREADS);
    cr_assert_not_null(global_queue->front, "After enqueue has been called the front should not be null");
    cr_assert_not_null(global_queue->rear, "After enqueue has been called the rear should not be null");
}


Test(queue_suite, 02_invalidate, .timeout = 2, .init = queue_init, .fini = queue_fini) {
    invalidate_queue(global_queue, queue_free_function);

    int num_items = 0;
    sem_getvalue(&global_queue->items, &num_items);

    cr_assert_eq(global_queue->invalid,true, "The queue has not been invalidated correctly");
    cr_assert_eq(num_items,0, "There should be 0 items in the queue and we detected: %d",num_items);
    cr_assert_null(global_queue->front, "After invalidate_queue has been called the front should be null");
    cr_assert_null(global_queue->rear, "After invalidate_queue has been called the rear should be null");
}

Test(queue_suite, 03_dequeue, .timeout = 2, .init = queue_init, .fini = queue_fini) {
    pthread_t thread_ids[NUM_THREADS];

    // spawn NUM_THREADS threads to enqueue elements
    for(int index = 0; index < NUM_THREADS; index++) {
        int *ptr = malloc(sizeof(int));
        *ptr = index;

        if(pthread_create(&thread_ids[index], NULL, thread_enqueue, ptr) != 0)
            exit(EXIT_FAILURE);
    }

    // wait for threads to die before checking queue
    for(int index = 0; index < NUM_THREADS; index++) {
        // Will hang here sometimes, deadlock?
        pthread_join(thread_ids[index], NULL);
    }


    // spawn NUM_THREADS threads to enqueue elements
    for(int index = 0; index < NUM_THREADS; index++) {
        int *ptr = malloc(sizeof(int));
        *ptr = index;

        if(pthread_create(&thread_ids[index], NULL, thread_dequeue, ptr) != 0)
            exit(EXIT_FAILURE);
    }

    // wait for threads to die before checking queue
    for(int index = 0; index < NUM_THREADS; index++) {
        // Will hang here sometimes, deadlock?
        pthread_join(thread_ids[index], NULL);
    }

    int num_items = 0;
    sem_getvalue(&global_queue->items, &num_items);
    cr_assert_eq(num_items,0, "Dequeue loop did not remove all the nodes");
    cr_assert_null(global_queue->front, "After all elements have been dequeued front should be null");
    cr_assert_null(global_queue->rear, "After all elements have been dequeued rear should be null");
}