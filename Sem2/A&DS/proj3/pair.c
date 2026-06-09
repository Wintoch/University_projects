#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Edge{
    int aim;
    int weight;
    struct Edge *next;
}Edge;

typedef struct Graph{
    int numOfVertices;
    Edge **adjacencyList;
    int *colors;
}Graph;

void readData(int numOfVertices, int numOfEdges, Graph* graph){
        int u,v,w;
        graph -> numOfVertices = numOfVertices;
        graph -> adjacencyList = (Edge**)calloc(numOfVertices, sizeof(Edge*));
        graph -> colors = (int*)calloc(numOfVertices, sizeof(int));
        for(int j = 0; j < numOfEdges; j++){
            scanf("%d %d %d", &u, &v ,&w);
            u -= 1;
            v -= 1;
            Edge* newedgeu = (Edge*)malloc(sizeof(Edge));
            newedgeu -> aim = v;
            newedgeu -> weight = w;
            newedgeu -> next = graph -> adjacencyList[u];
            graph -> adjacencyList[u] = newedgeu;

            Edge* newedgev = (Edge*)malloc(sizeof(Edge));
            newedgev -> aim = u;
            newedgev -> weight = w;
            newedgev -> next = graph -> adjacencyList[v];
            graph -> adjacencyList[v] = newedgev;
        }
}

int checkIfBipartiate(Graph* graph){
    int size = graph->numOfVertices;
    int* queue = (int*)malloc(size * sizeof(int));
    int start = 0;
    int end = 0;

    for(int i = 0; i < size; i++){
        if(graph->colors[i] == 0){
            graph->colors[i] = 1;
            queue[end++] = i;
            while(start<end){
                int present = queue[start++];
                Edge* current = graph->adjacencyList[present];
                while (current != NULL)
                {
                    int neighbour = current->aim;
                    if (graph->colors[neighbour] == 0) {
                        graph->colors[neighbour] = (graph->colors[present] == 1) ? 2 : 1;
                        queue[end++] = neighbour;
                    } 
                    else if (graph->colors[neighbour] == graph->colors[present]) {
                        free(queue);
                        return 0;
                    }
                    current = current->next;
                }                
            }
        }
    }
    free(queue);
    return 1;
}

