#ifndef NO_H
#define NO_H

#include <string>
#include <vector>
#include "Aresta.h"


using namespace std;
class No {
public:
    No();
    ~No();

    char id;
    int peso;
    int dijkstra_custo_minimo;
    bool dijkstra_fechado;
    char dijkstra_responsavel = 0;
    bool no_visitado = 0;

    vector<Aresta*> arestas;

    bool adicionar_aresta(char id_no_destino, int peso = 0);
};



#endif //NO_H
