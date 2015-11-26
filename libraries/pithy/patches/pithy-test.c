#include "pithy.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

/* For fuzzing only */
#undef EXIT_FAILURE
#define EXIT_FAILURE EXIT_SUCCESS

int main(int argc, char *argv[]) {
  char* input = NULL;
  char* output = NULL;
  size_t input_size = 0;
  size_t output_size = 0;
  bool compress = false;
  int level = 6;
  int ec = EXIT_SUCCESS;

  if (argc != 2) {
    fprintf (stderr, "Usage: %s (c|d) < input > output\n", argv[0]);
    ec = EXIT_FAILURE;
    goto cleanup;
  }

  switch (argv[1][0]) {
    case 'c':
      compress = true;
      if (argv[1][1] != '\0') {
        level = atoi (&(argv[1][1]));
      }
      break;
    case 'd':
      compress = false;
      break;
    default:
      fprintf (stderr, "Invalid mode '%c'\n", argv[1][0]);
      break;
  }

  while (1) {
    input = realloc (input, input_size + 8192);
    assert (input != NULL);
    size_t bytes_read = fread (input + input_size, 1, 8192, stdin);
    if (bytes_read > 0) {
      input_size += bytes_read;
    } else {
      if (feof (stdin))
        break;

      fprintf (stderr, "I/O error: %s\n", strerror (errno));
      ec = EXIT_FAILURE;
      goto cleanup;
    }
  }

  assert (input_size != 0);

  if (compress) {
    output_size = pithy_MaxCompressedLength (input_size);
  } else {
    int x = pithy_GetDecompressedLength(input, input_size, &output_size);
    if (x <= 0 || output_size == 0) {
      fprintf (stderr, "Unable to get decompressed length (%d)\n", x);
      ec = EXIT_FAILURE;
      goto cleanup;
    }
  }

  assert (output_size != 0);
  output = malloc (output_size);
  assert (output != NULL);

  if (compress) {
    int r = pithy_Compress (input, input_size, output, output_size, level);
    if (r <= 0) {
      fprintf (stderr, "Compression failed: %d\n", r);
      ec = EXIT_FAILURE;
      goto cleanup;
    }

    output_size = (size_t) r;
  } else {
    int r = pithy_Decompress(input, input_size, output, output_size);
    if (r != 1) {
      fprintf (stderr, "Decompression failed: %d\n", r);
      ec = EXIT_FAILURE;
      goto cleanup;
    }
  }

  fwrite (output, 1, output_size, stdout);

 cleanup:
  free (input);
  free (output);
  return ec;
}