void BipartiateSoultion(Graph* graph){
    int size = graph->numOfVertices;
    int* new_row_id = (int*)calloc(size, sizeof(int));
    int* new_col_id = (int*)calloc(size, sizeof(int));
    int rows = 0, cols = 0;

    for(int i = 0; i < size; i++){
        if(graph->colors[i] == 1){
            new_row_id[i] = rows++;
        } else {
            new_col_id[i] = cols++;
        }
    }

    int N = rows;
    int M = cols;
    int swapped = 0;
    if (N > M) {
        N = cols;
        M = rows;
        swapped = 1;
    }

    int *matchCol = (int*)malloc(M * sizeof(int));
    int *matchRoW = (int*)malloc(N * sizeof(int));
    for(int j = 0; j < M; j++) matchCol[j] = -1;
    for(int i = 0; i < N; i++) matchRoW[i] = -1;

    int** matrix = (int**)malloc(N * sizeof(int*));
    int** originalMatrix = (int**)malloc(N * sizeof(int*));
    int* flat_matrix = (int*)calloc(N * M, sizeof(int));
    int* flat_original = (int*)calloc(N * M, sizeof(int));

    for(int i = 0; i < N; i++){
        matrix[i] = &flat_matrix[i * M];
        originalMatrix[i] = &flat_original[i * M];
    }

    for(int i = 0; i < size; i++){
        if(graph->colors[i] == 1){
            Edge* current = graph->adjacencyList[i];
            while(current != NULL){
                int j = current->aim;
                int r = new_row_id[i];
                int c = new_col_id[j];

                if (swapped) {
                    matrix[c][r] = current->weight;
                    originalMatrix[c][r] = current->weight;
                } else {
                    matrix[r][c] = current->weight;
                    originalMatrix[r][c] = current->weight;
                }
                current = current->next;
            }
        }
    }

    for (int i = 0; i < N; i++){
        int max = 0; 
        for(int j = 0; j < M; j++){
            if(matrix[i][j] > max)  max = matrix[i][j];
        }
        for (int j = 0; j < M; j++){
            matrix[i][j] = max - matrix[i][j];
        }
    }

    if (N == M) {
        int* col_min = (int*)malloc(M * sizeof(int));
        for(int j = 0; j < M; j++) col_min[j] = 2147483647;

        for (int i = 0; i < N; i++){
            for(int j = 0; j < M; j++){
                if(matrix[i][j] < col_min[j]) col_min[j] = matrix[i][j];
            }
        }
        for (int i = 0; i < N; i++){
            for(int j = 0; j < M; j++){
                matrix[i][j] -= col_min[j];
            }
        }
        free(col_min);
    }

    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){
            if(matrix[i][j] == 0 && matchCol[j] == -1 && matchRoW[i] == -1){
                matchCol[j] = i;
                matchRoW[i] = j;
                break;
            }
        }
    }

    int* u = (int*)calloc(N, sizeof(int));    
    int* v = (int*)calloc(M, sizeof(int));
    int* slack = (int*)calloc(M, sizeof(int));
    int* visitedCol = (int*)calloc(M, sizeof(int));
    int* parent = (int*)calloc(M, sizeof(int));
    int* unvisited = (int*)malloc(M * sizeof(int));
    int* visited_list = (int*)malloc(M * sizeof(int));

    for(int i = 0; i < N; i++){
        if (matchRoW[i] != -1) continue;

        int num_unvisited = M;
        int num_visited = 0;

        memset(visitedCol, 0, M * sizeof(int));
        for(int j = 0; j < M; j++) slack[j] = 2147483647;
        for(int j = 0; j < M; j++) unvisited[j] = j;

        int currentRow = i;
        int currentCol = -1;

        while(1){
            int minSlack = 2147483647;
            int nextCol = -1;
            int next_unvisited_idx = -1;

            int u_curr = u[currentRow];
            int* row_ptr = matrix[currentRow];

            for (int j = 0; j < num_unvisited; j++){
                int k = unvisited[j];
                int currentSlack = slack[k];
                int price = row_ptr[k] - u_curr - v[k];

                if(price < currentSlack){
                    currentSlack = price;
                    slack[k] = price;
                    parent[k] = currentRow;
                }
                if(currentSlack < minSlack){
                    minSlack = currentSlack;
                    nextCol = k;
                    next_unvisited_idx = j;
                }
            }

            u[i] += minSlack;
            for(int j = 0; j < num_visited; j++){
                int k = visited_list[j];
                v[k] -= minSlack;
                u[matchCol[k]] += minSlack;
            }

            for(int j = 0; j < num_unvisited; j++){
                int k = unvisited[j];
                slack[k] -= minSlack;
            }

            visitedCol[nextCol] = 1;
            visited_list[num_visited++] = nextCol;
            unvisited[next_unvisited_idx] = unvisited[--num_unvisited];

            if(matchCol[nextCol] == -1){
                currentCol = nextCol;
                break;
            }
            currentRow = matchCol[nextCol];
        }

        while(currentCol != -1){
            int r = parent[currentCol];
            int prevCol = matchRoW[r];
            matchCol[currentCol] = r;
            matchRoW[r] = currentCol;
            currentCol = prevCol;
        }
    }

    long long result = 0;
    for(int j = 0; j < M; j++){
        if(matchCol[j] != -1) result += originalMatrix[matchCol[j]][j];
    }

    printf("%lld\n", result);

    free(flat_matrix);
    free(flat_original);
    free(matrix);
    free(originalMatrix);
    free(matchCol);
    free(new_row_id);
    free(new_col_id);
    free(matchRoW);
    free(u);
    free(v);
    free(slack);
    free(visitedCol);
    free(parent);
    free(unvisited);
    free(visited_list);
}

int checkIfUnitGraph(Graph* graph){
    int size = graph -> numOfVertices;
    for(int i = 0; i < size; i++){
        Edge* current = graph -> adjacencyList[i];
        while(current != NULL){
            if(current -> weight != 1){
                return 0;
            }else{
                current = current -> next;
            }
        }
    }
    return 1;
}

void shrink(int current, int next_node, int* parent, int* base, int* match, int* type, int* queue, int* tail, int V){
    int* lca = (int*)calloc(V, sizeof(int));
    int u = base[current];
    int v = base[next_node];
    int* compressed = (int*)calloc(V, sizeof(int));

    while(u != -1){
        lca[u] = 1;
        if(parent[u] == -1) break;
        u = base[parent[u]];
    }

    int root = -1;
    while(v != -1){
        if(lca[v] == 1){
            root = v;
            break;
        }
        v = base[parent[v]];
    }
    free(lca);

    if (base[current] != root) parent[current] = next_node;
    if (base[next_node] != root) parent[next_node] = current;

    int sides[2] = {base[current], base[next_node]};
    for(int i = 0; i < 2; i++){
        int curr = sides[i];
        int q = sides[1 - i];

        while(curr != root){
            int p = match[curr];
            int nextUp = parent[p];

            compressed[base[curr]] = 1;
            compressed[base[p]] = 1;

            if(type[p] == 2){
                type[p] = 1;
                queue[(*tail)++] = p;
            }

            
            parent[curr] = q;
            if(base[nextUp] != root){
                parent[nextUp] = p;
            }

            q = p;
            curr = base[nextUp];
        }
    }

    for(int j = 0; j < V; j++){
        if(compressed[base[j]] == 1){
            base[j] = root;
        }
    }

    free(compressed);
}

