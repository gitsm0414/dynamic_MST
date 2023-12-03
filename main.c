#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    //always node1 < node2
    int node1;
    int node2;
    int weight;
}edge_t;

typedef struct{
    edge_t *edges;
    int size;
    int capacity;
}adjacent_t;

typedef struct{
    int parent;
    int size;
}set_t;

void swap_edges(edge_t* e1, edge_t* e2);

int e_partition(edge_t* arr, int s, int e);

void quick_sort(edge_t* arr, int s, int e);

void init_adj(adjacent_t* adj, int init_capacity);

void insert_edge(adjacent_t* adj, int n1, int n2, int w);

void delete_edge(adjacent_t* adj, int n1, int n2);

void change_weight(adjacent_t* adj, int n1, int n2, int w);

void make_set(set_t *sets, int n);

int find_set(set_t *sets, int i);

void union_set(set_t *sets, int i, int j);

int findMST(adjacent_t* adj, int n);

//the number of edges
int num_edges;

int main()
{
    num_edges = 0;
    int n;

    FILE *infile, *outfile;
    char line[50];

    //opening input file
    infile = fopen("mst.in", "r");
    if (infile == NULL) {
        printf("Input file opening error.\n");
        return 1;
    }

    //opening output file
    outfile = fopen("mst.out", "a");
    if(outfile == NULL){
        printf("Output file opening error.\n");
        return 1;
    }

    //reading 'n'
    if (fgets(line, sizeof(line), infile)) {
        sscanf(line, "%d", &n);

    }

    //initialize adjacency lists
    int initial_capacity = 1;
    adjacent_t* adj_lists = (adjacent_t*)malloc(n*sizeof(adjacent_t));
    for(int i=0; i<n; i++){
        init_adj(&adj_lists[i], initial_capacity);
    }

    //getting one line of input file and call appropriate functions
    char cmd[20];
    int arg1, arg2, arg3, ssc, result;
    while(fgets(line, sizeof(line), infile)){

        if((ssc = sscanf(line, "%s %d %d %d", cmd, &arg1, &arg2, &arg3)) > 0){

            if(strcmp(cmd, "findMST")==0){
                    result = findMST(adj_lists, n);
                    if(result == -1){
                        fprintf(outfile, "Disconnected\n");
                    }
                    else{
                        fprintf(outfile, "%d\n", result);
                    }

            }else if(strcmp(cmd, "insertEdge")==0 && ssc==4){
                    //because the graph is undirected, arg1 and arg2 should be different
                    if(arg1==arg2) continue;

                    if(arg1 < arg2 && arg1 > 0 && arg2 > 0){
                            insert_edge(adj_lists, arg1-1, arg2-1, arg3);
                    }else if(arg1 > arg2 && arg1 > 0 && arg2 > 0){
                            insert_edge(adj_lists, arg2-1, arg1-1, arg3);
                    }


            }else if(strcmp(cmd, "deleteEdge")==0 && ssc==3){
                    //because the graph is undirected, arg1 and arg2 should be different
                    if(arg1==arg2) continue;

                    if(arg1 < arg2 && arg1 > 0 && arg2 > 0){
                            delete_edge(adj_lists, arg1-1, arg2-1);
                    }else if(arg1 > arg2 && arg1 > 0 && arg2 > 0){
                            delete_edge(adj_lists, arg2-1, arg1-1);
                    }


            }else if(strcmp(cmd, "changeWeight")==0 && ssc==4){
                    //because the graph is undirected, arg1 and arg2 should be different
                    if(arg1==arg2) continue;

                    if(arg1 < arg2 && arg1 > 0 && arg2 > 0){
                            change_weight(adj_lists, arg1-1, arg2-1, arg3);
                    }else if(arg1 > arg2 && arg1 > 0 && arg2 > 0){
                            change_weight(adj_lists, arg2-1, arg1-1, arg3);
                    }

            }
        }
    }

    for(int i=0; i<n; i++){
        free(adj_lists[i].edges);
    }
    free(adj_lists);

    fclose(infile);
    fclose(outfile);

    return 0;
}

void swap_edges(edge_t* e1, edge_t* e2){
    int temp;

    temp = e1->node1;
    e1->node1 = e2->node1;
    e2->node1 = temp;

    temp = e1->node2;
    e1->node2 = e2->node2;
    e2->node2 = temp;

    temp = e1->weight;
    e1->weight = e2->weight;
    e2->weight = temp;
}

