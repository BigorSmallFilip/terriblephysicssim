#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>

typedef bool Bool;
#define FALSE false
#define TRUE true

#define INLINE inline

int Random();
double RandomRange(const double min, const double max);

double DotProduct(
	double ax,
	double ay,
	double bx,
	double by
);

double CrossProduct(
	const double ax,
	const double ay,
	const double bx,
	const double by
);


typedef struct
{
	void** items;
	int count;
	int capacity;
} Vector;

Bool InitVector(Vector* vector, const int capacity);
Bool ResizeVector(Vector* vector, const int capacity);
Bool PushBackVector(Vector* vector, const void* item);
