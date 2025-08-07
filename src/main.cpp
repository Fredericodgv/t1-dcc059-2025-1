#include <iostream>
#include <string>
#include <fstream>
#include "Gerenciador.h"
#include "AlgoritmosGulosos.h"

using namespace std;
int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        cerr << "Uso: " << argv[0] << " <arquivo_de_entrada>" << endl;
        return 1;
    }

    Grafo *grafo = new Grafo();

    string nome_arquivo = argv[1];

    grafo = Gerenciador::ler_arquivo(nome_arquivo);

    //grafo->imprimir_lista_adjacencia();

    //Grafo* resultado = AlgoritmosGulosos::conjunto_dominante_reativo(grafo);

    //resultado->imprimir_lista_adjacencia();

    //grafo->imprimir_lista_adjacencia();

    Gerenciador::comandos(grafo);

    return 0;
}