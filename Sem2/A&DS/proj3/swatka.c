typedef struct Edge{
    int aim;
    int weight;
    struct Edge *next;
}Edge;

typedef struct Graph{
    int numOfVertices;
    Edge **adjacencyList;
}Graph;

int main(){
    int numOfOperations;
    scanf("%d", &numOfOperations);
    int numOfVertices, numOfEdges;
    int u,v,w;

    for (int i = 0; i < numOfOperations; i++){
        scanf("%d %d", &numOfVertices, &numOfEdges);
        Graph* graph = (Graph*)malloc(sizeof(Graph));
        graph -> numOfVertices = numOfVertices;
        graph -> adjacencyList = (Edge**)calloc(numOfVertices, sizeof(Edge*));
        for(int j = 0; j < numOfEdges; j++){
            scanf("%d %d %d", &u, &v ,&w);
            u -= 1;
            v -= 1;
            Edge* newedgeu = (Edge*)malloc(u * sizeof(Edge));
            newedgeu -> aim = v;
            newedgeu -> weight = w;
            newedgeu -> next = graph -> adjacencyList[u];
            graph -> adjacencyList[u] = newedgeu;

            Edge* newedgev = (Edge*)malloc(u * sizeof(Edge));
            newedgev -> aim = v;
            newedgev -> weight = w;
            newedgev -> next = graph -> adjacencyList[v];
            graph -> adjacencyList[v] = newedgev;
        }

        

    }

    return 0;
}