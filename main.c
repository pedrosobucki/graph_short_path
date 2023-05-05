#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

void main(void) {
  char c;
  int cc, row, col = 0;

  FILE *stream = fopen("./test_cases/caso01.txt", "r");

  int h, w;
  fscanf (stream, "%d", &h);
  fscanf (stream, "%d", &w);

  char map[h][w];

  // printf("\nheight: %d\nwidth: %d\n",h,w);

  fgetc(stream); // removes excessive EOL caracter before array start
  // printf("%d - ", row);

  while (row < h) {
    c = fgetc(stream);
    // printf("%c", c);

    if (c == '\n' || c == '\r' || c == EOF) {
      row++;
      col = 0;
      continue;
      // printf("%d - ", row);
    }

    map[row][col] = c;
    col++;

    // cc++;
  }

  fclose(stream);

  FILE *write_file = fopen("./generated_map.txt", "w");

  for (int i=0; i<h; i++) {
    // printf("%d - ", i);
    for (int j=0; j<w; j++) {
      // printf("%c", map[i][j]);
      fputc(map[i][j], write_file);
    }
    fputc('\n', write_file);
    // printf("\n");
  }

  fclose(write_file);

  // printf("\n\ncount: %d", cc);
}