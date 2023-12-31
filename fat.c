#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void usage(FILE *stream) {
  fprintf(stream, "Usage: ./fat <file_name>\n");
}

char *slurp_file(const char *file_path, size_t *size) {
  char *buffer = NULL;
  FILE *f = fopen(file_path, "rb");
  if (f == NULL)
    goto error;

  if (fseek(f, 0, SEEK_END) < 0)
    goto error;

  long m = ftell(f);
  if (m < 0)
    goto error;

  buffer = malloc(sizeof(char) * m);

  if (buffer == NULL) 
    goto error;
  
  if (fseek(f, 0, SEEK_SET) < 0)
    goto error;
  
  size_t n = fread(buffer, 1, m, f);
  assert(n == (size_t) m);

  if (ferror(f)) 
    goto error;

  if (size) 
    *size = n;

  fclose(f);
  return buffer;

error:
  if (f) 
    fclose(f);
  if (buffer) 
    free(buffer);
  return NULL;
}

int main(int argc, char **argv) {
  if(argc < 2) {
    usage(stderr);
    fprintf(stderr, "ERROR: input file is not provided\n");
    exit(1);
  }

  const char *input_file_path = argv[1];

  size_t content_size = 0;
  char *content = slurp_file(input_file_path, &content_size);
  if (content == NULL) {
    fprintf(stderr, "ERROR: could not read file %s: %s\n", 
        input_file_path, strerror(errno));
    exit(1);
  }

  fwrite(content, 1, content_size, stdout);

  return 0;
}
