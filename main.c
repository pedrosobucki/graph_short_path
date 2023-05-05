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

void main() {
  char c;
  int cc, hc, wc = 0;

  FILE *stream = fopen("./test_cases/caso01.txt", "r");

  int h, w;
  fscanf (stream, "%d", &h);
  fscanf (stream, "%d", &w);

  printf("\nheight: %d\nwidth: %d\n",h,w);

  fgetc(stream);
  printf("%d - ", hc);

  while (hc <= h) {
    c = fgetc(stream);
    printf("%c", c);

    if (c == '\n' || c == '\r') {
      hc++;
      printf("%d - ", hc);
    }

    cc++;
  }

  fclose(stream);

  printf("\n\ncount: %d", cc);
}