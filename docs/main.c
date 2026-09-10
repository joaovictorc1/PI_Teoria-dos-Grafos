#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>

#define MAX_VERTICES 1000

// ==========================================
// ESTRUTURAS DE DADOS (Gerado com IA)
// ==========================================

// Para Lista de Adjacência
typedef struct Aresta {
    int destino;
    double peso;
    struct Aresta* prox;
} Aresta;

typedef struct GrafoLista {
    Aresta* adj[MAX_VERTICES];
    int numVertices;
} GrafoLista;

// Para Matriz de Adjacência
typedef struct GrafoMatriz {
    double mat[MAX_VERTICES][MAX_VERTICES];
    int numVertices;
} GrafoMatriz;


// ==========================================
// FUNÇÕES DE INICIALIZAÇÃO
// ==========================================

GrafoLista* criarGrafoLista(int v) {
    GrafoLista* g = (GrafoLista*) malloc(sizeof(GrafoLista));
    g->numVertices = v;
    for(int i = 0; i < v; i++) g->adj[i] = NULL;
    return g;
}

GrafoMatriz* criarGrafoMatriz(int v) {
    GrafoMatriz* g = (GrafoMatriz*) malloc(sizeof(GrafoMatriz));
    g->numVertices = v;
    for(int i = 0; i < v; i++)
        for(int j = 0; j < v; j++)
            g->mat[i][j] = -1.0; // -1 indica ausência de aresta
    return g;
}

void adicionarArestaLista(GrafoLista* g, int u, int v, double peso) {
    Aresta* nova = (Aresta*) malloc(sizeof(Aresta));
    nova->destino = v;
    nova->peso = peso;
    nova->prox = g->adj[u];
    g->adj[u] = nova;
    
    // Grafo não direcionado
    Aresta* nova2 = (Aresta*) malloc(sizeof(Aresta));
    nova2->destino = u;
    nova2->peso = peso;
    nova2->prox = g->adj[v];
    g->adj[v] = nova2;
}

void adicionarArestaMatriz(GrafoMatriz* g, int u, int v, double peso) {
    g->mat[u][v] = peso;
    g->mat[v][u] = peso;
}

// ==========================================
// FUNÇÕES UTILITÁRIAS (RF03 - LOGS)
// ==========================================

double obterTempoAtualMS() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000.0) + (tv.tv_usec / 1000.0);
}

// ==========================================
// LEITURA DO DATASET (ETL C)
// ==========================================

void carregarDataset(GrafoLista* gl, GrafoMatriz* gm, int usarMatriz) {
    FILE* file = fopen("arestas_br.csv", "r");
    if(!file) {
        printf("Erro ao abrir arestas_br.csv\n");
        exit(1);
    }
    
    char buffer[100];
    fgets(buffer, sizeof(buffer), file); // Pula cabeçalho
    
    int u, v;
    double peso;
    while(fscanf(file, "%d,%d,%lf", &u, &v, &peso) == 3) {
        if (usarMatriz) adicionarArestaMatriz(gm, u, v, peso);
        else adicionarArestaLista(gl, u, v, peso);
    }
    fclose(file);
}

// ==========================================
// ALGORITMOS (STUBS PARA O PROJETO)
// ==========================================

void encontrarArticulacoesTarjan(GrafoLista* g) {
    // Fase I: Implementar Tarjan ou DFS modificado
    printf("Executando busca de Vértices de Articulação...\n");
}

void executarKruskalOuPrim(GrafoMatriz* g) {
    // Fase II: Otimização P
    printf("Executando Árvore Geradora Mínima (Backbone)...\n");
}

void vertexCoverGulosos(GrafoLista* g) {
    // Fase II: NP-Difícil (Posicionamento de Firewalls)
    printf("Executando Heurística Gulosa para Cobertura de Vértices...\n");
}

// ==========================================
// MAIN
// ==========================================

int main() {
    int usarMatriz = 0; // 0 = Lista, 1 = Matriz (RF02)
    
    printf("=== PROJETO DE GRAFOS: REDE ISP BRASIL ===\n");
    printf("Modo selecionado: %s\n", usarMatriz ? "MATRIZ DE ADJACENCIA" : "LISTA DE ADJACENCIA");
    
    GrafoLista* gl = NULL;
    GrafoMatriz* gm = NULL;
    
    if (usarMatriz) gm = criarGrafoMatriz(MAX_VERTICES);
    else gl = criarGrafoLista(MAX_VERTICES);
    
    double tInicio = obterTempoAtualMS();
    carregarDataset(gl, gm, usarMatriz);
    double tFim = obterTempoAtualMS();
    
    printf("Grafo carregado em: %.2f ms\n", tFim - tInicio);
    
    // Chamada dos algoritmos
    if (!usarMatriz) encontrarArticulacoesTarjan(gl);
    
    return 0;
}
