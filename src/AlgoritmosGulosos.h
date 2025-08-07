#ifndef ALGORITMOS_GULOSOS_H
#define ALGORITMOS_GULOSOS_H

#include "Grafo.h"

class AlgoritmosGulosos
{
public:
    static std::vector<char> conjunto_dominante_conexo(Grafo *grafo);
    static std::vector<char> conjunto_dominante_randomizado(Grafo *grafo, float alfa = 0.5f);
    static std::vector<char> conjunto_dominante_reativo(Grafo *grafo, vector<float> &alfas, int iteracoes, int tamanho_bloco);
    static std::vector<char> executar_randomizado_n_vezes(Grafo *grafo, int n_iteracoes, float alfa);
    static std::vector<char> executar_reativo_n_vezes(Grafo *grafo, int n_execucoes, std::vector<float> &alfas, int iteracoes_reativo, int tamanho_bloco);
};

#endif // ALGORITMOS_GULOSOS_H