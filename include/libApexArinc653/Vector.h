#ifndef VECTOR_H_
#define VECTOR_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

typedef struct dynamic_array_struct
{
  void* data;
  size_t elem_size; //Memory size of one element
  size_t capacity; //Total capacity of the vector
  size_t size; //Number of elements in the vector
} vector;

void vector_init(vector* v, size_t init_elem_size, size_t init_capacity);
//void vector_resize(vector* v, size_t new_size);
void vector_push_back(vector* v, void* element);
size_t vector_size(vector* v);
void vector_clear(vector* v);
void vector_get(vector* v, size_t index, void *elem);

#endif