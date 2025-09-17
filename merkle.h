#ifndef MERKLE_H
#define MERKLE_H

#include <openssl/evp.h>

typedef struct Node {
  unsigned char hash[EVP_MAX_MD_SIZE];
  struct Node *left;
  struct Node *right;
} Node;

void free_tree(Node *node);

Node *create_tree_from_dir(const char *dir);

#endif