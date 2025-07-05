#ifndef GERENCIADOR_H
#define GERENCIADOR_H

#include <iostream>
#include "Grafo.h"
#include <algorithm>
#include <fstream>

using namespace std;
class Gerenciador
{
public:
    static void comandos(Grafo *grafo);
    static char get_id_entrada();
    static vector<char> get_conjunto_ids(Grafo *grafo, int tam);
    static bool pergunta_imprimir_arquivo(string nome_arquivo);
    static Grafo *ler_arquivo(const string &nome_arquivo);
    static void imprimir_grafo_arquivo(Grafo *grafo, string nome_arquivo);
    static void imprimir_vector_tela(vector<char> &resultado);
    static void imprimir_vector_arquivo(vector<char> &resultado, string arquivo, string descricao);
};

#endif // GERENCIADOR_H
