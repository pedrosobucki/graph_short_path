#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

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

// Harbor*s
typedef struct Harbor {
  int num, target, weight, r, c;
} Harbor;

Harbor* new_harbor(int num, int r, int c) {
  Harbor* h = malloc(sizeof(Harbor));
  h->num = num;
  h->target = num+1;
  h->weight = 0;
  h->r = r;
  h->c = c;

  if (num == 9) {
    h->target = 1;
  }

  // printf("-%d-\n",h.num);
  return h;
}

// GLOBAL VARIABLES
int h, w;
Queue* qr;
Queue* qc;

int dr[4] = {0, 1, 0, -1};
int dc[4] = {-1, 0, 1, 0};

bool explore_neighbors(Harbor* har, bool** marked, char** map, int r, int c) {
  int rr, cc;
  char cell = map[rr][cc];

  for (int j=0; j<4; j++) {
    rr = r + dr[j];
    cc = c + dc[j];

    if (rr < 0 || rr >= h || cc < 0 || cc >= w)
      continue;

    if (marked[rr][cc] || cell == '#')
      continue;

    marked[rr][cc] = true;

    if (cell - '0' == har->target)
      return  true;

    enqueue(qr, rr);
    enqueue(qc, cc);
  }

  return false;
}

// RUNS MAIN CODE
void main(void) {
  char c;
  int cc, row, col = 0;
  Harbor* harbors[9];
  int distance[9] = {0,0,0,0,0,0,0,0,0};

  FILE *stream = fopen("./test_cases/caso01.txt", "r");

  int h, w;
  fscanf (stream, "%d", &h);
  fscanf (stream, "%d", &w);

  char map[h][w];
  bool marked[h][w];

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

  // PRINTS HARBOR DATA
  printf("\n\nHARBORS:\n");
  for (int i=0; i<9; i++) {
    Harbor* h = harbors[i];
    printf("%d: %d -> %d in [%d,%d]\n",i,h->num, h->target, h->r, h->c);
  }
  // int size = sizeof(harbors);
  // printf("\nsize: %d", size);

  // iterates over harbor list
  qr = new_queue();
  qc = new_queue();
  int hr, hc;
  bool found = false;

  // foreach Harbor
  for (int i=0; i<9; i++) {
    Harbor* har = harbors[i];
    hr = har->r;
    hc = har->c;

    marked[hr][hc] = true;

    enqueue(qr, hr);
    enqueue(qc, hc);

    while (!found || sizeof(qr) > 0) {
      found = explore_neighbors(har, marked, map, hr, hc);
    }

    printf("weight: %d", har->weight);
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
