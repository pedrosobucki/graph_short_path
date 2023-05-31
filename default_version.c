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
  int val = -1;

  if (queue->front == NULL) {
    return val;
  }

  Node* target = queue->front;
  queue->front = target->next;

  if (queue->front == NULL) {
    queue->rear = NULL;
  }

  val = target->val;
  free(target);

  return val;
}

bool is_empty(Queue* queue) {
  return queue->front == NULL;
}

// GLOBAL VARIABLES
int h, w;
Queue* qr;
Queue* qc;

int dr[4] = {0, 1, 0, -1};
int dc[4] = {-1, 0, 1, 0};

bool explore_neighbors(int target, bool marked[][w], char map[][w]) {
  for (int j=0; j<4; j++) {

    int r = dequeue(qr);
    int c = dequeue(qc);

    int rr = r + dr[j];
    int cc = c + dc[j];

    char cell = map[rr][cc];

    printf("\nexploring %d,%d: %c.\n", r, c, cell);

    if (rr < 0 || rr >= h || cc < 0 || cc >= w)
      continue;

    if (marked[rr][cc] || cell == '#')
      continue;

    marked[rr][cc] = true;

    if (cell - '0' == target)
      return  true;

    enqueue(qr, rr);
    enqueue(qc, cc);
  }

  return false;
}

typedef struct Harbor {
  int r, c;
} Harbor;

Harbor* new_harbor(int r, int c) {
  Harbor* harb = malloc(sizeof(Harbor));
  harb->r = r;
  harb->c = c;

  return harb;
}

// RUNS MAIN CODE
void main(void) {
  char c;
  int cc, row, col = 0;
  bool finished_harbors[9] = {false, false, false, false, false, false, false, false, false};
  Harbor* harbors[9];
  int distance[9];

  FILE *stream = fopen("./test_cases/caso01.txt", "r");

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
      harbors[num-1] = new_harbor(row, col);
    }

    map[row][col] = c;
    col++;

    // cc++;
  }
  fclose(stream);

  // PRINTS HARBOR DATA
  printf("\n\nHARBORS:\n");
  for (int i=0; i<9; i++) {
    Harbor* harb = harbors[i];
    printf("%d: %d -> [%d,%d]\n",i, (i+1), harb->r, harb->c);
  }
  // int size = sizeof(harbors);
  // printf("\nsize: %d", size);

  // iterates over harbor list
  qr = new_queue();
  qc = new_queue();
  int hr, hc;
  int acc, target = 2;

  // foreach Harbor
  for (int i=0; i<9;) {
    Harbor* har = harbors[i];
    bool found = false;
    hr = har->r;
    hc = har->c;

    marked[hr][hc] = true;

    enqueue(qr, hr);
    enqueue(qc, hc);

    printf("searching for: %d\n---------------------\n", target);
    while (!found && !is_empty(qr)) {
      found = explore_neighbors(target, marked, map);
      printf("found: %d & empty:%d\n", found, is_empty(qr));
    }
    printf("\n---------------------\n\n\n");

    if (found == true) {
      i += acc;
      acc = 0;
    }

    target = i + acc++;

    if (target >= 9) {
      break;
    }

    // printf("weight: %d");
    // printf("current: %d\ntarget:%d\n", i, target);
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
