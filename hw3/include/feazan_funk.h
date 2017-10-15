int pad_size(size_t size);
int index_to_place(size_t size);
void *add_new_page();

void initialize();
void *find_free_block(size_t size, int index);
void remove_block_from_list();
void *split_block(char *block_to_split, size_t size);
void add_to_free_list(sf_free_header *header_to_insert, int index);
void header_footer_for_allocated_block(char *allocated_block, size_t block_size);
bool ask_for_more_memory(sf_free_header *free_block);
void *give_more_memory();
void *coalesce(void *new_mem_start);
void initialize_new_page(void *mem_start);
void add_to_top(int index, sf_header *node_to_add);
bool has_availible_block(sf_free_header *head, size_t size);
int get_sf_free_index(size_t size);
bool needed_paddding(size_t size);