#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <openssl/evp.h>
#include <limits.h>

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

Node* create_leaf_from_filepath(const char *dir, const char *filepath) {
    Node* node = malloc(sizeof(Node));

    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, EVP_sha256(), NULL);
    unsigned char buf[4096];
    size_t n;
    char fullpath[PATH_MAX];
    snprintf(fullpath, sizeof(fullpath), "%s/%s", dir, filepath);
    FILE *f = fopen(fullpath, "rb");
    if (!f) {
        return NULL;
    }
    EVP_DigestUpdate(ctx, (unsigned char *)filepath, strlen(filepath));
    while ((n = fread(buf, 1, sizeof(buf), f)) > 0) EVP_DigestUpdate(ctx, buf, n);
    fclose(f);
    unsigned int outlen = 0;
    EVP_DigestFinal_ex(ctx, node->hash, &outlen);
    node->left = NULL;
    node->right = NULL;
    return node;
}

Node* create_tree_from_dir(const char *dir) {
    struct dirent *entry;
    DIR *dp;
    dp = opendir(dir);

    Node **leaves = NULL;
    int count = 0;

    while ((entry = readdir(dp))) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        if (entry->d_type == DT_DIR) return NULL;
        leaves = realloc(leaves, sizeof(Node*) * (count + 1));
        leaves[count++] = create_leaf_from_filepath(dir, entry->d_name);
    }

    while (count > 1) {
        int new_count = (count + 1) / 2;
        Node **parents = malloc(sizeof(Node*) * new_count);
        for (int i = 0; i < count; i += 2) {
            fflush(stdout);
            if (i + 1 < count) {
                parents[i/2] = create_node(leaves[i], leaves[i+1]);
            } else {
                parents[i/2] = create_node(leaves[i], leaves[i]);
            }
        }
        free(leaves);
        leaves = parents;
        count = new_count;
    }
    Node *root = leaves[0];
    free(leaves);
    return root;
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