#ifndef SHORTEST_H
#define SHORTEST_H

typedef struct l_node{
  int node;
  struct l_node * next;
}l_node;

typedef struct vertice_h{
  int v_x;
  int v_y;
  int dist;
  l_node * head_l;
}vertice_h;

typedef struct heap_node{
  int index;
  int dist;
}heap_node;

typedef struct heap_head{
  int size;
  heap_node * min_heap;
}heap_head;

vertice_h * readfile(FILE * infile, int vertices, int edges);

void run_queries(FILE * infile, vertice_h * adj_list, int vertices, int edges);

void ins_head(l_node * * head, int node);

void dijkstra(vertice_h * adj_list, int vertices, int edges, int q_start, int q_end, int * pred_node, heap_head * head);

int remove_min(heap_head * head);

void sift_down(heap_node * head, int size, int root);

void insert_heap(heap_head * head, heap_node ins_node);

int find_heap_idx(heap_node * head, int index);

void sift_up(heap_head * head, int index);

void print_reverse(int * prev, int i);

void free_list(l_node * head);

void free_adj_list(vertice_h * adj_list, int vertices);

int weighted_distance(vertice_h * adj_list, int u, int v);

#endif
