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

void Grafo::imprimir_lista_adjacencia()
{
    cout << "Lista de adjacencia:" << endl;

    for (No *no : lista_adj)
    {
        if (no == nullptr)
            continue;

        // Imprime o ID do nó e, se for ponderado, o peso
        cout << no->id;
        if (in_ponderado_vertice)
        {
            cout << "(" << no->peso << ")";
        }

        cout << " -> ";

        for (Aresta *aresta : no->arestas)
        {
            if (aresta == nullptr)
                continue;

            cout << aresta->id_no_alvo;

            if (in_ponderado_aresta)
            {
                cout << "(" << aresta->peso << ")";
            }

            cout << " ";
        }

        cout << endl;
    }
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

No *Grafo::get_no(char id)
{
    for (No *no : lista_adj)
    {
        if (no->id == id)
        {
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

Aresta *Grafo::aux_aresta_custo_minimo_grafo(vector<char> *ids_nos, No **u, No **v)
{
    Aresta *menor_aresta = new Aresta();
    menor_aresta->peso = numeric_limits<int>::max();

    for (int i = 0; i < ids_nos->size(); i++)
    {
        No *no_temporario = get_no((*ids_nos)[i]);
        for (int j = 0; j < no_temporario->arestas.size(); j++)
        {
            if (no_temporario->arestas[j]->peso < menor_aresta->peso)
            {
                menor_aresta = lista_adj[i]->arestas[j];
                *u = get_no(no_temporario->id);
                *v = get_no(no_temporario->arestas[j]->id_no_alvo);
            }
        }
    }

    return menor_aresta;
};

/* Aresta* Grafo::aux_menor_aresta_para(char origem, char destino){
    No* no_origem = get_no(origem);
    Aresta* menor_aresta = new Aresta();
    menor_aresta->peso = numeric_limits<int>::max();

    for (int i = 0; i < no_origem->arestas.size(); i++){
        if(no_origem->arestas[i]->peso < menor_aresta->peso)
            menor_aresta = no_origem->arestas[i];
    }

    return menor_aresta;
}; */

Aresta *Grafo::aux_tem_aresta_para(No *origem, char destino)
{
    Aresta *aresta = new Aresta();
    aresta->peso = numeric_limits<int>::max();

    for (int i = 0; i < origem->arestas.size(); i++)
    {
        if (origem->arestas[i]->id_no_alvo == destino)
            aresta = origem->arestas[i];
    }

    return aresta;
};

Grafo *Grafo::arvore_geradora_minima_prim(vector<char> ids_nos)
{
    if (!in_ponderado_aresta || in_direcionado)
    {
        cout << "Grafo nao ponderado nas arestas ou direcionado." << endl;
    }

    // iniciando grafo resultante
    Grafo *arvore_prim = new Grafo(ids_nos.size(), 0, 1, 1);

    // iniciando vetor de prox
    vector<char> prox(ids_nos.size());
    vector<int> custo(ids_nos.size());
    No *u = new No();
    No *v = new No();

    Aresta *aresta_inicial = aux_aresta_custo_minimo_grafo(&ids_nos, &u, &v); // pegando aresta inicial

    arvore_prim->adicionar_vertice(u->id, u->peso);
    arvore_prim->adicionar_vertice(v->id, v->peso);
    arvore_prim->adicionar_aresta_grafo(u->id, v->id, aresta_inicial->peso);

    
    // e 5 a b c d e 2
    
    for (int i = 0; i < ids_nos.size(); i++)
    {
        if (ids_nos[i] == u->id || ids_nos[i] == v->id) // se for u ou v apenas seta pra 0
        { 
            prox[i] = 0;
            custo[i] = numeric_limits<int>::max();
        }
        else
        {
            Aresta *aresta_u = aux_tem_aresta_para(get_no(ids_nos[i]), u->id);
            Aresta *menor_aresta = aresta_u;
            Aresta *aresta_v = aux_tem_aresta_para(get_no(ids_nos[i]), v->id);
            
            if (aresta_v->peso < aresta_u->peso)
            {
                menor_aresta = aresta_v;
            }
            
            if (menor_aresta->peso != numeric_limits<int>::max())
            {
                prox[i] = menor_aresta->id_no_alvo;
            }
            else
            {
                prox[i] = 0;
            }
            custo[i] = menor_aresta->peso;
        }
    }
    
    for (int i = 0; i < ids_nos.size() - 2; i++)
    {
        int idx_aresta_minima = min_element(custo.begin(), custo.end()) - custo.begin(); //pega o indice do menor elemento no vetor
        arvore_prim->adicionar_vertice(ids_nos[idx_aresta_minima], get_no(ids_nos[idx_aresta_minima])->peso);
        arvore_prim->adicionar_aresta_grafo(ids_nos[idx_aresta_minima], prox[idx_aresta_minima], custo[idx_aresta_minima]);
        
        No* no_recem_adicionado = get_no(ids_nos[idx_aresta_minima]);
        
        int peso_minimo = numeric_limits<int>::max();
        char id_peso_minimo;
        
        for(int j = 0; j < no_recem_adicionado->arestas.size(); j++){
            No* novo_no_acessivel = get_no(no_recem_adicionado->arestas[j]->id_no_alvo);
            Aresta* aresta = aux_tem_aresta_para(novo_no_acessivel, prox[idx_aresta_minima]);
            
            if(aresta->peso < peso_minimo){
                peso_minimo = aresta->peso;
                id_peso_minimo = novo_no_acessivel->id;
            }
        }
        
        if(peso_minimo != numeric_limits<int>::max()){
            int idx_id = find(ids_nos.begin(), ids_nos.end(), id_peso_minimo) - ids_nos.begin();
            prox[idx_id] = ids_nos[idx_aresta_minima];
        }
        
        prox[idx_aresta_minima] = 0;
        custo[idx_aresta_minima] = numeric_limits<int>::max();
    }
    
    arvore_prim->imprimir_lista_adjacencia();
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
