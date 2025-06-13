#include "Grafo.h"

Grafo::Grafo()
{
    ordem = 0;
    in_direcionado = false;
    in_ponderado_aresta = false;
    in_ponderado_vertice = false;
}

Grafo::Grafo(int ordem, bool direcionado, bool ponderado_aresta, bool ponderado_vertice)
{
    this->ordem = ordem;
    this->in_direcionado = direcionado;
    this->in_ponderado_aresta = ponderado_aresta;
    this->in_ponderado_vertice = ponderado_vertice;
}

Grafo::~Grafo()
{
}

bool Grafo::adicionar_vertice(char id, int peso)
{
    if (ordem < lista_adj.size())
    {
        cout << "Grafo cheio, nao e possivel adicionar mais nos." << endl;
        return false;
    }

    No *novo_no = new No();
    novo_no->id = id;
    novo_no->peso = peso;
    lista_adj.push_back(novo_no);
    return true;
}

bool Grafo::adicionar_aresta_grafo(char id_no_origem, char id_no_destino, int peso)
{
    No *no_origem = get_no(id_no_origem);
    No *no_destino = get_no(id_no_destino);

    if (no_origem == nullptr || no_destino == nullptr)
    {
        cout << "Um ou ambos os nos nao existem." << endl;
        return false;
    }

    if (no_origem->adicionar_aresta(id_no_destino, peso))
    {
        if (!in_direcionado)
        {
            no_destino->adicionar_aresta(id_no_origem, peso);
        }
        return true;
    }
    else
    {
        cout << "Erro ao adicionar aresta." << endl;
        return false;
    }
}

No* Grafo::get_no(char id) {
    for (No* no : lista_adj) {
        if (no->id == id) {
            return no;
        }
    }
    return nullptr;
}

vector<char> Grafo::fecho_transitivo_direto(int id_no)
{
    cout << "Metodo nao implementado" << endl;
    return {};
}

vector<char> Grafo::fecho_transitivo_indireto(int id_no)
{
    cout << "Metodo nao implementado" << endl;
    return {};
}

vector<char> Grafo::caminho_minimo_dijkstra(int id_no_a, int id_no_b)
{
    cout << "Metodo nao implementado" << endl;
    return {};
}

vector<char> Grafo::caminho_minimo_floyd(int id_no, int id_no_b)
{
    cout << "Metodo nao implementado" << endl;
    return {};
}

Grafo *Grafo::arvore_geradora_minima_prim(vector<char> ids_nos)
{
    cout << "Metodo nao implementado" << endl;
    return nullptr;
}

Grafo *Grafo::arvore_geradora_minima_kruskal(vector<char> ids_nos)
{
    cout << "Metodo nao implementado" << endl;
    return nullptr;
}

Grafo *Grafo::arvore_caminhamento_profundidade(int id_no)
{
    cout << "Metodo nao implementado" << endl;
    return nullptr;
}

int Grafo::raio()
{
    cout << "Metodo nao implementado" << endl;
    return 0;
}

int Grafo::diametro()
{
    cout << "Metodo nao implementado" << endl;
    return 0;
}

vector<char> Grafo::centro()
{
    cout << "Metodo nao implementado" << endl;
    return {};
}

vector<char> Grafo::periferia()
{
    cout << "Metodo nao implementado" << endl;
    return {};
}

vector<char> Grafo::vertices_de_articulacao()
{
    cout << "Metodo nao implementado" << endl;
    return {};
}