//using Hoare's partitioning algorithm
int e_partition(edge_t* arr, int s, int e){
    int pivot_w = arr[s].weight;
    int i = s-1, j = e+1;

    while(1){
        do{
            i++;
        }while(arr[i].weight < pivot_w);

        do{
            j--;
        }while(arr[j].weight > pivot_w);

        if(i>=j){
            return j;
        }

        swap_edges(&arr[i], &arr[j]);
    }
}

void quick_sort(edge_t* arr, int s, int e){
    if(s >= e){
        return;
    }
    else{
        int pivot = e_partition(arr, s, e);

        quick_sort(arr, s, pivot-1);
        quick_sort(arr, pivot+1, e);
    }
}

void init_adj(adjacent_t* adj, int init_capacity){
    adj->edges = (edge_t*)malloc(init_capacity*sizeof(edge_t));
    adj->size = 0;
    adj->capacity = init_capacity;
}

void insert_edge(adjacent_t* adj, int n1, int n2, int w){
    for(int i=0; i<adj[n1].size; i++){
        if(adj[n1].edges[i].node2 == n2){
            return;
        }
    }

    if(adj[n1].size == adj[n1].capacity){
        adj[n1].capacity *= 2;
        adj[n1].edges = (edge_t*)realloc(adj[n1].edges, adj[n1].capacity*sizeof(edge_t));
    }

    adj[n1].edges[adj[n1].size].node1 = n1;
    adj[n1].edges[adj[n1].size].node2 = n2;
    adj[n1].edges[adj[n1].size].weight = w;
    adj[n1].size++;
    num_edges++;
}

void delete_edge(adjacent_t* adj, int n1, int n2){
    for(int i=0; i<adj[n1].size; i++){
        if(adj[n1].edges[i].node2 == n2){
                if(i!=adj[n1].size-1){
                        swap_edges(&(adj[n1].edges[i]), &(adj[n1].edges[adj[n1].size - 1]));
                }
                adj[n1].size--;
                num_edges--;
                return;
        }
    }
    return;
}

void change_weight(adjacent_t* adj, int n1, int n2, int w){
    for(int i=0; i<adj[n1].size; i++){
        if(adj[n1].edges[i].node2 == n2){
                adj[n1].edges[i].weight = w;
                return;
        }
    }
    return;
}

void make_set(set_t *sets, int n){
    for(int i=0; i<n; i++){
        sets[i].parent = i;
        sets[i].size = 1;
    }
}

int find_set(set_t *sets, int i){
    if(sets[i].parent != i){
        sets[i].parent = find_set(sets, sets[i].parent);
    }
    return sets[i].parent;
}

//set1, set2 is representer of each set
void union_set(set_t *sets, int set1, int set2){
    if(sets[set1].size < sets[set2].size){
        sets[set1].parent = set2;
        sets[set2].size += sets[set1].size;
    }
    else{
        sets[set2].parent = set1;
        sets[set1].size += sets[set2].size;
    }

}

//if MST is connected, return total sum of weights, if disconnected, return -1
int findMST(adjacent_t* adj, int n){
    if(num_edges < n-1) return -1;

    int sum = 0;
    edge_t* edge_arr = (edge_t*)malloc(num_edges*sizeof(edge_t));
    set_t* sets = (set_t*)malloc(n*sizeof(set_t));

    int idx = 0;
    for(int i=0; i<n; i++){
        for(int j=0; j<adj[i].size; j++){
            edge_arr[idx].node1 = adj[i].edges[j].node1;
            edge_arr[idx].node2 = adj[i].edges[j].node2;
            edge_arr[idx].weight = adj[i].edges[j].weight;
            idx++;
        }
    }

    make_set(sets, n);

    quick_sort(edge_arr, 0, num_edges-1);

    int n1, n2, set1, set2;
    int count = 0;
    for(int i=0; i<num_edges; i++){
        if(count == n-1) break;
        n1 = edge_arr[i].node1;
        n2 = edge_arr[i].node2;

        set1 = find_set(sets, n1);
        set2 = find_set(sets, n2);
        if(set1 != set2){
            sum += edge_arr[i].weight;
            union_set(sets, set1, set2);
            count++;
        }
    }

    free(edge_arr);

    free(sets);

    if(count!=n-1) return -1;

    return sum;
}





