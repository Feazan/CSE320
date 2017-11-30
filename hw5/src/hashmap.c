#include "utils.h"
#include <hashmap_helpers.h>


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
    // error cases
    if (self == NULL || key.key_len <= 0 || val.val_len <= 0)
    {
        errno = EINVAL;
        return false;
    }

    if (self->capacity == self->size && force == false)
    {
        errno = ENOMEM;
        return false;
    }

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
        // TODO: This needs to be locked correctly
        if ((duplicate_key_index = index_of_key(self, key)) != -1)
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
        // TODO: This doesn't cycle around
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

map_val_t get(hashmap_t *self, map_key_t key)
{
    // TODO: pthread_mutex_lock(&(self->fields_lock));
    // reader writer pattern in the book
    // ++self->num_readers;
    if (self == NULL || key.key_len <= 0)
    {
        errno = EINVAL;
        return MAP_VAL(NULL, 0);
    }

    map_val_t val_to_return;
    int key_index;

    // Location of val to find in hashmap
    pthread_mutex_lock(&(self->fields_lock));
    key_index = index_of_key(self, key);
    pthread_mutex_unlock(&(self->fields_lock));

    if (key_index < 0) // Key was not found in map
    {
        return MAP_VAL(NULL, 0);
    }
    else // Key was found in map
    {
        // Set the values
        pthread_mutex_lock(&(self->write_lock));
        val_to_return.val_base = self->nodes[key_index].val.val_base;
        val_to_return.val_len = self->nodes[key_index].val.val_len;
        pthread_mutex_unlock(&(self->write_lock));
    }
    return MAP_VAL(val_to_return.val_base, val_to_return.val_len);
}

map_node_t delete(hashmap_t *self, map_key_t key)
{
    // error cases
    if (self == NULL || key.key_len <= 0)
    {
        errno = EINVAL;
        return MAP_NODE(MAP_KEY(NULL, 0), MAP_VAL(NULL, 0), false);
    }

    map_node_t deleted_node;
    // Find index of the element to remove
    pthread_mutex_lock(&(self->fields_lock));
    int key_index = index_of_key(self, key);
    pthread_mutex_unlock(&(self->fields_lock));

    if (key_index < 0) // Key was not found in map
    {
        return MAP_NODE(MAP_KEY(NULL, 0), MAP_VAL(NULL, 0), false);
    }
    else
    {
        // Node that will be deleted
        pthread_mutex_lock(&(self->fields_lock));
        deleted_node.key.key_len = self->nodes[key_index].key.key_len;
        deleted_node.key.key_base = self->nodes[key_index].key.key_base;
        deleted_node.val.val_len = self->nodes[key_index].val.val_len;
        deleted_node.val.val_base = self->nodes[key_index].val.val_base;
        self->nodes[key_index].tombstone = true;
        self->size--;
        pthread_mutex_unlock(&(self->fields_lock));
    }

    return MAP_NODE(MAP_KEY(deleted_node.key.key_base, deleted_node.key.key_len)
        , MAP_VAL(deleted_node.val.val_base, deleted_node.val.val_len), false);
}

bool clear_map(hashmap_t *self)
{
    if (self == NULL || self->invalid == true)
    {
        errno = EINVAL;
        return false;
    }

    pthread_mutex_lock(&(self->write_lock));
    for (int i = 0; i < self->capacity; i++)
    {
        if (self->nodes[i].key.key_base != NULL)
        {
            self->destroy_function(self->nodes[i].key, self->nodes[i].val);
            self->size--;
            self->nodes[i].key.key_base = NULL;
            self->nodes[i].val.val_base = NULL;
        }
    }
    pthread_mutex_unlock(&(self->write_lock));
	return true;
}

bool invalidate_map(hashmap_t *self)
{
    if (self == NULL)
    {
        errno = EINVAL;
        return false;
    }

    pthread_mutex_lock(&(self->write_lock));
    for (int i = 0; i < self->capacity; i++)
    {
        if (self->nodes[i].key.key_base != NULL)
        {
            self->destroy_function(self->nodes[i].key, self->nodes[i].val);
            self->size--;
            self->nodes[i].key.key_base = NULL;
            self->nodes[i].val.val_base = NULL;
        }
    }
    free(self->nodes);
    self->invalid = true;
    pthread_mutex_unlock(&(self->write_lock));
    return true;
}

int index_of_key(hashmap_t *self, map_key_t key)
{
    // TODO: error checking
    int key_index = -1;
    for (int i = 0; i < self->capacity; i++)
    {
        if(self->nodes[i].key.key_len == key.key_len)
        {
            if (memcmp(self->nodes[i].key.key_base, key.key_base, key.key_len) == 0)
            {
                key_index = i;
                break;
            }
        }
    }

    return key_index;
}