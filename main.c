#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "include/grafo.h"
#include "include/busca_largura.h"

Grafo* carregar_dataset(const char* nome_arquivo, int num_vertices) {
    FILE* arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        printf("Erro: Nao foi possivel abrir o arquivo %s\n", nome_arquivo);
        return NULL;
    }

    Grafo* g = criar_grafo(num_vertices);
    char linha[256];

    // Le o arquivo linha por linha
    while (fgets(linha, sizeof(linha), arquivo)) {
        // Ignora linhas em branco ou comentarios (que comecam com #)
        if (linha[0] == '\n' || linha[0] == '#') {
            continue;
        }

        int u, v;
        // Tenta extrair dois inteiros da linha
        if (sscanf(linha, "%d %d", &u, &v) == 2) {
            if (u < num_vertices && v < num_vertices) {
                adicionar_aresta(g, u, v);
            }
        }
    }

    fclose(arquivo);
    return g;
}
int main(int argc, char *argv[]) {
    // Permite rodar no terminal com: ./programa 28000
    int NUM_VERTICES = 28000; // Valor alto para cobrir IDs dispersos de datasets reais
    if (argc >= 2) {
        NUM_VERTICES = atoi(argv[1]);
    }

    const char* ARQUIVO_DATASET = "data/CA-GrQc.txt";

    printf("=======================================================\n");
    printf("   ANALISE TOPOLOGICA DE REDES (FASE I)\n");
    printf("=======================================================\n");
    printf("Configuracao: %d vertices maximos | Estrutura: Lista de Adjacencia\n", NUM_VERTICES);

    struct timespec inicio, fim;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    Grafo* g = carregar_dataset(ARQUIVO_DATASET, NUM_VERTICES);
    if (!g) return 1;

    printf("\n[OK] Grafo carregado com sucesso!\n");
    printf("Vertices alocados: %d | Arestas lidas: %d\n", g->num_vertices, g->num_arestas);

    // Chama sua funcao baseada em BFS
    identificar_articulacoes_bfs(g);

    clock_gettime(CLOCK_MONOTONIC, &fim);
    double tempo_ms = (fim.tv_sec - inicio.tv_sec) * 1000.0 + (fim.tv_nsec - inicio.tv_nsec) / 1000000.0;

    // Calculo do Consumo de Memoria da Lista de Adjacencia
    double memoria_mb = (sizeof(Grafo) + (NUM_VERTICES * sizeof(No*)) + (g->num_arestas * 2 * sizeof(No))) / (1024.0 * 1024.0);

    printf("\n--- TELEMETRIA ---\n");
    printf("Tempo total de execucao: %.3f ms\n", tempo_ms);
    printf("Consumo estimado de memoria: %.2f MB\n", memoria_mb);
    printf("=======================================================\n");

    liberar_grafo(g);
    return 0;
}