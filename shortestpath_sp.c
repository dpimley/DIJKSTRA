#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include "shortestpath_sp.h"

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
  fclose(inf_q);

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
  int * pred_node = malloc(vertices * sizeof(int));
  heap_head * head = malloc(sizeof(heap_head));
  head->size = 0;
  head->min_heap = malloc(vertices * sizeof(int));
  for (i = 0; i < num_queries; i++){
    fscanf(infile, "%d %d", &q_start, &q_end);
    dijkstra(adj_list, vertices, edges, q_start, q_end, pred_node, head);
  }
  free(pred_node);
  return;
}

void ins_head(l_node * * head, int node){

  l_node * ins_node = malloc(sizeof(l_node));  
  ins_node->node = node;
  ins_node->next = (*head);
  (*head) = ins_node;
  return;
}

void dijkstra(vertice_h * adj_list, int vertices, int edges, int q_start, int q_end, int * pred_node, heap_head * head){
  int i;

  head->size = 0;
  adj_list[q_start].dist = 0;

  for (i = 0; i < vertices; i++){
    if (i != q_start)
      adj_list[i].dist = INT_MAX;
    pred_node[i] = -1;
    int ins_node = i;
    insert_heap(head, adj_list,  ins_node);
  }

  int heap_idx;

  while (head->size != 0){
    int min_index = remove_min(head, adj_list);
   
    if (min_index == q_end){
      printf("%d\n", adj_list[q_end].dist);
      print_reverse(pred_node, min_index);
      printf("\n");
      return;
    }
        
    l_node * temp = adj_list[min_index].head_l;

    while (temp != NULL){
      if (adj_list[temp->node].dist > adj_list[min_index].dist + weighted_distance(adj_list, min_index, temp->node)){
        adj_list[temp->node].dist = adj_list[min_index].dist + weighted_distance(adj_list, min_index, temp->node);
        pred_node[temp->node] = min_index;
        heap_idx = adj_list[temp->node].heap_idx;
        head->min_heap[heap_idx] = temp->node;
        sift_up(head, adj_list, heap_idx);
      }
      temp = temp->next;
    }
  }      
  return;
}

int remove_min(heap_head * head, vertice_h * adj_list){
  if (head->size > 0){
    int min_node = head->min_heap[0];
    int last_element = head->min_heap[head->size - 1];
    head->min_heap[0] = last_element;
    adj_list[head->min_heap[0]].heap_idx = 0;
    head->size--;
    sift_down(head, adj_list, head->size, 0);
    return min_node;
  }
  return 0;
}

void sift_down(heap_head * head, vertice_h * adj_list, int size, int root){
  int left_idx = (2 * root) + 1;
  int right_idx = (2 * root) + 2;
  int min_node;
  if (left_idx >= size && right_idx >= size){
    return;
  }
  if (left_idx < size && adj_list[head->min_heap[root]].dist <= adj_list[head->min_heap[left_idx]].dist){
    if (right_idx < size && adj_list[head->min_heap[root]].dist <= adj_list[head->min_heap[right_idx]].dist){
      return;
    }
  }
  if (right_idx < size && adj_list[head->min_heap[right_idx]].dist <= adj_list[head->min_heap[left_idx]].dist){
    min_node = head->min_heap[right_idx];
    head->min_heap[right_idx] = head->min_heap[root];
    head->min_heap[root] = min_node;
    adj_list[head->min_heap[right_idx]].heap_idx = right_idx;
    adj_list[head->min_heap[root]].heap_idx = root;
    sift_down(head, adj_list, size, right_idx);
  }
  else{
    min_node = head->min_heap[left_idx];
    head->min_heap[left_idx] = head->min_heap[root];
    head->min_heap[root] = min_node;
    adj_list[head->min_heap[left_idx]].heap_idx = left_idx;
    adj_list[head->min_heap[root]].heap_idx = root;
    sift_down(head, adj_list, size, left_idx);
  }
  return;
}

void insert_heap(heap_head * head, vertice_h * adj_list, int ins_node){
  if (head->size == 0){
    head->min_heap[0] = ins_node;
    head->size++;
  }
  else{
    head->size++;
    head->min_heap[head->size - 1] = ins_node;
    int child_idx = head->size - 1;
    int parent_idx = (child_idx - 1) / 2;
    while (adj_list[head->min_heap[parent_idx]].dist > adj_list[ins_node].dist && child_idx > 0){
      head->min_heap[child_idx] = head->min_heap[parent_idx];
      adj_list[head->min_heap[child_idx]].heap_idx = child_idx;
      child_idx = parent_idx;
      parent_idx = (child_idx - 1) / 2;
    }
    head->min_heap[child_idx] = ins_node;
    adj_list[ins_node].heap_idx = child_idx;
  }
  return;
}

void sift_up(heap_head * head, vertice_h * adj_list, int index){
  int child_idx = index;
  if (child_idx == 0){
    return;
  }
  int parent_idx = (child_idx - 1) / 2;
  int ins_node = head->min_heap[index];
  while (adj_list[head->min_heap[parent_idx]].dist > adj_list[ins_node].dist && child_idx > 0){
    head->min_heap[child_idx] = head->min_heap[parent_idx];
    adj_list[head->min_heap[child_idx]].heap_idx = child_idx;
    child_idx = parent_idx;
    parent_idx = (child_idx - 1) / 2;
  }
  head->min_heap[child_idx] = ins_node;
  adj_list[head->min_heap[child_idx]].heap_idx = child_idx;
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

int weighted_distance(vertice_h * adj_list, int u, int v){
  return (int)sqrt((((adj_list[u].v_x - adj_list[v].v_x) * (adj_list[u].v_x - adj_list[v].v_x)) + ((adj_list[u].v_y - adj_list[v].v_y) * (adj_list[u].v_y - adj_list[v].v_y))));
}

float f_sqrt(int number){
  float x2 = number * 0.5f;
  float y = number;
  long i = *(long*)&y;
  i = 0x5f3759df - (i >> 1);
  y = *(float*)&i;
  y = y * (1.5f - (x2 * y * y));
  y = y * (1.5f - (x2 * y * y));
  return (1 / y);
}
  
