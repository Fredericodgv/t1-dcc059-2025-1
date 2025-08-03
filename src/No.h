#ifndef NO_H
#define NO_H

#include <string>
#include <vector>
#include "Aresta.h"
#include <list>

using namespace std;
class No
{
public:
    No();
    ~No();

    char id;
    int peso;
    int dijkstra_custo_minimo;
    bool dijkstra_fechado;
    char dijkstra_responsavel = 0;
    bool no_visitado = false;
    bool dominante_conexo_candidato = false;
    bool dominante_conexo_dominante = false;
    list<char> dominante_conexo_nos_uteis;
    //float dominante_conexo_grau_util = 0;

    vector<Aresta *> arestas;

    bool adicionar_aresta(char id_no_origem, char id_no_destino, int peso = 0);
    void remover_no_util(char id);
};

#endif // NO_H
