#include "utility.h"



int Random()
{
	return rand();
}

double RandomRange(const double min, const double max)
{
	double n = (double)Random() / (double)RAND_MAX;
	n *= (max - min);
	n += min;
	return n;
}

double DotProduct(double ax, double ay, double bx, double by)
{
	return ax * bx + ay * by;
}

double CrossProduct(const double ax, const double ay, const double bx, const double by)
{
	return ax * by - ay * bx;
}



Bool InitVector(Vector* vector, const int capacity)
{
	assert(vector);
	assert(capacity >= 0);
	assert(vector->items == NULL);
	assert(vector->count == 0);
	vector->count = 0;
	vector->capacity = capacity;
	vector->items = malloc(capacity * sizeof(void*));
	if (!vector->items) return FALSE;
	return TRUE;
}

Bool ResizeVector(Vector* vector, const int capacity)
{
	assert(vector);
	assert(capacity >= 0);
	vector->items = realloc(vector->items, capacity * sizeof(void*));
	if (!vector->items) return FALSE;
	vector->capacity = capacity;
	return TRUE;
}

Bool PushBackVector(Vector* vector, const void* item)
{
	assert(vector);
	assert(item);
	if (vector->count + 1 > vector->capacity)
		ResizeVector(vector, vector->capacity * 2);
	vector->items[vector->count] = item;
	vector->count++;
}
