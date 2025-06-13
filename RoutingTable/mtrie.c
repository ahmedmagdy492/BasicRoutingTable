#include "mtrie.h"

int init_mtrie(mtrie_t* mtrie) {
	mtrie->root = (mtrie_node_t*)malloc(sizeof(mtrie_node_t));
	if (!mtrie->root)
		return -1;

	init_bitmap(&mtrie->root->prefix, 1);
	init_bitmap(&mtrie->root->wildcard, 1);
	mtrie->root->prefix_len = 0;
	mtrie->root->parent = NULL;
	
	char defaultRoute[] = "0.0.0.0/0";
	mtrie->root->data = malloc(strlen(defaultRoute)+1);
	if (!mtrie->root->data)
		return -2;

	memset(mtrie->root->data, 0, strlen(defaultRoute) + 1);
	memcpy_s(mtrie->root->data, strlen(defaultRoute), defaultRoute, strlen(defaultRoute));

	mtrie->root->child[0] = NULL;
	mtrie->root->child[1] = NULL;
	mtrie->root->child[2] = NULL;

	return 0;
}

/// <summary>
/// Inserts a node into the given mtrie
/// </summary>
/// <param name="mtrie">A pointer to the mtrie object</param>
/// <param name="data">IP in CIDR notation (e.g. 192.168.1.2/24)</param>
/// <returns>
///	 0 -> success
///	-1 -> node heap allocation failure
/// -2 -> ip_to_binary failed with -1
/// -3 -> mask_value_to_bin_str failed with -1
/// -4 -> calc_wildcard failed with -1
/// -5 -> calc_effective_prefix failed with -1
/// -6 -> dynamic allocation for prefix bitmap failed
/// -7 -> dynamic allocation for wildcard bitmap failed
/// </returns>
int insert_mtrie_node(mtrie_t* mtrie, const char* data) {
	mtrie_node_t* node = (mtrie_node_t*)malloc(sizeof(mtrie_node_t));

	if (!node)
		return -1;

	node->data = malloc(strlen(data) + 1);
	if (!node->data)
		return -2;

	memset(node->data, 0, strlen(data) + 1);
	memcpy_s(node->data, strlen(data), data, strlen(data));

	CIDR cidr_struct = parse_cidr(data);
	node->prefix_len = cidr_struct.mask;

	char* bin_ip = NULL;
	char* temp_ip = (char*)malloc(strlen(cidr_struct.ip)+1);
	memset(temp_ip, 0, strlen(cidr_struct.ip) + 1);
	memcpy_s(temp_ip, strlen(cidr_struct.ip), cidr_struct.ip, strlen(cidr_struct.ip));

	if (ip_to_binary(temp_ip, &bin_ip) == -1) {
		free(temp_ip);
		return -2;
	}
	free(temp_ip);

	char* mask_value = NULL;
	if (mask_value_to_bin_str(cidr_struct.mask, &mask_value) == -1)
		return -3;

	char* wildcard = NULL;
	if (calc_wildcard(mask_value, strlen(mask_value), &wildcard) == -1)
		return -4;

	char* eff_addr = NULL;
	if (calc_effective_prefix(bin_ip, wildcard, strlen(wildcard), &eff_addr) == -1)
		return -5;

	if (init_bitmap(&node->prefix, cidr_struct.mask) == -1)
		return -6;
	
	bin_str_to_bitmap(&node->prefix, bin_ip, cidr_struct.mask);

	if (init_bitmap(&node->wildcard, cidr_struct.mask) == -1)
		return -7;
	bin_str_to_bitmap(&node->wildcard, wildcard, cidr_struct.mask);

	node->child[0] = NULL;
	node->child[1] = NULL;
	node->child[2] = NULL;

	if (eff_addr[0] == '0') {
		if (mtrie->root->child[ZERO] == NULL) {
			mtrie->root->child[ZERO] = node;
		}
		else {

		}
	}
	else if (eff_addr[0] == '1') {
		if (mtrie->root->child[ONE] == NULL) {
			mtrie->root->child[ONE] = node;
		}
		else {

		}
	}
	else if (eff_addr[0] == 'X') {
		if (mtrie->root->child[DONT_CARE] == NULL) {
			mtrie->root->child[DONT_CARE] = node;
		}
		else {

		}
	}

	free(cidr_struct.ip);
	free(bin_ip);
	free(mask_value);
	free(wildcard);
	free(eff_addr);

	return 0;
}

void free_mtrie_node(mtrie_node_t* node) {
	if (!node)
		return;

	free_bitmap(node->prefix.bit_arr);
	free_bitmap(node->wildcard.bit_arr);
	free(node->data);
}

void free_mtrie(mtrie_t* mtrie) {
	// (TBD)
	/*free_bitmap(&mtrie->root->prefix);
	free_bitmap(&mtrie->root->wildcard);*/
}