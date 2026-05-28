#ifndef SAGE_DARRAY_H
#define SAGE_DARRAY_H

#include <stddef.h>

#define DARRAY_RESIZE_FACTOR 2

typedef struct darray {
    size_t len;
    size_t capacity;
    size_t item_size;
    void *items;
} darray;

darray *darray_alloc(size_t n_size, size_t initial_size);

/* When pushing an item, the reason for passing the memory address is not to
   store the reference to that item, but to copy the bytes at tat location
   over to the darray. (You can store items from the stack over to the darray) */
size_t darray_push(darray *arr, void *item);
void *darray_pop_at(darray *arr,  size_t index);
void *darray_pop(darray *arr);
void *darray_at(darray *arr, size_t index);
void darray_free(darray *arr);

#endif /* SAGE_DARRAY_H */
