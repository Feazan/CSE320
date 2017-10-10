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
        // After the add_new_page() function runs
        // The correct element stores a page of memory
        // Free header and free footer point to the correct positions
        void *mem_start = add_new_page();
        if (mem_start == NULL) {
            return NULL;
        }
    }

    int list_index = get_sf_free_index(size);
    for (int i = list_index; i < FREE_LIST_COUNT; i++)
    {
        if (seg_free_list[i].head != NULL)
        {
            printf("List %d is being traversed\n", i+1);
            if (seg_free_list[i].head->header.allocated == 0)
            {
                if (seg_free_list[i].head->header.block_size > size)
                {

                    int payload_pad_size = pad_size(size);
                    // When you write the lower 4 is 0 so we need to shift right 4
                    // when Reading shift left 4 to put the 4 bits back in
                    seg_free_list[i].head->header.block_size = (payload_pad_size + (SF_HEADER_SIZE/4)) >> 4;
                    seg_free_list[i].head->header.allocated = 1;
                    seg_free_list[i].head->header.padded = 1;
                    //sf_blockprint(&seg_free_list[i].head->header);

                    sf_footer *footer = (sf_footer *) ((char *)(&seg_free_list[i].head->header) + (payload_pad_size + (SF_HEADER_SIZE/8)));
                    footer->requested_size = size;
                    footer->block_size = seg_free_list[i].head->header.block_size;
                    footer->allocated = 1;
                    footer->padded = 1;
                    footer->two_zeroes = 0x00;


                    sf_snapshot();
                    sf_blockprint(&seg_free_list[i].head->header);
                }
            }
        }
    }

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

int pad_size(size_t size)
{
    while ((size % 16) != 0)
        size++;
    return size;
}

// The purpose of this method is to try and find which
// Index the requested memory size fits into
int get_sf_free_index(size_t size)
{
    for (int i = 0; i < FREE_LIST_COUNT; i++)
    {
        if (seg_free_list[i].max > (size + 16) || seg_free_list[i].max == -1)
        {
            return i;
        }
    }
    // Just in case
    return -1;
}


// Add new page to free list
void *add_new_page()
{
    void *heap_start = get_heap_start();
    void *heap_end = get_heap_end();
    int num_pages = 0;

    if(heap_start != NULL && heap_end != NULL)
    {
        num_pages = (heap_end - heap_start)/PAGE_SZ;
        printf("Number of pages: %d\n", num_pages);
    }

    // allocate new page
    sf_free_header *new_page = sf_sbrk();
    printf("%p\n", new_page);

    // handle case when new page cannot be obtained
    if (new_page == (void *) -1)
    {
        sf_errno = ENOMEM;
        return NULL;
    }

    // The value of the lists can change
    // Do a check before you place the value from heap into list
    // Put the value of the page into the
    // bucket where it will fit

    // try using page size
    int index_of_page = get_sf_free_index(PAGE_SZ);
    free_list *last_free_list = &seg_free_list[index_of_page];
    new_page->next = last_free_list->head;
    new_page->prev = NULL;
    new_page->header.allocated = 0;
    new_page->header.padded = 0;

    // Total block size for the page is page size minus space for next/prev pointers
    new_page->header.block_size = PAGE_SZ >> 4;

    sf_footer *free_footer = (sf_footer *) (((char *)(new_page)) +  PAGE_SZ - SF_HEADER_SIZE/8);
        //(sf_footer *) ((char *)(&seg_free_list[i].head->header)
    free_footer->requested_size = 0;
    free_footer->block_size = PAGE_SZ >> 4;
    free_footer->allocated = 0;
    free_footer->padded = 0;
    free_footer->two_zeroes = 0x00;


    if (last_free_list->head != NULL)
    {
        last_free_list->head->prev = new_page;
    }
    last_free_list->head = new_page;

    sf_snapshot();
    sf_blockprint(new_page);

    return last_free_list->head;
}