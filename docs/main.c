#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define MAX_VERTICES 1000

typedef struct Aresta {
    int destino;
    double peso;
    struct Aresta* prox;
} Aresta;

typedef struct GrafoLista {
    Aresta* adj[MAX_VERTICES];
    int numVertices;
} GrafoLista;

GrafoLista* criarGrafoLista(int v) {
    GrafoLista* g = (GrafoLista*) malloc(sizeof(GrafoLista));
    g->numVertices = v;
    for(int i = 0; i < v; i++) g->adj[i] = NULL;
    return g;
}

void adicionarArestaLista(GrafoLista* g, int u, int v, double peso) {
    Aresta* nova = (Aresta*) malloc(sizeof(Aresta));
    nova->destino = v;
    nova->peso = peso;
    nova->prox = g->adj[u];
    g->adj[u] = nova;
    
    Aresta* nova2 = (Aresta*) malloc(sizeof(Aresta));
    nova2->destino = u;
    nova2->peso = peso;
    nova2->prox = g->adj[v];
    g->adj[v] = nova2;
}

double obterTempoAtualMS() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000.0) + (tv.tv_usec / 1000.0);
}

void carregarDataset(GrafoLista* gl) {
    FILE* file = fopen("gerador_mock_brasil.py", "r");
    if(!file) {
        printf("Erro: Ficheiro gerador_mock_brasil.py nao encontrado.\n");
        exit(1);
    }
    
    char buffer[100];
    fgets(buffer, sizeof(buffer), file);
    
    int u, v;
    double peso;
    while(fscanf(file, "%d,%d,%lf", &u, &v, &peso) == 3) {
        adicionarArestaLista(gl, u, v, peso);
    }
    fclose(file);
}

void bfs(GrafoLista* g, int start, int* visitado, int ignorar) {
    int fila[MAX_VERTICES];
    int inicio = 0, fim = 0;

    fila[fim++] = start;
    visitado[start] = 1;

    while (inicio < fim) {
        int u = fila[inicio++];
        Aresta* atual = g->adj[u];
        
        while (atual != NULL) {
            int v = atual->destino;
            if (!visitado[v] && v != ignorar) {
                visitado[v] = 1;
                fila[fim++] = v;
            }
            atual = atual->prox;
        }
    }
}

int contarComponentesConexos(GrafoLista* g, int ignorar) {
    int visitado[MAX_VERTICES] = {0};
    int componentes = 0;

    for (int i = 0; i < g->numVertices; i++) {
        if (i == ignorar) continue;
        
        if (!visitado[i]) {
            componentes++;
            bfs(g, i, visitado, ignorar);
        }
    }
    
    return componentes;
}

void encontrarArticulacoesBFS(GrafoLista* g) {
    double tInicio = obterTempoAtualMS();

    int componentesIniciais = contarComponentesConexos(g, -1);
    int count = 0;

    printf("\n--- RESULTADO ---\n");
    printf("Quais roteadores partem a rede se cairem?\n\n");

    for (int i = 0; i < g->numVertices; i++) {
        int componentesAtuais = contarComponentesConexos(g, i);
        
        if (componentesAtuais > componentesIniciais) {
            printf(" -> [ALERTA] Roteador ID: %d e um Vertice de Articulacao.\n", i);
            count++;
        }
    }

    double tFim = obterTempoAtualMS();

    printf("\nResumo da Topologia:\n");
    printf("Total de gargalos criticos encontrados: %d\n", count);
    printf("Tempo de execucao do Algoritmo BFS: %.4f ms\n", tFim - tInicio);
    printf("------------------------\n\n");
}

int main() {
    printf("=== PROJETO DE GRAFOS: REDE ===\n");
    printf("Estrutura utilizada: LISTA DE ADJACENCIA (Busca em Largura - BFS)\n\n");
    
    GrafoLista* gl = criarGrafoLista(MAX_VERTICES);
    
    double tInicio = obterTempoAtualMS();
    carregarDataset(gl);
    double tFim = obterTempoAtualMS();
    
    printf("Dataset carregado com sucesso na memoria em %.2f ms.\n", tFim - tInicio);
    
    encontrarArticulacoesBFS(gl);
    
    return 0;
}