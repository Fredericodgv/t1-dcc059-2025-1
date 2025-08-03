#ifndef ALGORITMOS_GULOSOS_H
#define ALGORITMOS_GULOSOS_H

#include "Grafo.h"

class AlgoritmosGulosos
{
public:

    static Grafo* conjunto_dominante(Grafo* grafo);
    static Grafo* conjunto_dominante_randomizado(Grafo* grafo);
    static Grafo* conjunto_dominante_reativo(Grafo* grafo, vector<float> &alfas, int iteracoes, int tamanho_bloco);
};

#endif // ALGORITMOS_GULOSOS_H