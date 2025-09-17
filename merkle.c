#include <string.h>

#include "merkle.h"

void create_hash_of_nodes(Node node1, Node node2, unsigned char out_hash[EVP_MAX_MD_SIZE]) {
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, EVP_sha256(), NULL);
    EVP_DigestUpdate(ctx, node1.hash, EVP_MAX_MD_SIZE);
    EVP_DigestUpdate(ctx, node2.hash, EVP_MAX_MD_SIZE);
    unsigned int outlen = 0;
    EVP_DigestFinal_ex(ctx, out_hash, &outlen);
};

Node* create_node(Node *child1, Node *child2) {
    Node* node = malloc(sizeof(Node));
    unsigned char hash[EVP_MAX_MD_SIZE];
    create_hash_of_nodes(*child1, *child2, hash);
    memcpy(node->hash, hash, EVP_MAX_MD_SIZE);
    node->left = child1;
    node->right = child2;
    return node;
}