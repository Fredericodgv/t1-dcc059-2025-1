//
// Created by Rafael on 28/05/2025.
//

#ifndef GRAFO_H
#define GRAFO_H

#include "No.h"
#include "Aresta.h"
#include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <queue>

using namespace std;
class Grafo
{
public:
    Grafo();
    Grafo(int ordem, bool direcionado, bool ponderado_aresta, bool ponderado_vertice);
    ~Grafo();

    void imprimir_lista_adjacencia();
    bool aux_e_conexo();

    bool adicionar_vertice(char id, int peso = 0);
    bool adicionar_aresta_grafo(char id_no_origem, char id_no_destino, int peso = 0);

    Grafo *gerar_subgrafo(vector<char> ids_nos);
    Aresta *aux_aresta_custo_minimo_grafo(Grafo *grafo, vector<char> *ids_nos, No *&u, No *&v);

    void aux_retorna_chars_caminho_dijkstra(vector<char> &vec, char id_atual, char id_no_a);

    void aux_caminhamento_profundidade(char id_no, function<void(No *)> funcao_caminhamento, Grafo *arvore = nullptr);
    void caminhamento_profundidade(char id_no, function<void(No *)> funcao_caminhamento, Grafo *arvore = nullptr);
    void aux_insere_aresta_arvore_caminhamento(Grafo *arvore, No *no);
    void aux_reseta_visitas();
    int aux_calcula_distancia_nos(char id_no_inicial, char id_no_final);
    int aux_calcula_excentricidade_no(char id_no);
    bool aux_verifica_vertice_central(char id_no);
    bool aux_verifica_vertice_periferico(char id_no);
    map<char, int> bfs_calcula_distancias(char no_origem);

    void aux_fecho_transitivo_direto(No *no, vector<char> &resultado);

    No *get_no(char id); // a

    vector<char> fecho_transitivo_direto(char id_no);                 // a
    vector<char> fecho_transitivo_indireto(char id_no);               // b
    vector<char> caminho_minimo_dijkstra(char id_no_a, char id_no_b); // c
    vector<char> caminho_minimo_floyd(char id_no, char id_no_b);      // d
    Grafo *arvore_geradora_minima_prim(vector<char> ids_nos);         // e
    Grafo *arvore_geradora_minima_kruskal(vector<char> ids_nos);      // f
    Grafo *arvore_caminhamento_profundidade(char id_no);              // g
    void imprimir_matrizes_floyd();
    int raio();                             // h 1
    int diametro();                         // h 2
    vector<char> centro();                  // h 3
    vector<char> periferia();               // h 4
    vector<char> vertices_de_articulacao(); // i

    int ordem;
    bool in_direcionado;
    bool in_ponderado_aresta;
    bool in_ponderado_vertice;
    vector<No *> lista_adj;
    map<char, int> pos_id;
    // matrizes de floyd
    vector<vector<int>> matriz_distancia;
    vector<vector<char>> matriz_predecessor;
    bool floyd_gerado = false;

private:
    void gera_floyd();                                        // Gera as matrizes de Floyd se necessário
    char find_set(map<char, char> &parent, char v);           // Funcao auxiliar para encontrar o representante de um conjunto
    void union_sets(map<char, char> &parent, char u, char v); // Funcao auxiliar para unir dois conjuntos
};

#endif // GRAFO_H
