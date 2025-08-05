#ifndef ALGORITMOS_GULOSOS_H
#define ALGORITMOS_GULOSOS_H

#include "Grafo.h"

class AlgoritmosGulosos
{
public:
    static Grafo *conjunto_dominante(Grafo *grafo);
    static Grafo *conjunto_dominante_randomizado(Grafo *grafo, float alfa = 0.5f);
    static Grafo *conjunto_dominante_reativo(Grafo *grafo, vector<float> &alfas, int iteracoes, int tamanho_bloco);
    static Grafo* executar_randomizado_n_vezes(Grafo* grafo, int n_iteracoes, float alfa);
    Grafo* executar_reativo_n_vezes(const Grafo* grafo, int n_execucoes, std::vector<float>& alfas, int iteracoes_reativo, int tamanho_bloco);
};

#endif // ALGORITMOS_GULOSOS_H