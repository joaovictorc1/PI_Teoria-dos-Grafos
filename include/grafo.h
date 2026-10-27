#ifndef GRAFO_H
#define GRAFO_H


typedef struct No {
    int destino;
    double peso;
    struct No* proximo;
} No;

typedef struct Grafo {
    int num_vertices;
    int num_arestas;
    No** listas_adj;
} Grafo;

Grafo* criar_grafo(int num_vertices);
void adicionar_aresta(Grafo* g,
    int origem,
    int destino,
    double peso);
void liberar_grafo(Grafo* g);

#endif