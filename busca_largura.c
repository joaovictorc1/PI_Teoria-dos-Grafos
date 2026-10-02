#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "grafo.h"
#include "busca_largura.h"

void busca_largura(Grafo* g, int inicio) {
    if (!g || inicio < 0 || inicio >= g->num_vertices) return;

    bool* visitado = (bool*) calloc(g->num_vertices, sizeof(bool));
    int* fila = (int*) malloc(g->num_vertices * sizeof(int));
    int frente = 0, tras = 0;

    visitado[inicio] = true;
    fila[tras++] = inicio;

    printf("Busca em Largura (BFS) a partir do vertice %d:\n", inicio);

    while (frente < tras) {
        int atual = fila[frente++];
        printf("%d ", atual);

        No* temp = g->listas_adj[atual];
        while (temp != NULL) {
            int vizinho = temp->destino;
            if (!visitado[vizinho]) {
                visitado[vizinho] = true;
                fila[tras++] = vizinho;
            }
            temp = temp->proximo;
        }
    }
    printf("\n");

    free(visitado);
    free(fila);
}


static int bfs_contar_alcancaveis(Grafo* g, int vertice_inicio, int vertice_ignorado) {
    if (vertice_inicio == vertice_ignorado) return 0;

    bool* visitado = (bool*) calloc(g->num_vertices, sizeof(bool));
    int* fila = (int*) malloc(g->num_vertices * sizeof(int));
    int frente = 0, tras = 0;
    int contagem = 0;

    visitado[vertice_inicio] = true;
    fila[tras++] = vertice_inicio;
    contagem++;

    while (frente < tras) {
        int atual = fila[frente++];
        No* temp = g->listas_adj[atual];

        while (temp != NULL) {
            int vizinho = temp->destino;
            if (vizinho != vertice_ignorado && !visitado[vizinho]) {
                visitado[vizinho] = true;
                fila[tras++] = vizinho;
                contagem++;
            }
            temp = temp->proximo;
        }
    }

    free(visitado);
    free(fila);
    return contagem;
}


void identificar_articulacoes_bfs(Grafo* g) {
    if (!g) return;

    printf("\n--- ROTEADORES CRITICOS IDENTIFICADOS ---\n");
    int ap_count = 0;

    for (int i = 0; i < g->num_vertices; i++) {
        int inicio = (i == 0) ? 1 : 0;
        int alcancaveis = bfs_contar_alcancaveis(g, inicio, i);

        if (alcancaveis < g->num_vertices - 1) {
            printf("[ALERTA] Roteador ID %d: se desativado dividira a rede.\n", i);
            ap_count++;
        }
    }

    printf("Total de roteadores criticos: %d / %d\n", ap_count, g->num_vertices);
}


void simular_ataque_roteador(Grafo* g, int id_atacado) {
    if (!g || id_atacado < 0 || id_atacado >= g->num_vertices) {
        printf("[ERRO] ID de roteador invalido para simulacao.\n");
        return;
    }

    printf("\n=======================================================\n");
    printf("   SIMULACAO DE ATAQUE / FALHA NO ROTEADOR ID: %d\n", id_atacado);
    printf("=======================================================\n");

    bool* visitado = (bool*) calloc(g->num_vertices, sizeof(bool));
    int* fila = (int*) malloc(g->num_vertices * sizeof(int));
    int frente = 0, tras = 0;

    int inicio = (id_atacado == 0) ? 1 : 0;

    visitado[inicio] = true;
    fila[tras++] = inicio;

    while (frente < tras) {
        int atual = fila[frente++];
        No* temp = g->listas_adj[atual];

        while (temp != NULL) {
            int vizinho = temp->destino;
            if (vizinho != id_atacado && !visitado[vizinho]) {
                visitado[vizinho] = true;
                fila[tras++] = vizinho;
            }
            temp = temp->proximo;
        }
    }

    int alcancaveis = 0;
    int isolados = 0;

    printf("Roteadores ISOLADOS / SEM SINAL devido ao ataque:\n[ ");
    for (int i = 0; i < g->num_vertices; i++) {
        if (i == id_atacado) continue;

        if (!visitado[i]) {
            printf("%d ", i);
            isolados++;
        } else {
            alcancaveis++;
        }
    }
    printf("]\n");

    printf("\nResumo do Impacto:\n");
    printf("- Roteador atacado/fora de operacao: ID %d\n", id_atacado);
    printf("- Roteadores ativos e conectados:   %d / %d\n", alcancaveis, g->num_vertices - 1);
    printf("- Roteadores/Clientes desconectados: %d\n", isolados);

    if (isolados > 0) {
        printf(" STATUS: O ROTEADOR %d E UM PONTO CRITICO DE FALHA (ARTICULACAO)!\n", id_atacado);
    } else {
        printf(" STATUS: A rede possui rotas redundantes. Nao houve isolamento.\n");
    }
    printf("=======================================================\n\n");

    free(visitado);
    free(fila);
}