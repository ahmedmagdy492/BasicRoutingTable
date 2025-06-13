#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

typedef struct bitmap_t_ {
	uint8_t* bit_arr;
	size_t bits_count;
} bitmap_t;

int init_bitmap(bitmap_t* bm, size_t bits_count);

void bin_str_to_bitmap(bitmap_t* bm, const char* bin_str, int prefix_len);

void bitmap_set_at(bitmap_t* bm, int index);

void bitmap_unset_at(bitmap_t* bm, int index);

void free_bitmap(bitmap_t* bm);