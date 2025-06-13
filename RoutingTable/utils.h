#pragma once

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define BIN_STR_IP_SIZE 36

typedef struct _CIDR {
	char* ip;
	int mask;
} CIDR;

/// <summary>
/// Memory for ip field is allocated dynamically
/// </summary>
CIDR parse_cidr(const char* cidr);

void rev_str(char* str, int len);

void int_to_binary_str(int n, char* out);

/// <summary>
/// Converts a string IP in Dot Notation to Binary String with Dot Notation
/// </summary>
/// <param name="ip"></param>
/// <returns>0 in case of success, -1 if malloc failed</returns>
int ip_to_binary(char* ip, char** out);

int mask_value_to_bin_str(int mask, char** out);

int calc_wildcard(char* mask_bin_str, int len, char** out);

int calc_effective_prefix(const char* ip_bin_str, const char* wildcard, int len, char** out);