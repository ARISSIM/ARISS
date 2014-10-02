#include "Vector.h"

void vector_init(vector* v, size_t init_elem_size, size_t init_capacity) {
    v->elem_size = init_elem_size;
    v->capacity = init_capacity;
    v->data = calloc(v->capacity, v->elem_size); //Allocates memory for an array of v->capacity elements of v->elem_size bytes each, the memory is set to 0.
    v->size = 0;
}

/*
void vector_resize(vector* v, size_t new_size) {
    v->capacity = new_size;
    realloc(v->data, v->capacity * v->elem_size); //Changes the size of the memory block pointed by v->data to (v->capacity * v->elem_size) bytes
}
*/

void vector_push_back(vector* v, void* element) {
    v->size ++;
    if (v->size > v->capacity) {
        void* tmp;
        memcpy(tmp, v->data, v->elem_size * v->capacity);//Copy the elements ((v->elem_size * v->capacity) bytes) from v->data to tmp
        v->data = calloc(v->capacity*2, v->elem_size);//Allocate the memory and set it to 0.
        memcpy(tmp, v->data, v->elem_size * v->capacity);
        v->capacity = v->capacity*2;//Modify the capacity
    }
    void* target = v->data + v->elem_size * (v->size-1);
    memcpy(target, element, v->elem_size);
}

size_t vector_size(vector* v) {
    return v->size;
}

void vector_clear(vector* v) {
    memset(v->data, 0, v->capacity * v->elem_size); //fills  the  first  (v->capacity*v->elem_size)  bytes of the memory area pointed to by v->data with the constant byte 0.
}


void vector_get(vector* v, size_t index, void *elem) {
    void* target = v->data + v->elem_size * index;
    memcpy(elem, target, v->elem_size);
}