int BFS(Graph* graph, int startNode, int* match, int* parent, int* base){
    int V = graph -> numOfVertices;
    int head = 0;
    int tail = 0;
    int* queue = (int*)malloc(V * sizeof(int));
    int* type = (int*)calloc(V, sizeof(int));

    queue[tail++] = startNode;
    type[startNode] = 1;

    //0 - nieodwiedzony, 1 - wewnetrzny, 2 - zewnetrzny
    while(head < tail){
        int current = queue[head++];

        Edge* neighbor = graph->adjacencyList[current];
        while (neighbor != NULL) {
            int next_node = neighbor->aim;
            
            if (base[current] == base[next_node]) {
                neighbor = neighbor->next;
                continue;
            }

            if (type[next_node] == 0) {
                parent[next_node] = current;

                if(match[next_node] == -1){
                    int curr = next_node;

                    while(curr != -1){
                        int p = parent[curr];
                        int temp = match[p];

                        match[curr] = p;
                        match[p] = curr;

                        curr = temp;
                    }

                    free(queue);
                    free(type);
                    return 1;
                }else{
                    type[next_node] = 2;

                    int partner = match[next_node];
                    type[partner] = 1;
                    parent[partner] = next_node;

                    queue[tail++] = partner;
                }
            }else if (type[next_node] == 1 && parent[current] != next_node) {
                shrink(current, next_node, parent, base, match, type, queue, &tail, V);
            }
            
            neighbor = neighbor->next;
        }
    }

    free(queue);
    free(type);
    return 0;
}

void UnitSolution(Graph* graph){
    int V = graph -> numOfVertices;

    int* match = (int*)malloc(V * sizeof(int));
    for(int i = 0; i < V; i++) match[i] = -1;
    int* parent = (int*)malloc(V * sizeof(int));
    int* base = (int*)malloc(V * sizeof(int));

    for(int i = 0; i < V; i++){
        if(match[i] == -1){
            for(int j  = 0; j < V; j++){
                parent[j] = -1;
                base[j] = j;
            }
            BFS(graph, i, match, parent, base);
        }
    }
    long long result = 0;
    for(int i = 0; i < V; i++){
        if(match[i] > i){
            result += 1;
        }
    }
    printf("%lld\n", result);

    free(match);
    free(parent);
    free(base);
}

void BFSBrut(Graph* graph, int v, long long current_sum, int* used, long long* best){
    if(v == graph -> numOfVertices){
        if(current_sum > *best){
            *best = current_sum;
        }
        return;
    }

    if(used[v] == 1){
        BFSBrut(graph, v + 1, current_sum, used, best);
        return;
    }

    BFSBrut(graph, v + 1, current_sum, used, best);

    Edge* current = graph -> adjacencyList[v];
    while(current != NULL){
        int aim = current->aim;
        if(used[aim] == 0 && aim > v){
            used[v] = 1;
            used[aim] = 1;

            BFSBrut(graph, v + 1, current_sum + current->weight, used, best);

            used[v] = 0;
            used[aim] = 0;
        }

        current = current -> next;
    }
}

void WeightedBrutSolution(Graph* graph){
    long long best = 0;
    int* used = (int*)calloc(graph->numOfVertices, sizeof(int));
    BFSBrut(graph, 0, 0, used, &best);
    printf("%lld\n", best);
    free(used);
}

int main(){
    int numOfOperations;
    scanf("%d", &numOfOperations);
    int numOfVertices, numOfEdges;

    for (int i = 0; i < numOfOperations; i++){
        scanf("%d %d", &numOfVertices, &numOfEdges);
        Graph* graph = (Graph*)malloc(sizeof(Graph));
        readData(numOfVertices, numOfEdges, graph);
        int bipartiate = checkIfBipartiate(graph);

        if(bipartiate){
            BipartiateSoultion(graph);
        }else{
            int unit = checkIfUnitGraph(graph);
            if(unit){
                UnitSolution(graph);
            }else{
                WeightedBrutSolution(graph);
            }
        }
        
        free(graph);
    }

    return 0;
}