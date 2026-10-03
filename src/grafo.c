#include <stdio.h>
#include <stdlib.h>
#include "include/grafo.h"

static No* criar_no(int destino) {
    No* novo_no = (No*) malloc(sizeof(No));
    novo_no->destino = destino;
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

void adicionar_aresta(Grafo* g, int origem, int destino) {
    if (!g || origem >= g->num_vertices || destino >= g->num_vertices) return;


    No* novo_no = criar_no(destino);
    novo_no->proximo = g->listas_adj[origem];
    g->listas_adj[origem] = novo_no;

  
    novo_no = criar_no(origem);
    novo_no->proximo = g->listas_adj[destino];
    g->listas_adj[destino] = novo_no;

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
