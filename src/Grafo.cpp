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
    pos_id.insert({id, lista_adj.size()});
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
    auto it_map = pos_id.find(id);
    if(it_map != pos_id.end()){
        return lista_adj[pos_id.find(id)->second];
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

// funções de quicksort pegas na geeksforgeeks
int partition(vector<No *> &vec, int low, int high)
{

    // Selecting last element as the pivot
    int pivot = vec[high]->dijkstra_custo_minimo;

    // Index of elemment just before the last element
    // It is used for swapping
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++)
    {

        // If current element is smaller than or
        // equal to pivot
        if (vec[j]->dijkstra_custo_minimo <= pivot)
        {
            i++;
            swap(vec[i], vec[j]);
        }
    }

    // Put pivot to its position
    swap(vec[i + 1], vec[high]);

    // Return the point of partition
    return (i + 1);
}

void quicksort_nos_dijkstra(vector<No *> &vec, int low, int high)
{

    // Base case: This part will be executed till the starting
    // index low is lesser than the ending index high
    if (low < high)
    {

        // pi is Partitioning Index, arr[p] is now at
        // right place
        int pi = partition(vec, low, high);

        // Separately sort elements before and after the
        // Partition Index pi
        quicksort_nos_dijkstra(vec, low, pi - 1);
        quicksort_nos_dijkstra(vec, pi + 1, high);
    }
}

void Grafo::aux_retorna_chars_caminho_dijkstra(vector<char> &vec, char id_atual, char id_no_a)
{
    No *no_atual = get_no(id_atual);
    char id_responsavel = no_atual->dijkstra_responsavel;
    if (id_atual != id_no_a)
    {
        aux_retorna_chars_caminho_dijkstra(vec, id_responsavel, id_no_a);
    }

    vec.push_back(no_atual->id);
}

vector<char> Grafo::caminho_minimo_dijkstra(int id_no_a, int id_no_b)
{
    if (!in_ponderado_aresta)
    {
        cout << "Nao e ponderado nas arestas" << endl;
        return {};
    }

    vector<char> resultante(0);
    vector<No *> nos_abertos(0);

    No *no_inicial = get_no(id_no_a);
    no_inicial->dijkstra_custo_minimo = 0;
    no_inicial->dijkstra_responsavel = id_no_a;
    no_inicial->dijkstra_fechado = false;

    nos_abertos.push_back(no_inicial);
    for (int i = 0; i < lista_adj.size(); i++)
    {
        if (lista_adj[i]->id != id_no_a)
        {
            nos_abertos.push_back(lista_adj[i]);
            lista_adj[i]->dijkstra_custo_minimo = numeric_limits<int>::max();
            lista_adj[i]->dijkstra_responsavel = 0;
            lista_adj[i]->dijkstra_fechado = false;
        }
    }

    // teste: c c b 2
    for (int i = 0; i < lista_adj.size(); i++)
    {
        No *no_custo_minimo = nos_abertos[i];
        no_custo_minimo->dijkstra_fechado = true;

        if (no_custo_minimo->id == id_no_b)
        {
            break;
        }

        for (int j = 0; j < no_custo_minimo->arestas.size(); j++)
        {
            No *no = get_no(no_custo_minimo->arestas[j]->id_no_alvo);
            int novo_custo = no_custo_minimo->dijkstra_custo_minimo + no_custo_minimo->arestas[j]->peso;

            if (!no->dijkstra_fechado)
            {
                if (no->dijkstra_custo_minimo > novo_custo)
                {
                    no->dijkstra_custo_minimo = novo_custo;
                    no->dijkstra_responsavel = no_custo_minimo->id;
                }
            }
        }

        quicksort_nos_dijkstra(nos_abertos, i, nos_abertos.size() - 1);
    }

    aux_retorna_chars_caminho_dijkstra(resultante, id_no_b, id_no_a);

    return resultante;
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

Aresta *aux_tem_aresta_para(No *origem, char destino)
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

    // adicionando aresta e vertices iniciais no grafo
    arvore_prim->adicionar_vertice(u->id, u->peso);
    arvore_prim->adicionar_vertice(v->id, v->peso);
    arvore_prim->adicionar_aresta_grafo(u->id, v->id, aresta_inicial->peso);

    // teste:
    //  e 5 a b c d e 2
    // inicializando vetor prox
    for (int i = 0; i < ids_nos.size(); i++) // para cada nó
    {
        if (ids_nos[i] == u->id || ids_nos[i] == v->id) // se for u ou v apenas seta id pra 0 e custo pra infinito
        {
            prox[i] = 0;
            custo[i] = numeric_limits<int>::max();
        }
        else
        {
            // vê se nó tem aresta para u ou v e pega a de menor peso
            Aresta *aresta_u = aux_tem_aresta_para(get_no(ids_nos[i]), u->id);
            Aresta *menor_aresta = aresta_u;
            Aresta *aresta_v = aux_tem_aresta_para(get_no(ids_nos[i]), v->id);

            if (aresta_v->peso < aresta_u->peso)
            {
                menor_aresta = aresta_v;
            }

            if (menor_aresta->peso != numeric_limits<int>::max()) // apenas considera a aresta válida se o peso não for infinito(existe conexão)
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
        // pega o indice do menor elemento no vetor e adiciona vértice e aresta correspondente
        int idx_aresta_minima = min_element(custo.begin(), custo.end()) - custo.begin();
        arvore_prim->adicionar_vertice(ids_nos[idx_aresta_minima], get_no(ids_nos[idx_aresta_minima])->peso);
        arvore_prim->adicionar_aresta_grafo(ids_nos[idx_aresta_minima], prox[idx_aresta_minima], custo[idx_aresta_minima]);

        No *no_recem_adicionado = get_no(ids_nos[idx_aresta_minima]);

        for (int j = 0; j < no_recem_adicionado->arestas.size(); j++)
        {                                                                                     // para cada aresta do nó adicionado
            No *novo_no_acessivel = get_no(no_recem_adicionado->arestas[j]->id_no_alvo);      // pega o id alvo
            Aresta *aresta = aux_tem_aresta_para(novo_no_acessivel, prox[idx_aresta_minima]); // se tem aresta para o grafo resultante

            // pega o prox do novo nó acessível e vê se compensa substituir a aresta
            int idx_id = find(ids_nos.begin(), ids_nos.end(), novo_no_acessivel->id) - ids_nos.begin();
            if (aresta->peso < custo[idx_id])
            {
                prox[idx_id] = ids_nos[idx_aresta_minima]; // atualiza prox
                custo[idx_id] = aresta->peso;
            }
        }

        prox[idx_aresta_minima] = 0;
        custo[idx_aresta_minima] = numeric_limits<int>::max();
    }

    return arvore_prim;
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
