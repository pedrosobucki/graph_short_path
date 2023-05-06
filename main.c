#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

// DEFINES STRUCTS

// Queue
typedef struct Node {
  int val;
  struct Node* next;
} Node;

typedef struct Queue {
  Node *front, *rear;
} Queue;

Node* new_node(int val) {
  Node* nn = malloc(sizeof(Node));
  nn->val = val;
  nn->next = NULL;

  return nn;
}

Queue* new_queue(){
  Queue* q = malloc(sizeof(Queue));
  q->front = q->rear = NULL;
  return q;
}

void enqueue(Queue* queue, int val) {
  Node* nn = new_node(val);

  if (queue->rear == NULL) {
    queue->rear = queue->front = nn;
    return;
  }

  queue->rear->next = nn;
  queue->rear = nn;
}

int dequeue(Queue* queue) {
  if (queue->front == NULL) {
    return -1;
  }

  Node* target = queue->front;
  queue->front = target->next;

  if (queue->front == NULL) {
    queue->rear = NULL;
  }

  free(target);
  return target->val;
}

// Harbors
typedef struct Harbor {
  int num, target, weight, r, c;
} Harbor;

Harbor new_harbor(int num, int r, int c) {
  Harbor h;
  h.num = num;
  h.target = num+1;
  h.weight = 0;
  h.r = r;
  h.c = c;

  if (num == 9) {
    h.target = 1;
  }

  // printf("-%d-\n",h.num);
  return h;
}

// RUNS MAIN CODE
void main(void) {
  char c;
  int cc, row, col = 0;
  Harbor harbors[9];

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

    // STORES HARBOR DATA  
    if (isdigit(c)) {
      int num = c - '0'; //casts number to int
      harbors[num-1] = new_harbor(num, row, col);
    }

    map[row][col] = c;
    col++;

    // cc++;
  }
  fclose(stream);

  // // PRINTS HARBOR DATA
  // printf("\n\nHARBORS:\n");
  // for (int i=0; i<9; i++) {
  //   Harbor h = harbors[i];
  //   printf("%d: %d -> %d in [%d,%d]\n",i,h.num, h.target, h.r, h.c);
  // }

  // iterates over harbor list
  Queue* qr = new_queue();
  Queue* qc = new_queue();
  for (int i=0; i<9; i++) {
    
    // while (cc < h)

  }


  // PRINT MAP TO FILE
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
