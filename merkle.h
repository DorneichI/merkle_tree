#ifndef MERKLE_H
#define MERKLE_H

#include <openssl/evp.h>

typedef struct Node {
    unsigned char hash[EVP_MAX_MD_SIZE];
    struct Node* left;
    struct Node* right;
} Node;

Node* create_node(Node *child1, Node *child2);



#endif