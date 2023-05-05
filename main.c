#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

char *get_string(FILE *stream) {
  int bytes = 0;
  int capacity = 50;
  char *buf = malloc(capacity);

  char c;
  while ((c = fgetc(stream)) != EOF && c != '\r' && c != '\n') {
    //has a valid character
    bytes++;
    if (bytes+1 >= capacity) {
      capacity = capacity*2;
      buf = realloc(buf, capacity);
      if (buf == NULL) {
        return NULL;
      }
    }

    buf[bytes-1] = c;
  }

  if (c == '\r') {
    c = fgetc(stream);
    if (c != '\n') {
      ungetc(c, stream);
    }
  }

  if (bytes == 0) {
    if (c == EOF) {
      free(buf);
      return NULL;
    } else {
      buf = malloc(1);
    }
  }

  buf[bytes] = 0;

  return buf;
}

#define BUZZ_SIZE 1024

void main() {
  char *str;
  int linenum = 0;

  FILE * fin = fopen("./test_cases/caso01.txt", "r");

  while ((str = get_string(fin)) != NULL) {
    printf("%d--\t%s\n",linenum++, str);
    free(str);
  }
}