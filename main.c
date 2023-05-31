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
int h, w, nodes_in_next_layer;
Queue* qr;
Queue* qc;

// moddifiers for neighbors navigation
int dr[4] = {0, 1, 0, -1};
int dc[4] = {-1, 0, 1, 0};

// checls if neighbors (up, down, left, right) are traversable and new paths, if so, adds them to queue for navigation
void explore_neighbors(int r, int c, int target, bool marked[][w], char map[][w]) {
  for (int j=0; j<4; j++) {

    int rr = r + dr[j];
    int cc = c + dc[j];

    // skips if is offset
    if (rr < 0 || rr >= h || cc < 0 || cc >= w)
      continue;

    char cell = map[rr][cc];

    // skips if is already marked or is not traversable
    if (marked[rr][cc] == 1 || cell == '*') 
      continue;

    // marks cell
    marked[rr][cc] = 1;

    // queues cell
    enqueue(qr, rr);
    enqueue(qc, cc);

    nodes_in_next_layer++;
  }
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
void main(int argc, char *argv[]) {
  char c;
  int cc, row, col = 0;
  Harbor* harbors[9];

  // selects test case file
  char filename[23];
  strcpy(filename, "./test_cases/caso") ;
  strcat(filename, argv[1]) ;
  strcat(filename, ".txt") ;

  FILE *stream = fopen(filename, "r");

  // stores graph height and width
  fscanf (stream, "%d", &h);
  fscanf (stream, "%d", &w);

  // allocates memory for graph
  char map[h][w];

  fgetc(stream); // removes excessive EOL caracter before array start

  // populates graph
  while (row < h) {
    c = fgetc(stream);

    if (c == '\n' || c == '\r' || c == EOF) {
      row++;
      col = 0;
      continue;
    }

    // STORES HARBOR DATA  
    if (isdigit(c)) {
      int num = c - '0'; //casts number to int
      harbors[num-1] = new_harbor(row, col);
    }

    map[row][col] = c;
    col++;
  }
  fclose(stream);

  int hr, hc;
  int acc = 1;
  int target = 1;
  bool marked[h][w];
  int fuel_needed = 0;

  // foreach Harbor
  for (int i=0; i<9;) {
    Harbor* har = harbors[i];
    memset(marked, 0, sizeof(marked)); // sets all cells as 'unmarked'
    
    bool found = false;
    int harbor_travel_distance = 0;
    int nodes_left_in_layer = 1;

    if (target < 9) {
      target++;
    } else {
      target = 1;
    }

    // starts empty queue 
    qr = new_queue();
    qc = new_queue();

    hr = har->r;
    hc = har->c;

    nodes_in_next_layer = 0;

    // marks harbor as traversed
    marked[hr][hc] = 1;

    // queues harbor for traversing
    enqueue(qr, hr);
    enqueue(qc, hc);

    // printf("%d[%d,%d] searching for: %d\n---------------------\n", i + 1, hr, hc, target);

    while (!is_empty(qr)) {

      // retrieves current node coordinates
      int r = dequeue(qr);
      int c = dequeue(qc);

      // if current node is target harbor, stops queue iteration
      if (map[r][c] - '0' == target) {
        found = true;
        // printf("\n********************\nfound at %dx%d in layer %d\n", r,c, harbor_travel_distance);
        // printf("proof: %c\n*********************\n", map[r][c]);
        break;
      }

      // adds valid path neighbors to queue
      explore_neighbors(r, c, target, marked, map);
      nodes_left_in_layer--;

      // if all nodes in layer have been analysed, reset data and move to next layer
      if (nodes_left_in_layer == 0) {
        nodes_left_in_layer = nodes_in_next_layer;
        nodes_in_next_layer = 0;
        harbor_travel_distance++;
      }

    }
    // printf("\n---------------------\n\n\n");

    // actions if path id found between two harbors
    if (found == true) {
      // updates total fuel needed for travel
      fuel_needed += harbor_travel_distance;

      // sets next available harbor for travel
      i += acc;
      acc = 0;
    }

    acc++;

    // stops iteration when ship has returned home
    if (target == 1) {
      break;
    }

    free(qr);
    free(qc);
  }

  printf("total fuel needed: %d.\n\n", fuel_needed);

  // // PRINT MAP TO FILE
  // FILE *write_file = fopen("./generated_map.txt", "w");

  // for (int i=0; i<h; i++) {
  //   // printf("%d - ", i);
  //   for (int j=0; j<w; j++) {
  //     // printf("%c", map[i][j]);
  //     fputc(map[i][j], write_file);
  //   }
  //   fputc('\n', write_file);
  //   // printf("\n");
  // }

  // fclose(write_file);
}
