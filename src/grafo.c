#include <stdio.h>
#include <stdlib.h>
#include "include/grafo.h"

static No* criar_no(int destino, double peso) {
    No* novo_no = malloc(sizeof(No));

    if (novo_no == NULL) {
        return NULL;
    }

    novo_no->destino = destino;
    novo_no->peso = peso;
    novo_no->proximo = NULL;

    return novo_no;
}

Grafo* criar_grafo(int num_vertices) {
    Grafo* g = (Grafo*) malloc(sizeof(Grafo));
    g->num_vertices = num_vertices;
    g->num_arestas = 0;
    g->listas_adj = (No**) malloc(num_vertices * sizeof(No*));

    for (int i = 0; i < num_vertices; i++) {
        g->listas_adj[i] = NULL;
    }

    return g;
}

void adicionar_aresta(
    Grafo* g,
    int origem,
    int destino,
    double peso
) {
    if (!g || origem < 0 || destino < 0 ||
        origem >= g->num_vertices ||
        destino >= g->num_vertices) {
        return;
    }

    No* novo_origem = criar_no(destino, peso);
    No* novo_destino = criar_no(origem, peso);

    if (!novo_origem || !novo_destino) {
        free(novo_origem);
        free(novo_destino);
        return;
    }

    novo_origem->proximo = g->listas_adj[origem];
    g->listas_adj[origem] = novo_origem;

    novo_destino->proximo = g->listas_adj[destino];
    g->listas_adj[destino] = novo_destino;

    g->num_arestas++;
}

void liberar_grafo(Grafo* g) {
    if (!g) return;

    for (int i = 0; i < g->num_vertices; i++) {
        No* atual = g->listas_adj[i];
        while (atual != NULL) {
            No* temp = atual;
            atual = atual->proximo;
            free(temp);
        }
    }

    free(g->listas_adj);
    free(g);
}
