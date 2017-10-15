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
static int num_pages = 0;
bool needed_pad = false;

// Start here
void *sf_malloc(size_t size)
{
    char *block_to_allocate = NULL;
    sf_free_header *free_header;
    // sf_header allocated_block;

    int padded_size = pad_size(size);

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
    if (num_pages == 0)
    {
        initialize();
    }

    int myIndex = get_sf_free_index(padded_size);
    while (block_to_allocate == NULL && num_pages < 4)
    {

        block_to_allocate = find_free_block(padded_size, myIndex);
    }

    // update free list
    // allocate

    // This is what needs to get added to the free list
    free_header = (sf_free_header *)split_block(block_to_allocate, padded_size);

    // Insert the free block into the list
    int add = get_sf_free_index(free_header->header.block_size << 4);
    add_to_free_list(free_header, add);

    // Now I need to return pointer to the block size

    //sf_snapshot();

    // Need block to allocate and block size passed
    // to function  that will add the header and footer to
    // the allocated block
    header_footer_for_allocated_block(block_to_allocate, padded_size);

	return block_to_allocate + 8;
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

// Function takes care of padding for size
int pad_size(size_t size)
{
    needed_pad = needed_paddding(size);

    while ((size % 16) != 0)
        size++;
    return size;
}

bool needed_paddding(size_t size)
{
    if(size % 16 == 0)
        return false;
    else
        return true;
}

// The purpose of this method is to try and find which
// Index the requested memory size fits into
int get_sf_free_index(size_t size)
{
    int index = 3;
    for (int i = 0; i < FREE_LIST_COUNT; i++)
    {
        if (seg_free_list[i].max > (size + 16) && has_availible_block(seg_free_list[i].head, size))
        {
            index = i;
            break;
        }
    }
    return index;
}

bool has_availible_block(sf_free_header *head, size_t size)
{
    sf_free_header *list_entry = head;
    while (list_entry != NULL)
    {
        size_t size_to_check = list_entry->header.block_size << 4;
        if (size_to_check >= size)
            return true;
        list_entry = list_entry->next;
    }

    return false;
}

void initialize()
{
        // This pulls a page (4096 b) from memory and stores it in mem_start
        void *mem_start = sf_sbrk();
        num_pages++;
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

        int where_to_place_first = get_sf_free_index(PAGE_SZ);
        seg_free_list[where_to_place_first].head = free_header;
}

// Might just pass the entire seg_free_list to this function
void *find_free_block(size_t size, int index)
{
    sf_free_header *free_block;

    // Search from index to LIST_SIZE -1
    for (int i = index; i < FREE_LIST_COUNT; i++)
    {
        // Traverse each node in the list
        // Check each block_size << 4 (Make this a define)
        free_block = seg_free_list[i].head;
        while (free_block != NULL)
        {
            if ((free_block->header.block_size << 4) >= size)
            {
                // If >= then I have my block
                // set that free block
                remove_block_from_list(free_block, index);
                break;
            }
            else
            {
                // What happens when I reach here and the
                // next is null
                // I need a new page
                // Function for getting more memory
                char *new_page = give_more_memory();
                // Initialize the new page
                initialize_new_page(new_page);
                // Call coalesce
                free_block = coalesce(new_page);
            }
        }


        // PAGE == 4 ERROR (Just in case)
        if (num_pages == 5)
        {
            sf_errno = ENOMEM;
            return NULL;
        }
    }

    // Return that block
    return free_block;
}

void add_to_top(int index, sf_header *node_to_add)
{
    sf_free_header **head = &seg_free_list[index].head;
    sf_free_header *current = (sf_free_header *)node_to_add;
    current->prev = NULL;

    current->next = *head;

    if(*head != NULL)
    {
        (*head)->prev = current;
    }

    (*head) = current;
}

void remove_block_from_list(sf_free_header *block_to_remove, int index)
{
    sf_free_header **head = &seg_free_list[index].head;

    // Node to remove is the head node
    if (*head == block_to_remove)
    {
        *head = block_to_remove->next;
    }
    // Node to remove is not the last node
    if (block_to_remove->next != NULL)
    {
        block_to_remove->next->prev = block_to_remove->prev;
    }
    if (block_to_remove->prev != NULL)
    {
        block_to_remove->prev->next = block_to_remove->next;
    }
}

void *split_block(char *block_to_split, size_t size)
{
    // old_block_size should be the allocated size
    // Add 16 to account for the header and footer
    size += 16;
    sf_free_header *free_header = (sf_free_header *)block_to_split;
    int old_block_size = free_header->header.block_size;
    int new_block_size = (old_block_size << 4) - size;

    // Create the new header
    sf_free_header *updated_header = (sf_free_header *) (block_to_split + size);
    updated_header->header.allocated = 0;
    updated_header->header.padded = 0;
    updated_header->header.two_zeroes = 0x00;
    updated_header->header.block_size = new_block_size >> 4;


    // Update the footer
    sf_footer *updated_footer = (sf_footer *)((block_to_split - 8) + (old_block_size << 4));
    updated_footer->allocated = 0;
    updated_footer->padded = 0;
    updated_footer->two_zeroes = 0x00;
    updated_footer->block_size = new_block_size >> 4;
    updated_footer->requested_size = 0;

    // Insert the new header into the block
    // Store total block size
    return updated_header;
}

void add_to_free_list(sf_free_header *header_to_insert, int index)
{
    while(seg_free_list[index].head != NULL)
    {
        index++;
    }
    seg_free_list[index].head = header_to_insert;
}

void header_footer_for_allocated_block(char *allocated_block, size_t block_size)
{
    // Maybe I can pass size here and do size += 16;
    block_size += 16;
    // allocated_header is the reference to the header that needs to be set
    sf_free_header *allocated_header = (sf_free_header *)allocated_block;
    allocated_header->header.allocated = 1;
    if (needed_pad == true)
    {
        allocated_header->header.padded = 1;
    }
    else
    {
        allocated_header->header.padded = 0;
    }
    allocated_header->header.two_zeroes = 0x00;
    allocated_header->header.block_size = block_size >> 4;

    // Need to set the allocated footer
    sf_footer *allocated_footer = (sf_footer *)((allocated_block - 8) + (block_size));
    allocated_footer->allocated = 1;
    if (needed_pad == true)
    {
        allocated_footer->padded = 1;
    }
    else
    {
        allocated_footer->padded = 0;
    }
    allocated_footer->two_zeroes = 0x00;
    allocated_footer->block_size = block_size >> 4;
    allocated_footer->requested_size = 0;
}


bool ask_for_more_memory(sf_free_header *free_block)
{
    if (num_pages < 4 && free_block == NULL)
        return true;
    else
        return false;
}

void *give_more_memory()
{
    // local variables
    void *heap_start = get_heap_start();
    void *heap_end = get_heap_end();

    if(heap_start != NULL && heap_end != NULL)
    {
        num_pages = (heap_end - heap_start)/PAGE_SZ;
        //printf("Number of pages BEFORE sf_sbrk: %d\n", num_pages);
    }

    // Get the new page of memory
    void *new_page = sf_sbrk();
    num_pages++;

    //printf("Number of pages AFTER sf_sbrk: %d\n", num_pages);

    return new_page;
}

void *coalesce(void *new_mem_start)
{
    // Add a header and footer to the newly added page

    // check previous footer

    sf_footer *prev_footer = (sf_footer *)((char *)new_mem_start - 8);
    size_t prev_block_size = (prev_footer->block_size) << 4;
    sf_header *prev_header = (sf_header *)((char *)prev_footer - prev_block_size + 8);

    sf_header *new_mem_header = (sf_header *)new_mem_start;
    size_t new_mem_size = new_mem_header->block_size << 4;
    sf_footer *new_mem_footer = (sf_footer *) ((char *)new_mem_header + (new_mem_size - 8));

    int coalesce_index = get_sf_free_index(new_mem_size);
    //sf_snapshot();

    add_to_top(coalesce_index, new_mem_header);

    //sf_snapshot();

    if (prev_footer->allocated == 0)
    {
        int index1 = get_sf_free_index(prev_block_size);
        int index2 = get_sf_free_index(new_mem_header->block_size << 4);
        // This might need to be a - 8 instead of a + 8
        //sf_snapshot();

        remove_block_from_list((sf_free_header *)prev_header, index1);
        remove_block_from_list((sf_free_header *)new_mem_header, index2);

        //sf_snapshot();


        // Update the header [correct pointer arithmatic?]
        prev_header = (sf_header *)((char *)prev_footer - prev_block_size + 8);
        prev_header->allocated = 0;
        prev_header->padded = 0;
        prev_header->two_zeroes = 0x00;
        prev_header->block_size = (prev_block_size + PAGE_SZ) >> 4;

        // Update the footer
        new_mem_footer->allocated = 0;
        new_mem_footer->padded = 0;
        new_mem_footer->two_zeroes = 0x00;
        new_mem_footer->block_size = (prev_block_size + PAGE_SZ) >> 4;
        new_mem_footer->requested_size = 0;

        int index3 = get_sf_free_index(prev_header->block_size << 4);
        add_to_top(index3, prev_header);

        //sf_blockprint(prev_header);
       // sf_snapshot();
    }

    return (void *)prev_header;

}

void initialize_new_page(void *new_mem_start)
{
    // The page that is pulled from the heap is free memory
    sf_header *free_header = (sf_header *) new_mem_start;

    // Set the values of the header for initial setup
    // Do not need new header new page is my entry page
    free_header->allocated = 0;
    free_header->padded = 0;
    free_header->two_zeroes = 0x00;
    free_header->block_size = RIGHT_SHIFT(PAGE_SZ);

    // Set the values of the footer for the initial setup
    sf_footer *free_footer = (sf_footer *) (new_mem_start + (PAGE_SZ - 8));
    free_footer->allocated = 0;
    free_footer->padded = 0;
    free_footer->two_zeroes = 0;
    free_footer->block_size = RIGHT_SHIFT(PAGE_SZ);
    free_footer->requested_size = 0;
}
