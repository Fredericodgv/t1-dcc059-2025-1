#include "No.h"

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