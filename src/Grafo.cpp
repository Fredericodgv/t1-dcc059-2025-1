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

    // Inicializa matrizes de Floyd
    if (in_ponderado_aresta)
    {
        matriz_distancia.resize(ordem, vector<int>(ordem, numeric_limits<int>::max()));
        matriz_predecessor.resize(ordem, vector<char>(ordem, 0));
    }
}

Grafo::~Grafo()
{
}

void Grafo::imprimir_lista_adjacencia()
{
    // Ordena os nós por ID para consistência na exibição
    sort(lista_adj.begin(), lista_adj.end(),
         [](No *a, No *b)
         { return a->id < b->id; });

    for (int i = 0; i < lista_adj.size(); i++)
    {
        No *no = lista_adj[i];
        if (no == nullptr)
            continue;

        pos_id.insert_or_assign(no->id, i);
        // Imprime o ID do nó
        cout << no->id << ": ";

        // Ordena as arestas por ID do nó de destino
        sort(no->arestas.begin(), no->arestas.end(),
             [](Aresta *a, Aresta *b)
             { return a->id_no_alvo < b->id_no_alvo; });

        bool first = true;
        for (Aresta *aresta : no->arestas)
        {
            if (aresta == nullptr)
                continue;

            if (!first)
                cout << " -> ";
            first = false;

            // Imprime o nó de destino
            cout << aresta->id_no_alvo;

            // Se for ponderado, imprime o peso
            if (in_ponderado_aresta)
            {
                cout << "(" << aresta->peso << ")";
            }
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

    if (no_origem->adicionar_aresta(id_no_origem, id_no_destino, peso))
    {
        if (!in_direcionado)
        {
            no_destino->adicionar_aresta(id_no_destino, id_no_origem, peso);
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
    if (it_map != pos_id.end())
    {
        return lista_adj[it_map->second];
    }
    return nullptr;
}

void Grafo::aux_fecho_transitivo_direto(No *no, vector<char> &resultado)
{
    resultado.push_back(no->id);
}

vector<char> Grafo::fecho_transitivo_direto(char id_no)
{
    if (!in_direcionado)
    {
        cout << "Grafo nao direcionado" << endl;
        return {};
    }

    vector<char> resultado(0);

    aux_caminhamento_profundidade(id_no, [&resultado, this](No *no, char id_no_seguinte)
                                  { aux_fecho_transitivo_direto(no, resultado); });
    
    //filtrando os nos iguais
    sort(resultado.begin(), resultado.end());
    auto last = unique(resultado.begin(), resultado.end());
    resultado.erase(last, resultado.end());

    return resultado;
}

Grafo* aux_inverte_arestas_grafo(Grafo *arvore) {

    Grafo *grafo_invertido = new Grafo();
    grafo_invertido->in_direcionado = true;

    for(No* n : arvore->lista_adj) {
        grafo_invertido->adicionar_vertice(n->id, 0);
    }

    for(No* n : grafo_invertido->lista_adj) {
        for(Aresta* a : n->arestas) {
            grafo_invertido->adicionar_aresta_grafo(a->id_no_alvo, a->id_no_origem, 0);
        }
    }

    return grafo_invertido;
}

vector<char> Grafo::fecho_transitivo_indireto(char id_no)
{
    if (!in_direcionado)
    {
        cout << "Grafo nao direcionado" << endl;
        return {};
    }
    
    //cria grafo com arestas invertidas
    Grafo* grafo_invertido = aux_inverte_arestas_grafo(this);
    vector<char> resultado(0);

    //com o grafo  invertido, utiliza-se o mesmo metodo do fecho transitivo direto
    aux_caminhamento_profundidade(id_no, [&resultado, grafo_invertido](No *no, char id_no_seguinte)
                                  { aux_fecho_transitivo_direto(no, resultado); });
    
    delete grafo_invertido;

    //filtrando os nos iguais
    sort(resultado.begin(), resultado.end());
    auto last = unique(resultado.begin(), resultado.end());
    resultado.erase(last, resultado.end());

    return resultado;
}

void Grafo::aux_retorna_chars_caminho_dijkstra(vector<char> &vec, char id_atual, char id_no_a)
{
    No *no_atual = get_no(id_atual);
    char id_responsavel = no_atual->dijkstra_responsavel;
    if (id_responsavel != 0)
    {
        if (id_atual != id_no_a)
        {
            aux_retorna_chars_caminho_dijkstra(vec, id_responsavel, id_no_a);
        }

        vec.push_back(no_atual->id);
    } else {
        vec.push_back(0);
    }
}

vector<char> Grafo::caminho_minimo_dijkstra(char id_no_a, char id_no_b)
{
    if (!in_ponderado_aresta)
    {
        cout << "Nao e ponderado nas arestas" << endl;
        return {};
    }

    vector<char> resultante(0);
    vector<No *> nos_abertos(0);

    No *no_inicial = get_no(id_no_a);

    //pegando nó de origem e inserindo na primeira posição
    no_inicial->dijkstra_custo_minimo = 0;
    no_inicial->dijkstra_responsavel = id_no_a;
    no_inicial->dijkstra_fechado = false;

    nos_abertos.push_back(no_inicial);

    //para cada outro nó, inicializa como aberto, responsável = 0 e custo infinito
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

    //até no máximo n iterações
    for (int i = 0; i < lista_adj.size(); i++)
    {
        No *no_custo_minimo = nos_abertos[i]; //pega o primeiro elemento, que é sempre o de custo mínimo(para chegar) e fecha
        no_custo_minimo->dijkstra_fechado = true;

        if (no_custo_minimo->id == id_no_b) //se é o destino, sai
        {
            break;
        }

        for (int j = 0; j < no_custo_minimo->arestas.size(); j++) //para cada aresta do nó recém fechado
        {
            No *no_desatualizado = get_no(no_custo_minimo->arestas[j]->id_no_alvo);
            int custo_para_no_desatualizado = no_custo_minimo->arestas[j]->peso;
            int novo_custo = no_custo_minimo->dijkstra_custo_minimo + custo_para_no_desatualizado; //calcula novo custo para o nó de destino

            if (!no_desatualizado->dijkstra_fechado) //se estiver aberto
            {
                if (novo_custo < no_desatualizado->dijkstra_custo_minimo) //se o custo compensar
                {
                    //atualiza custo mínimo e responsável
                    no_desatualizado->dijkstra_custo_minimo = novo_custo;
                    no_desatualizado->dijkstra_responsavel = no_custo_minimo->id;
                }
            }
        }

        //ordena para deixar o menor no início
        sort(nos_abertos.begin(), nos_abertos.end(), [](No* no1, No* no2){
            return no1->dijkstra_custo_minimo < no2->dijkstra_custo_minimo;
        });
    }

    aux_retorna_chars_caminho_dijkstra(resultante, id_no_b, id_no_a); //insere os chars no vetor resultante

    return resultante;
}

void Grafo::imprimir_matrizes_floyd()
{
    int n = lista_adj.size();

    cout << "\n=== MATRIZ DE DISTÂNCIAS ===" << endl;
    cout << "    ";
    for (int j = 0; j < n; j++)
        cout << lista_adj[j]->id << "\t";
    cout << endl;

    for (int i = 0; i < n; i++)
    {
        cout << lista_adj[i]->id << " | ";
        for (int j = 0; j < n; j++)
        {
            if (matriz_distancia[i][j] == INT_MAX)
                cout << "INF\t";
            else
                cout << matriz_distancia[i][j] << "\t";
        }
        cout << endl;
    }

    cout << "\n=== MATRIZ DE PREDECESSORES ===" << endl;
    cout << "    ";
    for (int j = 0; j < n; j++)
        cout << lista_adj[j]->id << "\t";
    cout << endl;

    for (int i = 0; i < n; i++)
    {
        cout << lista_adj[i]->id << " | ";
        for (int j = 0; j < n; j++)
        {
            if (matriz_predecessor[i][j] == 0)
                cout << "-\t";
            else
                cout << matriz_predecessor[i][j] << "\t";
        }
        cout << endl;
    }
}

void Grafo::gera_floyd()
{

    // Inicializa a matriz de distância
    int n = lista_adj.size();
    matriz_distancia.assign(n, vector<int>(n, INT_MAX));
    matriz_predecessor.assign(n, vector<char>(n, 0)); // ou '\0'

    // Inicialização
    for (No *no : lista_adj)
    {
        int i = pos_id[no->id];
        matriz_distancia[i][i] = 0;
        matriz_predecessor[i][i] = no->id;

        for (Aresta *aresta : no->arestas)
        {
            int j = pos_id[aresta->id_no_alvo];
            matriz_distancia[i][j] = aresta->peso;
            matriz_predecessor[i][j] = no->id;
        }
    }

    // Algoritmo de Floyd-Warshall
    for (int k = 0; k < n; k++)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (matriz_distancia[i][k] != INT_MAX && matriz_distancia[k][j] != INT_MAX &&
                    matriz_distancia[i][j] > matriz_distancia[i][k] + matriz_distancia[k][j])
                {
                    matriz_distancia[i][j] = matriz_distancia[i][k] + matriz_distancia[k][j];
                    matriz_predecessor[i][j] = matriz_predecessor[k][j];
                }
            }
        }
    }
}

vector<char> Grafo::caminho_minimo_floyd(char id_no_a, char id_no_b)
{
    if (!in_ponderado_aresta)
    {
        cout << "Grafo nao ponderado nas arestas." << endl;
        return {};
    }

    if (!floyd_gerado)
    {
        gera_floyd();
        floyd_gerado = true;
    }

    int idx_a = pos_id[id_no_a];
    int idx_b = pos_id[id_no_b];

    if (matriz_distancia[idx_a][idx_b] == INT_MAX)
    {
        cout << "Nao ha caminho entre os nos." << endl;
        return {};
    }

    vector<char> caminho;

    // Função recursiva para reconstruir o caminho
    function<void(int, int)> reconstruir = [&](int i, int j)
    {
        if (i == j)
        {
            caminho.push_back(lista_adj[i]->id);
        }
        else if (matriz_predecessor[i][j] == 0)
        {
            caminho.clear(); // caminho inválido
        }
        else
        {
            reconstruir(i, pos_id[matriz_predecessor[i][j]]);
            caminho.push_back(lista_adj[j]->id);
        }
    };

    reconstruir(idx_a, idx_b);
    return caminho;
}

Aresta *Grafo::aux_aresta_custo_minimo_grafo(Grafo *grafo, vector<char> *ids_nos, No **u, No **v)
{
    Aresta *menor_aresta = new Aresta();
    menor_aresta->peso = numeric_limits<int>::max();

    for (int i = 0; i < ids_nos->size(); i++)
    {
        No *no_temporario = grafo->get_no((*ids_nos)[i]);
        for (int j = 0; j < no_temporario->arestas.size(); j++)
        {
            if (no_temporario->arestas[j]->peso < menor_aresta->peso)
            {
                menor_aresta = grafo->lista_adj[i]->arestas[j];
                *u = grafo->get_no(no_temporario->id);
                *v = grafo->get_no(no_temporario->arestas[j]->id_no_alvo);
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

Grafo *Grafo::gerar_subgrafo(vector<char> ids_nos)
{
    Grafo *subgrafo = new Grafo(ids_nos.size(), 0, 1, 1);

    for (int i = 0; i < ids_nos.size(); i++)
    {
        No *no = get_no(ids_nos[i]);
        subgrafo->adicionar_vertice(no->id, no->peso);

        for (Aresta *aresta : no->arestas)
        {
            if (find(ids_nos.begin(), ids_nos.end(), aresta->id_no_alvo) != ids_nos.end())
            {
                subgrafo->adicionar_aresta_grafo(no->id, aresta->id_no_alvo, aresta->peso);
            }
        }
    }

    return subgrafo;
}

Grafo *Grafo::arvore_geradora_minima_prim(vector<char> ids_nos)
{
    if (!in_ponderado_aresta || in_direcionado)
    {
        cout << "Grafo nao ponderado nas arestas ou direcionado." << endl;
    }

    int arestas_peso_infinito = 0;
    // grafo com os vértices pedidos
    Grafo *subgrafo = gerar_subgrafo(ids_nos);

    // iniciando grafo resultante
    Grafo *arvore_prim = new Grafo(ids_nos.size(), 0, 1, 1);
    if (ids_nos.size() < 2)
    {
        arvore_prim->adicionar_vertice(ids_nos[0], subgrafo->get_no(ids_nos[0])->peso);
        return arvore_prim;
    }

    // iniciando vetor de prox
    vector<char> prox(ids_nos.size());
    vector<int> custo(ids_nos.size());
    No *u = new No();
    No *v = new No();

    Aresta *aresta_inicial = aux_aresta_custo_minimo_grafo(subgrafo, &ids_nos, &u, &v); // pegando aresta inicial

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
            Aresta *aresta_u = aux_tem_aresta_para(subgrafo->get_no(ids_nos[i]), u->id);
            Aresta *menor_aresta = aresta_u;
            Aresta *aresta_v = aux_tem_aresta_para(subgrafo->get_no(ids_nos[i]), v->id);

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
        arvore_prim->adicionar_vertice(ids_nos[idx_aresta_minima], subgrafo->get_no(ids_nos[idx_aresta_minima])->peso);
        arvore_prim->adicionar_aresta_grafo(ids_nos[idx_aresta_minima], prox[idx_aresta_minima], custo[idx_aresta_minima]);

        No *no_recem_adicionado = subgrafo->get_no(ids_nos[idx_aresta_minima]);

        for (int j = 0; j < no_recem_adicionado->arestas.size(); j++)
        {                                                                                          // para cada aresta do nó adicionado
            No *novo_no_acessivel = subgrafo->get_no(no_recem_adicionado->arestas[j]->id_no_alvo); // pega o id alvo
            Aresta *aresta = aux_tem_aresta_para(novo_no_acessivel, prox[idx_aresta_minima]);      // se tem aresta para o grafo resultante

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

char find_sets(map<char, char> &parent, char v)
{
    if (parent[v] == v)
        return v;
    return parent[v] = find_sets(parent, parent[v]);
}

void unite_sets(map<char, char> &parent, char u, char v)
{
    parent[find_sets(parent, u)] = find_sets(parent, v);
}

Grafo *Grafo::arvore_geradora_minima_kruskal(vector<char> ids_nos)
{
    if (!in_ponderado_aresta || in_direcionado)
    {
        cout << "Grafo nao ponderado nas arestas ou direcionado." << endl;
        return nullptr;
    }

    // Criar o grafo resultado
    Grafo *agm = new Grafo(ids_nos.size(), false, true, in_ponderado_vertice);

    // Adiciona os vértices no grafo resultado
    for (char id : ids_nos)
    {
        No *original = get_no(id);
        int peso_vertice = (original != nullptr) ? original->peso : 0;
        agm->adicionar_vertice(id, peso_vertice);
    }

    vector<Aresta *> arestas_ordenadas;

    // Coletando as arestas, evitando duplicatas
    for (No *no : lista_adj)
    {
        // Verifica se o nó está no subconjunto agm
        if (agm->get_no(no->id) == nullptr)
            continue;

        for (Aresta *aresta : no->arestas)
        {
            // verifica se a aresta já existe e se o nó está no subconjunto agm
            if (no->id < aresta->id_no_alvo && agm->get_no(no->id) != nullptr && agm->get_no(aresta->id_no_alvo) != nullptr)
            {
                arestas_ordenadas.push_back(aresta);
            }
        }
    }

    // Ordenando pelo peso
    sort(arestas_ordenadas.begin(), arestas_ordenadas.end(),
         [](Aresta *a, Aresta *b)
         { return a->peso < b->peso; });

    // Imprime as arestas ordenadas
    cout << "Arestas ordenadas por peso:" << endl;
    for (Aresta *aresta : arestas_ordenadas)
    {
        cout << "Aresta: " << aresta->id_no_origem << " - " << aresta->id_no_alvo << " | Peso: " << aresta->peso << endl;
    }

    // Inicializando Union-Find
    map<char, char> parent;
    for (char id : ids_nos)
    {
        parent[id] = id;
    }

    int arestas_adicionadas = 0;
    for (Aresta *aresta : arestas_ordenadas)
    {
        char u = aresta->id_no_origem;
        char v = aresta->id_no_alvo;

        if (find_sets(parent, u) != find_sets(parent, v))
        {
            agm->adicionar_aresta_grafo(u, v, aresta->peso);
            unite_sets(parent, u, v);
            arestas_adicionadas++;

            if (arestas_adicionadas == ids_nos.size() - 1)
                break; // Árvore geradora mínima completa
        }
    }

    return agm;
}

void Grafo::aux_caminhamento_profundidade(char id_no, function<void(No *, char)> funcao_caminhamento = [](No *, char) {})
{
    No *no_atual = get_no(id_no);
    No *no_seguinte;
    int id_no_seguinte;

    no_atual->no_visitado = true;

    for (int i = 0; i < no_atual->arestas.size(); i++)
    {

        id_no_seguinte = no_atual->arestas[i]->id_no_alvo;
        no_seguinte = get_no(id_no_seguinte);
        if (no_seguinte->no_visitado == false)
        {
            funcao_caminhamento(no_atual, id_no_seguinte);
            aux_caminhamento_profundidade(id_no_seguinte, funcao_caminhamento);
            no_atual->no_visitado = false;
        }
    }
}

void aux_insere_aresta_grafo(Grafo *arvore, No *no, char id_no_seguinte)
{
    arvore->adicionar_vertice(no->id, 0);
    arvore->adicionar_aresta_grafo(no->id, id_no_seguinte, 0);
}

Grafo *Grafo::arvore_caminhamento_profundidade(char id_no)
{
    Grafo *arvore_profundidade = new Grafo();

    aux_caminhamento_profundidade(id_no, [&arvore_profundidade, this](No *no, char id_no_seguinte)
                                  { aux_insere_aresta_grafo(arvore_profundidade, no, id_no_seguinte); });

    return arvore_profundidade;
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
