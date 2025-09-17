#include <dirent.h>
#include <openssl/evp.h>
#include <string.h>

#include "hash_utils.h"
#include "merkle.h"

void create_hash_of_nodes(const Node node1, const Node node2,
                          unsigned char out_hash[EVP_MAX_MD_SIZE]) {
  EVP_MD_CTX *ctx = EVP_MD_CTX_new();
  EVP_DigestInit_ex(ctx, EVP_sha256(), NULL);
  EVP_DigestUpdate(ctx, node1.hash, EVP_MAX_MD_SIZE);
  EVP_DigestUpdate(ctx, node2.hash, EVP_MAX_MD_SIZE);
  unsigned int outlen = 0;
  EVP_DigestFinal_ex(ctx, out_hash, &outlen);
};

void free_tree(Node *node) {
  if (node == NULL) {
    return;
  }
  free_tree(node->left);
  free_tree(node->right);
  free(node);
}

Node *create_node(Node *child1, Node *child2) {
  Node *node = malloc(sizeof(Node));
  unsigned char hash[EVP_MAX_MD_SIZE];
  create_hash_of_nodes(*child1, *child2, hash);
  memcpy(node->hash, hash, EVP_MAX_MD_SIZE);
  node->left = child1;
  node->right = child2;
  return node;
}

Node *create_leaf(const unsigned char *hash) {
  Node *node = malloc(sizeof(Node));
  memcpy(node->hash, hash, EVP_MAX_MD_SIZE);
  node->left = NULL;
  node->right = NULL;
  return node;
}

Node *duplicate_leaf(Node *src) {
  if (src->left != NULL || src->right != NULL) {
    return NULL;
  }
  Node *copy = malloc(sizeof(Node));
  memcpy(copy->hash, src->hash, EVP_MAX_MD_SIZE);
  copy->left = NULL;
  copy->right = NULL;
  return copy;
}

Node *create_tree_from_dir(const char *dir) {
  struct dirent *entry;
  DIR *dp;
  dp = opendir(dir);

  Node **leaves = NULL;
  int count = 0;

  while ((entry = readdir(dp))) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
      continue;
    if (entry->d_type == DT_DIR)
      return NULL;
    leaves = realloc(leaves, sizeof(Node *) * (count + 1));

    unsigned char hash[EVP_MAX_MD_SIZE];
    hash_file(dir, entry->d_name, hash);
    leaves[count++] = create_leaf(hash);
  }

  while (count > 1) {
    int new_count = (count + 1) / 2;
    Node **parents = malloc(sizeof(Node *) * new_count);
    for (int i = 0; i < count; i += 2) {
      fflush(stdout);
      if (i + 1 < count) {
        parents[i / 2] = create_node(leaves[i], leaves[i + 1]);
      } else {
        parents[i / 2] = create_node(leaves[i], duplicate_leaf(leaves[i]));
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