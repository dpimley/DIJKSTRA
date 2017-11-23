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

  FILE * inf_q = fopen(argv[2], "r");

  if (inf_q == NULL){
    printf("ERROR: Input file pointer initialized to NULL.\n");
    return EXIT_FAILURE;
  }

  vertice_h * adj_list = readfile(inf, vertices, edges);

  run_queries(inf_q, adj_list, vertices, edges);

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

void run_queries(FILE * infile, vertice_h * adj_list, int vertices, int edges){
  int num_queries, q_start, q_end;
  fscanf(infile, "%d", &num_queries);
  int i;
  for (i = 0; i < num_queries; i++){
    fscanf(infile, "%d %d", &q_start, &q_end);
    dijkstra(adj_list, vertices, edges, q_start, q_end);
  }
  return;
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

  heap_head * head = malloc(sizeof(heap_head));
  head->size = 0;
  head->min_heap = malloc(vertices * sizeof(heap_node *));

  for (i = 0; i < vertices; i++){
    adj_list[i].dist = INT_MAX;
    pred_node[i] = -1;
    heap_node * ins_node = malloc(sizeof(heap_node));
    ins_node->dist = INT_MAX;
    ins_node->index = i;
    if (i == q_start){
      ins_node->dist = 0;
      ins_node->index = q_start;
    }
    insert_heap(head, ins_node);
  }

  adj_list[q_start].dist = 0;

  while (head->size != 0){
    int min_index = remove_min(head);

    if (min_index == q_end){
      printf("%d\n", adj_list[q_end].dist);
      print_reverse(pred_node, min_index);
      printf("\n");
      //free_adj_list(adj_list, vertices);
      //free_heap(head, vertices);
      return;
    }
        
    l_node * temp = adj_list[min_index].head_l;

    while (temp != NULL){
      if (adj_list[temp->node].dist > adj_list[min_index].dist + weighted_distance(adj_list, min_index, temp->node)){
        adj_list[temp->node].dist = adj_list[min_index].dist + weighted_distance(adj_list, min_index, temp->node);
        pred_node[temp->node] = min_index;
        int heap_idx = find_heap_idx(head->min_heap, temp->node);
        head->min_heap[heap_idx]->dist = adj_list[temp->node].dist;
        sift_up(head, heap_idx);
      }
      temp = temp->next;
    }
  }      
  return;
}

int remove_min(heap_head * head){
  if (head->size > 0){
    heap_node * min_node = head->min_heap[0];
    heap_node * last_element = head->min_heap[head->size - 1];
    head->min_heap[0] = last_element;
    head->size--;
    sift_down(head->min_heap, head->size, 0);
    return min_node->index;
  }
  return 0;
}

void sift_down(heap_node * * head, int size, int root){
  int left_idx = (2 * root) + 1;
  int right_idx = (2 * root) + 2;
  heap_node * min_node = NULL;
  if (left_idx >= size && right_idx >= size){
    return;
  }
  if (left_idx < size && head[root]->dist <= head[left_idx]->dist){
    if (right_idx < size && head[root]->dist <= head[right_idx]->dist){
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
    sift_down(head, size, left_idx);
  }
  return;
}

void insert_heap(heap_head * head, heap_node * ins_node){
  if (head->size == 0){
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
    head->min_heap[child_idx] = ins_node;
  }
  return;
}

int find_heap_idx(heap_node * * head, int index){
  int i;
  while (head[i]->index != index){
    i++;
  }
  return i;
}

void sift_up(heap_head * head, int index){
  int child_idx = index;
  if (child_idx == 0){
    return;
  }
  int parent_idx = (child_idx - 1) / 2;
  heap_node * ins_node = head->min_heap[index];
  while (head->min_heap[parent_idx]->dist > ins_node->dist && child_idx > 0){
    head->min_heap[child_idx] = head->min_heap[parent_idx];
    child_idx = parent_idx;
    if (child_idx == 0){
      break;
    }
    parent_idx = (child_idx - 1) / 2;
  }
  head->min_heap[child_idx] = ins_node;
  return;
}

void print_reverse(int * prev, int i){
  if (prev[i] == -1){
    printf("%d ", i);
    return;
  }

  print_reverse(prev, prev[i]);
  printf("%d ", i);
  return;
}

void free_list(l_node * head){
  while (head != NULL){
    l_node * temp = head;
    head = head->next;
    free(temp);
  }
  return;
}

void free_adj_list(vertice_h * adj_list, int vertices){
  int i;
  for (i = 0; i < vertices; i++){
    free_list((adj_list[i]).head_l);
  }
  free(adj_list);
  return;
}

void free_heap(heap_head * head, int vertices){
  int i;
  for (i = 0; i < vertices; i++){
    free(head->min_heap[i]);
  }
  free(head->min_heap);
  free(head);
}

int weighted_distance(vertice_h * adj_list, int u, int v){
  return (int)sqrt(((adj_list[u].v_x - adj_list[v].v_x) * (adj_list[u].v_x - adj_list[v].v_x)) + ((adj_list[u].v_y - adj_list[v].v_y) * (adj_list[u].v_y - adj_list[v].v_y)));
}
