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

int dr[4] = {0, 1, 0, -1};
int dc[4] = {-1, 0, 1, 0};

void explore_neighbors(int r, int c, int target, int marked[][w], char map[][w]) {
  for (int j=0; j<4; j++) {

    int rr = r + dr[j];
    int cc = c + dc[j];

    // int found2 = marked[4][11];

    // printf("\nexploring %d,%d: %c\n", r, c, cell);
    // if (cell - '0' == target)
    //   printf("Found \"%d\" at [%d,%d]", target,rr,cc);

    if (rr < 0 || rr >= h || cc < 0 || cc >= w) {
      // printf("\t- Found missplacement at: [%d,%d]\n", r,c);
      continue;
    }

    char cell = map[rr][cc];

    if (marked[rr][cc] == 1 || cell == '*') {
      // printf("\t * [%d,%d](%c) marked!\n", rr, cc, cell);
      continue;
    }

    marked[rr][cc] = 1;

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
  int distance[9];

  char filename[23];
  strcpy(filename, "./test_cases/caso") ;
  strcat(filename, argv[1]) ;
  strcat(filename, ".txt") ;

  FILE *stream = fopen(filename, "r");

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
    // printf("%d: %d -> [%d,%d]\n",i, (i+1), harb->r, harb->c);
  }

  // qr = new_queue();
  // qc = new_queue();
  int hr, hc;
  int acc = 1;
  int target = 1;
  int marked[h][w];


  // foreach Harbor
  for (int i=0; i<9;) {
    Harbor* har = harbors[i];
    memset(marked, 0, sizeof(marked));//resets marked array
    
    bool found = false;
    int move_count = 0;
    int nodes_left_in_layer = 1;

    if (target < 9) {
      target++;
    } else {
      target = 1;
    }

    qr = new_queue();
    qc = new_queue();

    hr = har->r;
    hc = har->c;

    nodes_in_next_layer = 0;
    marked[hr][hc] = 1;

    enqueue(qr, hr);
    enqueue(qc, hc);

    printf("%d[%d,%d] searching for: %d\n---------------------\n", i + 1, hr, hc, target);
    while (!is_empty(qr)) {
      int r = dequeue(qr);
      int c = dequeue(qc);

      if (map[r][c] - '0' == target) {
        found = true;
        printf("\n********************\nfound at %dx%d in layer %d\n", r,c, move_count);
        printf("proof: %c\n*********************\n", map[r][c]);
        break;
      }

      explore_neighbors(r, c, target, marked, map);
      nodes_left_in_layer--;

      if (nodes_left_in_layer == 0) {
        nodes_left_in_layer = nodes_in_next_layer;
        nodes_in_next_layer = 0;
        move_count++;
      }

    }
    printf("\n---------------------\n\n\n");

    if (found == true) {
      i += acc;
      acc = 0;
    }

    acc++;

    if (target == 1) {
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
