
#include "bitmap.h"



int init_bitmap(bitmap_t* bm, size_t bits_count) {
	bm->bit_arr = (uint8_t*)malloc(sizeof(bits_count / 8));

	if (!bm->bit_arr)
		return -1;

	memset(bm->bit_arr, 0, sizeof(bits_count / 8));
	bm->bits_count = bits_count;

	return 0;
}

void bin_str_to_bitmap(bitmap_t* bm, const char* bin_str, int prefix_len) {
	int c = 0;
	for (int i = 0; i < prefix_len; ++i) {
		if (bin_str[i] == '0') {
			++c;
		}
		else if (bin_str[i] == '1') {
			bitmap_set_at(bm, c);
			++c;
		}
	}
}

void bitmap_set_at(bitmap_t* bm, int index) {
	unsigned int bitIndexToSet = 1;
	int diff = bm->bits_count - index;
	bitIndexToSet = bitIndexToSet << diff;

	unsigned int* tmp = (unsigned int*)bm->bit_arr;
	*tmp = *tmp | bitIndexToSet;
}

void bitmap_unset_at(bitmap_t* bm, int index) {
	// (TBD)
	/*unsigned int bitIndexToSet = 0;
	unsigned int* tmp = (unsigned int*)bm->bit_arr;
	bitIndexToSet = bitIndexToSet | *tmp;

	int diff = bm->bits_count - index;
	bitIndexToSet = bitIndexToSet << diff;


	*tmp = *tmp & bitIndexToSet;*/
}

void free_bitmap(bitmap_t* bm) {
	free(bm->bit_arr);
}