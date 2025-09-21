#include <string.h>

#include "merkle.h"
#include "print_utils.h"

int merkle_equal(const char *dir1, const char *dir2) {
  Node *tree1 = create_tree_from_dir(dir1);
  Node *tree2 = create_tree_from_dir(dir2);
  int result = memcmp(tree1->hash, tree2->hash, EVP_MAX_MD_SIZE) == 0;
  free_tree(tree1);
  free_tree(tree2);
  return result;
};

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("argument count must be 2, given: %i)\n", argc - 1);
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