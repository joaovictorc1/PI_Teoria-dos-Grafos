#ifndef BUSCA_LARGURA_H
#define BUSCA_LARGURA_H

#include "grafo.h"

void busca_largura(Grafo* g, int inicio);
void identificar_articulacoes_bfs(Grafo* g);


void simular_ataque_roteador(Grafo* g, int id_atacado);

#endif