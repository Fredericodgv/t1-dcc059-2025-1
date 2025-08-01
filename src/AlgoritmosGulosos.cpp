#include "AlgoritmosGulosos.h"
#include <vector>
#include <set>
#include <algorithm>
#include <random> // Necessário para a randomização
#include <chrono> // Necessário para uma semente de tempo de alta qualidade

// --- Algoritmo Guloso Padrão ---

Grafo *AlgoritmosGulosos::conjunto_dominante(Grafo *grafo)
{
}

// --- Algoritmo Guloso Randomizado ---

Grafo *AlgoritmosGulosos::conjunto_dominante_randomizado(Grafo *grafo)
{
}

// --- Algoritmo Guloso Reativo ---

void aux_calcula_probabilidades(float probabilidades[], float medias[], int m, float melhor_qualidade)
{
    float somatorio_qualidades = 0;
    float qualidades[m];

    for (int i = 0; i < m; i++)
    {
        qualidades[i] = melhor_qualidade / medias[i];
        somatorio_qualidades += qualidades[i];
    }

    for (int i = 0; i < m; i++)
    {
        probabilidades[i] = qualidades[i] / somatorio_qualidades;
    }
}

float aux_escolhe_alfa(float alfas[], float probabilidades[], int m)
{
    float soma = 0;
    int pos_soma = 0;
    mt19937 gerador(chrono::system_clock::now().time_since_epoch().count());

    uniform_real_distribution<> dist(0.0, 1.0);
    float aleat = dist(gerador);

    for (int i = 0; i < m; i++)
    {
        soma += probabilidades[i];
        pos_soma = i;

        if(soma >= aleat) break;
    }

    return alfas[pos_soma];
}

Grafo *AlgoritmosGulosos::conjunto_dominante_reativo(Grafo *grafo, float alfas[], int iteracoes, int tamanho_bloco)
{
    int m = sizeof(alfas);
    float probabilidades[m], medias[m];

    for (int i = 0; i < iteracoes; i++)
    {
        if (i % tamanho_bloco == 0)
        {
        } // controla número de iterações antes de recalcular
        // recalcula chances

        float alfa = aux_escolhe_alfa(alfas, probabilidades, m);
    }
}