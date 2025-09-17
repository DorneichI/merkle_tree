#ifndef PRINT_UTILS_H
#define PRINT_UTILS_H

#include "merkle.h"

void print_hash_hex(const unsigned char *hash, size_t len);

void print_tree_in_order(Node *node);

#endif