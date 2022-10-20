#ifndef DYNARRAY_H
#define DYNARRAY_H

#include <stdint.h>

#define create_dynamic_array( type ) create_dynamic_array_from_size(sizeof( type ))

typedef struct DynamicArray
{
	void* data;

	size_t element_size;
	size_t size;
	size_t capacity;
} DynamicArray;

DynamicArray create_dynamic_array_from_size(size_t type_size);
void destroy_dynamic_array(DynamicArray array);

void* dynamic_array_get(DynamicArray* array, size_t i);
void dynamic_array_clear(DynamicArray* array);

void dynamic_array_push(DynamicArray* array, void* element);
void dynamic_array_pop(DynamicArray* array);

#endif // DYNARRAY_H