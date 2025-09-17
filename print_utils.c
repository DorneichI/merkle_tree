#include "print_utils.h"

void print_hash_hex(const unsigned char *hash, size_t len) {
    for (size_t i = 0; i < len; i++) {
        printf("%02x", hash[i]);
    }
    printf("\n");
}

void print_tree_in_order(Node *node) {
    if (node == NULL) {
        return;
    }
    print_tree_in_order(node->left);
    print_hash_hex(node->hash, EVP_MAX_MD_SIZE);
    print_tree_in_order(node->right);

}