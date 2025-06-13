#pragma once

#include "bitmap.h"
#include "utils.h"

typedef enum bit_type_ {
	ZERO,
	ONE,
	DONT_CARE,
	BIT_TYPE_MAX
} bit_type_t;

typedef struct mtrie_node_ {
	bitmap_t prefix;
	bitmap_t wildcard;
	uint16_t prefix_len;
	struct mtrie_node_* parent;
	struct mtrie_node_* child[BIT_TYPE_MAX];
	void* data;
} mtrie_node_t;

typedef struct mtrie_ {
	mtrie_node_t* root;
} mtrie_t;

int init_mtrie(mtrie_t* mtrie);

int insert_mtrie_node(mtrie_t* mtrie, const char* data);

void free_mtrie_node(mtrie_node_t* node);

void free_mtrie(mtrie_t* mtrie);