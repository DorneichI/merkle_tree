#include <string.h>

#include "merkle.h"

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

int merkle_equal(const char *dir1, const char *dir2) {
    Node *tree1 = create_tree_from_dir(dir1);
    Node *tree2 = create_tree_from_dir(dir2);
    return memcmp(tree1->hash, tree2->hash, EVP_MAX_MD_SIZE) == 0;
};

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("argument count must be 2, given: %i)\n", argc-1);
    } else {
        char *dir1 = argv[1];
        char *dir2 = argv[2];
        
        int result = merkle_equal(dir1, dir2);

        if (result) {
            printf("The two dirs are equal\n");
        } else {
            printf("The two dirs are not equal\n");
        }

        return result;
    }
}