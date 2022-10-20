#include "dynarray.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <memory.h>

DynamicArray create_dynamic_array_from_size(size_t type_size)
{
	DynamicArray array;

	array.capacity = 5;
	array.size = 0;
	array.element_size = type_size;

	array.data = malloc(type_size * array.capacity);
	if (array.data == NULL)
	{
		array.capacity = 0;
		fprintf(stderr, "Failed to allocate memory for dynamic array\n");
	}

	return array;
}

void destroy_dynamic_array(DynamicArray array)
{
	assert(array.capacity > 0);

	free(array.data);
	array.capacity = 0;
}

void* dynamic_array_get(DynamicArray* array, size_t i)
{
	return (void*)((char*)array->data + (i * array->element_size));
}

void dynamic_array_clear(DynamicArray* array)
{
	array->capacity = 5;
	array->size = 0;

	array->data = realloc(array->data, array->element_size * array->capacity);
}

void dynamic_array_push(DynamicArray* array, void* element)
{
	assert(array->capacity > 0);

	if (array->size == array->capacity)
	{
		array->capacity *= (3 / 2);
		array->data = realloc(array->data, array->element_size * array->capacity);
	}

	memcpy((void*)((char*)array->data + (array->size * array->element_size)), element, array->element_size);
	array->size++;
}

void dynamic_array_pop(DynamicArray* array)
{
	assert(array->capacity > 0);

	array->size -= 1;
	if (array->size <= array->capacity / 2 && array->capacity > 5)
	{
		array->capacity = (2 / 3) * array->capacity + 1;
		array->data = realloc(array->data, array->capacity);
	}
}
