#ifndef GRAFO_H
#define GRAFO_H


typedef struct No {
    int destino;
    struct No* proximo;
} No;

typedef struct Grafo {
    int num_vertices;
    int num_arestas;
    No** listas_adj;
} Grafo;

Grafo* criar_grafo(int num_vertices);
void adicionar_aresta(Grafo* g, int origem, int destino);
void liberar_grafo(Grafo* g);

#endif