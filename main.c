#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

void main(void) {
  char c;
  int cc, hc, wc = 0;

  FILE *stream = fopen("./test_cases/caso01.txt", "r");

  int h, w;
  fscanf (stream, "%d", &h);
  fscanf (stream, "%d", &w);

  printf("\nheight: %d\nwidth: %d\n",h,w);

  c = fgetc(stream);
  printf("%d - ", hc);

  while (hc < h) {
    c = fgetc(stream);
    printf("%c", c);

    if (c == '\n' || c == '\r' || c == EOF) {
      hc++;
      printf("%d - ", hc);
    }

    cc++;
  }

  fclose(stream);

  printf("\n\ncount: %d", cc);
}