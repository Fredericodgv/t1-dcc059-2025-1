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
    vector<Aresta*> arestas;

    bool adicionar_aresta(char id_no_destino, int peso = 0);
};



#endif //NO_H
