#include "utils.h"
#include <errno.h>

#define MAP_KEY(base, len) (map_key_t) {.key_base = base, .key_len = len}
#define MAP_VAL(base, len) (map_val_t) {.val_base = base, .val_len = len}
#define MAP_NODE(key_arg, val_arg, tombstone_arg) (map_node_t) {.key = key_arg, .val = val_arg, .tombstone = tombstone_arg}

hashmap_t *create_map(uint32_t capacity, hash_func_f hash_function, destructor_f destroy_function)
{
    if (capacity < 1
        || hash_function == NULL
        || destroy_function == NULL)
    {
        errno = EINVAL;
        return NULL;
    }

    hashmap_t *my_hashmap = calloc(1, sizeof(hashmap_t));
    // TODO: Correct way to calloc? does the calloc for the nodes need to be in a loop?
    my_hashmap->nodes = calloc(capacity, sizeof(map_node_t));

    if (pthread_mutex_init(&my_hashmap->write_lock, 0) != 0
        || pthread_mutex_init(&my_hashmap->fields_lock, 0) != 0)
    {
        return NULL;
    }

    my_hashmap->capacity = capacity;
    my_hashmap->size = 0;
    my_hashmap->hash_function = hash_function;
    my_hashmap->destroy_function = destroy_function;

    return my_hashmap;
}

bool put(hashmap_t *self, map_key_t key, map_val_t val, bool force) {
    return false;
}

map_val_t get(hashmap_t *self, map_key_t key) {
    return MAP_VAL(NULL, 0);
}

map_node_t delete(hashmap_t *self, map_key_t key) {
    return MAP_NODE(MAP_KEY(NULL, 0), MAP_VAL(NULL, 0), false);
}

bool clear_map(hashmap_t *self) {
	return false;
}

bool invalidate_map(hashmap_t *self) {
    return false;
}
