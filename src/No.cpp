#include "No.h"
#include <algorithm>
using namespace std;

No::No()
{
}

No::~No()
{
}

bool No::adicionar_aresta(char id_no_origem, char id_no_destino, int peso)
{
    Aresta *nova_aresta = new Aresta(id_no_origem, id_no_destino, peso);
    arestas.push_back(nova_aresta);
    return true;
}

void No::remover_no_util(char id){
    auto it = find(dominante_conexo_nos_uteis.begin(), dominante_conexo_nos_uteis.end(), id);
    if(it != dominante_conexo_nos_uteis.end()){
        dominante_conexo_nos_uteis.erase(it);
    }
}