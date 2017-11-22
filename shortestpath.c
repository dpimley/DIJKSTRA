#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include "shortestpath.h"

int main(int argc, char * * argv){
 
  FILE * inf = fopen(argv[1], "r");

  if (inf == NULL){
    printf("ERROR: Input file pointer initialized to NULL.\n");
    return EXIT_FAILURE;
  }

  int vertices, edges;

  fscanf(inf, "%d %d", &vertices, &edges);

  vertice_h * adj_list = readfile(inf, vertices, edges);

  int q_start = 6;

  int q_end = 23; 

  dijkstra(adj_list, vertices, edges, q_start, q_end);

  fclose(inf);  

  return EXIT_SUCCESS;
}

vertice_h * readfile(FILE * infile, int vertices, int edges){

  vertice_h * adj_list = malloc(vertices * sizeof(vertice_h));

  int pl_v, pl_x, pl_y;

  int i;

  for (i = 0; i < vertices; i++){
    fscanf(infile, "%d %d %d", &pl_v, &pl_x, &pl_y);
    adj_list[pl_v].v_x = pl_x;
    adj_list[pl_v].v_y = pl_y;
  }

  int cur_v, adj_v;

  for (i = 0; i < edges; i++){
    fscanf(infile, "%d %d", &cur_v, &adj_v);
    ins_head(&(adj_list[cur_v].head_l), adj_v);
    ins_head(&(adj_list[adj_v].head_l), cur_v);
  }

  return adj_list;
}

void ins_head(l_node * * head, int node){

  l_node * ins_node = malloc(sizeof(l_node));  
  ins_node->node = node;
  ins_node->next = (*head);
  (*head) = ins_node;
  return;
}

void dijkstra(vertice_h * adj_list, int vertices, int edges, int q_start, int q_end){
  int i;
  int * pred_node = malloc(vertices * sizeof(int));

  for (i = 0; i < vertices; i++){
    adj_list[i].dist = INT_MAX;
    pred_node[i] = -1;
  }

  adj_list[q_start].dist = 0;

  for (i = 0; i < vertices; i++){
    int min_index = find_min_index(adj_list, vertices);

    l_node * temp = adj_list[min_index].head_l;

    while (temp != NULL){
      if (adj_list[temp->node].dist > adj_list[min_index].dist + weighted_distance(adj_list, min_index, temp->node)){
        adj_list[temp->node].dist = adj_list[min_index].dist + weighted_distance(adj_list, min_index, temp->node);
        pred_node[temp->node] = min_index;
      }
      temp = temp->next;
    }
  }      

  for (i = 0; i < vertices; i++){
    printf("%d: %d\n", i, adj_list[i].dist);
  }
  return;
}

heap_node * remove_min(heap_head * head){
  if (head->size){
    heap_node * min_node = head->min_heap[0];
    heap_node * last_element = head->min_heap[head->size - 1];
    head->min_heap[head->size - 1] = NULL;
    head->min_heap[0] = last_element;
    head->size--;
    sift_down(head-min_heap, head->size, 0);
    return min_node;
  }
  return 0;
}

void sift_down(heap_node * head, int size, int root){
  int left_idx = (2 * root) + 1;
  int right_idx = (2 * root) + 2;
  heap_node * min_node = NULL;
  if (left_idx >= size && right _idx >= size){
    return;
  }
  if (left_idx < size && head[root]->dist <= head[left_idx]->dist){
    if (right idx < size && head[root]->dist <= head[right_idx]->dist){
      return;
    }
  }
  if (right_idx < size && head[right_idx]->dist <= head[left_idx]->dist){
    min_node = head[right_idx];
    head[right_idx] = head[root];
    head[root] = min_node;
    sift_down(head, size, right_idx);
  }
  else{
    min_node = head[left_idx];
    head[left_idx] = head[root];
    head[root] = min_node;
    sfit_down(head, size, left_idx);
  }
  return;
}

void insert_heap(heap_head * head, heap_node * ins_node){
  if (!head->size){
    head->min_heap[0] = ins_node;
    head->size++;
  }
  else{
    head->size++;
    head->min_heap[head->size - 1] = ins_node;
    int child_idx = head->size - 1;
    int parent_idx = (child_idx - 1) / 2;
    while (head->min_heap[parent_idx]->dist > ins_node->dist && child_idx > 0){
      head->min_heap[child_idx] = head->min_heap[parent_idx];
      child_idx = parent_idx;
      parent_idx = (child_idx - 1) / 2;
    }
    head->mind_heap[child_idx] = ins_node;
  }
  return;
}

int weighted_distance(vertice_h * adj_list, int u, int v){
  return (int)sqrt(((adj_list[u].v_x - adj_list[v].v_x) * (adj_list[u].v_x - adj_list[v].v_x)) + ((adj_list[u].v_y - adj_list[v].v_y) * (adj_list[u].v_y - adj_list[v].v_y)));
}