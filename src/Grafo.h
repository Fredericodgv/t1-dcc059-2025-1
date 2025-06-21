//
// Created by Rafael on 28/05/2025.
//

#ifndef GRAFO_H
#define GRAFO_H

#include "No.h"
#include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;
class Grafo
{
public:
    Grafo();
    Grafo(int ordem, bool direcionado, bool ponderado_aresta, bool ponderado_vertice);
    ~Grafo();

    void imprimir_lista_adjacencia();

    bool adicionar_vertice(char id, int peso = 0);
    bool adicionar_aresta_grafo(char id_no_origem, char id_no_destino, int peso = 0);

    Aresta* aux_aresta_custo_minimo_grafo(vector<char>* ids_nos, No** u, No** v);
    void aux_retorna_chars_caminho_dijkstra(vector<char> &vec, char id_atual, char id_no_a);

    No *get_no(char id); // a

    vector<char> fecho_transitivo_direto(int id_no);                // a
    vector<char> fecho_transitivo_indireto(int id_no);              // b
    vector<char> caminho_minimo_dijkstra(int id_no_a, int id_no_b); // c
    vector<char> caminho_minimo_floyd(int id_no, int id_no_b);      // d
    Grafo *arvore_geradora_minima_prim(vector<char> ids_nos);       // e
    Grafo *arvore_geradora_minima_kruskal(vector<char> ids_nos);    // f
    Grafo *arvore_caminhamento_profundidade(int id_no);             // g
    int raio();                                                     // h 1
    int diametro();                                                 // h 2
    vector<char> centro();                                          // h 3
    vector<char> periferia();                                       // h 4
    vector<char> vertices_de_articulacao();                         // i

    int ordem;
    bool in_direcionado;
    bool in_ponderado_aresta;
    bool in_ponderado_vertice;
    vector<No *> lista_adj;
};

#endif // GRAFO_H
