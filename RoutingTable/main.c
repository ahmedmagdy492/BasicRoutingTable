#include <stdio.h>

#include "mtrie.h"

int main() {
	mtrie_t routing_table;
	init_mtrie(&routing_table);

	char cidr1[] = "1.2.3.4/32";
	char cidr2[] = "128.2.3.4/32";
	char cidr3[] = "0.0.0.0/0";
	int res_code = insert_mtrie_node(&routing_table, cidr1);
	res_code = insert_mtrie_node(&routing_table, cidr2);
	res_code = insert_mtrie_node(&routing_table, cidr3);

	printf("%s\n", (char*)routing_table.root->data);
	for (int i = 0; i < BIT_TYPE_MAX; ++i) {
		printf("%s\n", (char*)routing_table.root->child[i]->data);
	}

	free_mtrie(&routing_table);
}