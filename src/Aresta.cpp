#include "Aresta.h"

Aresta::Aresta()
{
}

Aresta::Aresta(char id_no_origem, char id_no_alvo, int peso)
{
    this->id_no_origem = id_no_origem;
    this->id_no_alvo = id_no_alvo;
    this->peso = peso;
}

Aresta::~Aresta()
{
}