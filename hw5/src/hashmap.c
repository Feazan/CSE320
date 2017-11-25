#include "utils.h"
#include <hashmap_helpers.h>


#define MAP_KEY(base, len) (map_key_t) {.key_base = base, .key_len = len}
#define MAP_VAL(base, len) (map_val_t) {.val_base = base, .val_len = len}
#define MAP_NODE(key_arg, val_arg, tombstone_arg) (map_node_t) {.key = key_arg, .val = val_arg, .tombstone = tombstone_arg}



hashmap_t *create_map(uint32_t capacity, hash_func_f hash_function, destructor_f destroy_function)
{
    // TODO: Does this need to by MT-Safe?
    if (capacity < 1
        || hash_function == NULL
        || destroy_function == NULL)
    {
        errno = EINVAL;
        return NULL;
    }

    hashmap_t *my_hashmap = calloc(1, sizeof(hashmap_t));
    // TODO: Correct way to calloc? does the calloc for the nodes need to be in a loop?
    // Where to free this calloc?
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

bool put(hashmap_t *self, map_key_t key, map_val_t val, bool force)
{
    // Index is the hashed index
    int index = get_index(self, key);
    int duplicate_key_index;

    // force entry into hashmap, if full
    if (force == true && self->capacity == self->size)
    {
        pthread_mutex_lock(&(self->write_lock));
        self->nodes[index].key = key;
        self->nodes[index].val = val;
        pthread_mutex_unlock(&(self->write_lock));
        return true;
    }

    // Simplest case -- Check if index is occupied
    if (self->nodes[index].key.key_base == NULL)
    {
        pthread_mutex_lock(&(self->write_lock));
        self->nodes[index].key = key;
        self->nodes[index].val = val;
        self->size++;
        pthread_mutex_unlock(&(self->write_lock));
        return true;
    }
    else // Means that the index is occupied -- COLLISION
    {
        // Check if a duplicate key was found
        if ((duplicate_key_index = indexof_duplicate_key(self, key)) != -1)
        {
            // Key was found, index needs updating
            pthread_mutex_lock(&(self->write_lock));
            self->nodes[duplicate_key_index].key = key;
            self->nodes[duplicate_key_index].val = val;
            pthread_mutex_unlock(&(self->write_lock));
            return true;
        }
        // A duplicate key was not found
        int capacity = self->capacity;
        while(index <= capacity)
        {
            if (index > (self->capacity - 1))
            {
                index = 0;
                // TODO: Do I need this line?
                capacity = get_index(self, key);
            }

            if (self->nodes[index].key.key_base == NULL
                || self->nodes[index].tombstone == true)
            {
                pthread_mutex_lock(&(self->write_lock));
                self->nodes[index].key = key;
                self->nodes[index].val = val;
                self->nodes[index].tombstone = false;
                self->size++;
                pthread_mutex_unlock(&(self->write_lock));
                return true;
            }
            index++;
        }
    }
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

int indexof_duplicate_key(hashmap_t *self, map_key_t key)
{
    // TODO: error checking
    int index_to_update = -1;
    for (int i = 0; i < self->capacity; i++)
    {
        if(self->nodes[i].key.key_len == key.key_len)
        {
            if (memcmp(self->nodes[i].key.key_base, key.key_base, key.key_len) == 0)
            {
                index_to_update = i;
                break;
            }
        }
    }

    return index_to_update;
}