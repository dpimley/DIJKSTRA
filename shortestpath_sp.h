#ifndef SHORTEST_H
#define SHORTEST_H

typedef struct l_node{
  int node;
  struct l_node * next;
}l_node;

typedef struct vertice_h{
  unsigned short v_x;
  unsigned short v_y;
  int dist;
  int heap_idx;
  l_node * head_l;
}vertice_h;

typedef struct heap_head{
  int size;
  int * min_heap;
}heap_head;

vertice_h * readfile(FILE * infile, int vertices, int edges);

void run_queries(FILE * infile, vertice_h * adj_list, int vertices, int edges);

void ins_head(l_node * * head, int node);

void dijkstra(vertice_h * adj_list, int vertices, int edges, int q_start, int q_end, int * pred_node, heap_head * head);

int remove_min(heap_head * head, vertice_h * adj_list);

void sift_down(heap_head * head, vertice_h * adj_list, int size, int root);

void insert_heap(heap_head * head, vertice_h * adj_list, int ins_node);

void sift_up(heap_head * head, vertice_h * adj_list, int index);

void print_reverse(int * prev, int i);

int weighted_distance(vertice_h * adj_list, int u, int v);

float f_sqrt(int x);

#endif
