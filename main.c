#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "grafo.h"
#include "busca_largura.h"


Grafo* carregar_dataset(const char* nome_arquivo, int num_vertices) {
    FILE* arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        printf("Erro: Nao foi possivel abrir o arquivo %s\n", nome_arquivo);
        return NULL;
    }

    Grafo* g = criar_grafo(num_vertices);
    int u, v;


    while (fscanf(arquivo, "%d %d", &u, &v) == 2) {
        if (u < num_vertices && v < num_vertices) {
            adicionar_aresta(g, u, v);
        }
    }

    fclose(arquivo);
    return g;
}

int main() {

    int NUM_VERTICES = 1000; 
    const char* ARQUIVO_DATASET = "../data/dataset_SNAP.txt";

    printf("Carregando dataset de rede com %d vertices...\n", NUM_VERTICES);

    struct timespec inicio, fim;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    Grafo* g = carregar_dataset(ARQUIVO_DATASET, NUM_VERTICES);
    if (!g) return 1;

    printf("Grafo instanciado em Lista de Adjacencia: %d vertices, %d arestas.\n", 
           g->num_vertices, g->num_arestas);


    identificar_articulacoes_bfs(g);

    clock_gettime(CLOCK_MONOTONIC, &fim);

    double tempo_ms = (fim.tv_sec - inicio.tv_sec) * 1000.0 + 
                      (fim.tv_nsec - inicio.tv_nsec) / 1000000.0;

    printf("\n--- TELEMETRIA ---\n");
    printf("Tempo total de execucao: %.3f ms\n", tempo_ms);

    liberar_grafo(g);
    return 0;
}