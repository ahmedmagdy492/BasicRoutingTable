
#include "utils.h"

CIDR parse_cidr(const char* cidr) {
	CIDR out = { 0 };

	out.ip = (char*)malloc(sizeof(char) * (strlen(cidr) + 1));
	memset(out.ip, 0, sizeof(char) * (strlen(cidr) + 1));

	char* context = NULL;
	char* token = strtok_s(cidr, "/", &context);
	if (token != NULL) {
		memcpy_s(out.ip, sizeof(char) * (strlen(cidr) + 1), token, strlen(token) * sizeof(char));
	}

	token = strtok_s(NULL, "/", &context);
	out.mask = atoi(token);

	return out;
}

void rev_str(char* str, int len) {
	int i = 0, j = len - 1;

	for (; i < j; ++i, --j) {
		char temp = str[i];
		str[i] = str[j];
		str[j] = temp;
	}
}

void int_to_binary_str(int n, char* out) {
	int q = n / 2;
	int r = n % 2;
	int index = 0;

	while (q > 0) {
		out[index] = 48 + r;
		r = q % 2;
		q = q / 2;
		++index;
	}

	out[index++] = 48 + r;

	size_t len = strlen(out);

	for (int i = index; i < 8; ++i) {
		out[i] = '0';
	}

	rev_str(out, strlen(out));
}

/// <summary>
/// Converts a string IP in Dot Notation to Binary String with Dot Notation
/// </summary>
/// <param name="ip"></param>
/// <returns>0 in case of success, -1 if malloc failed</returns>
int ip_to_binary(char* ip, char** out) {
	char* context = NULL;
	char* bin_ip = (char*)malloc(sizeof(char) * BIN_STR_IP_SIZE);

	if (!bin_ip)
		return -1;

	memset(bin_ip, 0, sizeof(char) * BIN_STR_IP_SIZE);

	char* token = strtok_s(ip, ".", &context);
	int offset = 0;
	while (token != NULL) {
		int octet = atoi(token);
		int_to_binary_str(octet, bin_ip + offset);
		token = strtok_s(NULL, ".", &context);
		offset += 8;
		bin_ip[offset] = '.';
		offset += 1;
	}

	bin_ip[offset - 1] = '\0';
	*out = bin_ip;
	return 0;
}

int mask_value_to_bin_str(int mask, char** out) {
	char* bin_mask = (char*)malloc(sizeof(char) * BIN_STR_IP_SIZE);

	if (!bin_mask)
		return -1;

	memset(bin_mask, 0, sizeof(char) * BIN_STR_IP_SIZE);

	int i = 0;
	for (; i < mask + 2; ++i) {
		bin_mask[i] = '1';
	}

	for (; i < BIN_STR_IP_SIZE - 1; ++i) {
		bin_mask[i] = '0';
	}

	bin_mask[8] = '.';
	bin_mask[17] = '.';
	bin_mask[26] = '.';
	*out = bin_mask;
	return 0;
}

int calc_wildcard(char* mask_bin_str, int len, char** out) {

	char* wildcard = (char*)malloc(sizeof(char) * (len + 1));

	if (!wildcard)
		return -1;

	memset(wildcard, 0, sizeof(char) * (len + 1));

	for (int i = 0; i < len; ++i) {
		if (mask_bin_str[i] == '0') {
			wildcard[i] = '1';
		}
		else if (mask_bin_str[i] == '1') {
			wildcard[i] = '0';
		}
		else {
			wildcard[i] = mask_bin_str[i];
		}
	}

	*out = wildcard;

	return 0;
}

int calc_effective_prefix(const char* ip_bin_str, const char* wildcard, int len, char** out) {
	char* eff_addr = (char*)malloc(sizeof(char) * (len + 1));

	if (!eff_addr)
		return -1;

	memset(eff_addr, 0, sizeof(char) * (len + 1));

	for (int i = 0; i < len; ++i) {
		if (wildcard[i] == '0') {
			eff_addr[i] = ip_bin_str[i];
		}
		else if (wildcard[i] == '1') {
			eff_addr[i] = 'X';
		}
		else {
			eff_addr[i] = wildcard[i];
		}
	}

	*out = eff_addr;

	return 0;
}