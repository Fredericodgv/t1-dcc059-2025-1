#ifndef ARESTA_H
#define ARESTA_H

using namespace std;
class Aresta
{
public:
    Aresta();
    Aresta(char id_no_origem, char id_no_alvo, int peso = 0);
    ~Aresta();

    char id_no_origem;
    char id_no_alvo;
    int peso;
};

#endif // ARESTA_H
