#include <openssl/evp.h>
#include <string.h>

#include "hash_utils.h"

void hash_file(const char *dir, const char *file, unsigned char *out_hash) {
  EVP_MD_CTX *ctx = EVP_MD_CTX_new();
  EVP_DigestInit_ex(ctx, EVP_sha256(), NULL);
  unsigned char buf[4096];
  size_t n;
  char fullpath[PATH_MAX];
  snprintf(fullpath, sizeof(fullpath), "%s/%s", dir, file);
  FILE *f = fopen(fullpath, "rb");
  if (!f) {
    memset(out_hash, 0, EVP_MAX_MD_SIZE);
    return;
  }
  EVP_DigestUpdate(ctx, (unsigned char *)file, strlen(file));
  while ((n = fread(buf, 1, sizeof(buf), f)) > 0)
    EVP_DigestUpdate(ctx, buf, n);
  fclose(f);
  unsigned int outlen = 0;
  EVP_DigestFinal_ex(ctx, out_hash, &outlen);
}
