/**
 * All functions you make for the assignment must be implemented in this file.
 * Do not submit your assignment with a main function in this file.
 * If you submit with a main function in this file, you will get a zero.
 */
#include "sfmm.h"
#include "feazan_funk.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define RIGHT_SHIFT(size) (size >> 4)
#define LEFT_SHIFT(size) (size << 4)




/**
 * You should store the heads of your free lists in these variables.
 * Doing so will make it accessible via the extern statement in sfmm.h
 * which will allow you to pass the address to sf_snapshot in a different file.
 */
free_list seg_free_list[4] = {
    {NULL, LIST_1_MIN, LIST_1_MAX},
    {NULL, LIST_2_MIN, LIST_2_MAX},
    {NULL, LIST_3_MIN, LIST_3_MAX},
    {NULL, LIST_4_MIN, LIST_4_MAX}
};

int sf_errno = 0;

// Start here
void *sf_malloc(size_t size)
{
    sf_header *block_to_allocate;

    // Check if the size of the memory requested is within range
    if (size <= 0 || size > (PAGE_SZ * FREE_LIST_COUNT))
    {
        sf_errno = EINVAL;
        return NULL;
    }

    // If heap is not intialized allocate a new page
    // search free lists, if they have nothing availible
    // see if theres a block you can use
    // THEN call sf_sbrk
    if (get_heap_start() == NULL)
    {
        initialize();
    }

    // TODO: fix the hard coded 3 -- Need to find the best fit
    block_to_allocate = find_free_block(size, 3);


    sf_snapshot();
    sf_blockprint(block_to_allocate);


	return NULL;
}

void *sf_realloc(void *ptr, size_t size) {
	return NULL;
}

void sf_free(void *ptr)
{
    // Pointer is NULL
    // Header of the block is before heap start
    // Block ends after heap ends
    // The alloc bit in the header or footer is 0
    // The requested size, block size, padded bit are valid when put together

    // Padded and alloc bit in the header and footer are inconsistent

    // Call abort if any of these cases are true
    // Determine if next block in mem can be coalessed (sp?)
    //      If it can remove the next block from the free list and combine
    //      the two blocks

    //             Determine which list the newly freed block should be placed into

	return;
}

void initialize()
{
        // This pulls a page (4096 b) from memory and stores it in mem_start
        void *mem_start = sf_sbrk();
        // The page that is pulled from the heap is free memory
        sf_free_header *free_header = (sf_free_header *) mem_start;

        // Set the values of the header for initial setup
        free_header->header.allocated = 0;
        free_header->header.padded = 0;
        free_header->header.two_zeroes = 0x00;
        free_header->header.block_size = RIGHT_SHIFT(PAGE_SZ);
        free_header->next = NULL;
        free_header->prev = NULL;

        // Set the values of the footer for the initial setup
        sf_footer *free_footer = (sf_footer *) (mem_start + (PAGE_SZ - 8));
        free_footer->allocated = 0;
        free_footer->padded = 0;
        free_footer->two_zeroes = 0;
        free_footer->block_size = RIGHT_SHIFT(PAGE_SZ);
        free_footer->requested_size = 0;

        // TODO: fix the hard coded 3 -- Need to find the best fit
        seg_free_list[3].head = free_header;
}

// Might just pass the entire seg_free_list to this function
void *find_free_block(size_t size, int index)
{
    sf_header *free_block;

    // Search from index to LIST_SIZE -1
    for (int i = index; i < FREE_LIST_COUNT; i++)
    {
        // Traverse each node in the list
        // Check each block_size << 4 (Make this a define)
        // TODO: didnt shift by 4, fix that
        if (seg_free_list[i].head != NULL)
        {
            if (seg_free_list[i].head->header.block_size >= size)
            {
                // If >= then I have my block
                // set that free block
                free_block = &seg_free_list[i].head->header;
            }
        }
    }

    // Return that block
    return free_block;
}

void remove_block_from_list(sf_header *block_to_remove, int index)
{
    // After block is removed
    // Create a new header for the (free block - (allocated block size))
    //
}