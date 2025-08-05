#include "AlgoritmosGulosos.h"
#include <vector>
#include <set>
#include <algorithm>
#include <random>
#include <chrono>

/**
 * @brief Implementa o algoritmo guloso padrão para encontrar um conjunto dominante em um grafo.
 * @param grafo Ponteiro para o grafo de entrada.
 * @return Ponteiro para o grafo resultante contendo o conjunto dominante.
 */

Grafo *AlgoritmosGulosos::conjunto_dominante(Grafo *grafo)
{
    std::set<No *> nos_nao_cobertos;
    for (No *no : grafo->lista_adj)
    {
        nos_nao_cobertos.insert(no);
    }

    int nos_max = grafo->ordem;
    Grafo *solucao = new Grafo(nos_max);

    while (!nos_nao_cobertos.empty())
    {
        No *melhor_no_candidato = nullptr;
        int max_cobertura = -1;

        for (No *no_candidato : grafo->lista_adj)
        {
            int cobertura_atual = 0;

            if (nos_nao_cobertos.count(no_candidato))
            {
                cobertura_atual++;
            }

            for (Aresta *aresta : no_candidato->arestas)
            {
                No *vizinho = grafo->get_no(aresta->id_no_alvo);
                if (nos_nao_cobertos.count(vizinho))
                {
                    cobertura_atual++;
                }
            }

            if (cobertura_atual > max_cobertura)
            {
                max_cobertura = cobertura_atual;
                melhor_no_candidato = no_candidato;
            }
        }

        if (melhor_no_candidato != nullptr)
        {
            solucao->adicionar_vertice(melhor_no_candidato->id);

            nos_nao_cobertos.erase(melhor_no_candidato);
            for (Aresta *aresta : melhor_no_candidato->arestas)
            {
                No *vizinho = grafo->get_no(aresta->id_no_alvo);
                nos_nao_cobertos.erase(vizinho);
            }
        }
    }

    solucao->ordem = solucao->lista_adj.size();

    return solucao;
}

/** 
 * @brief Implementa o algoritmo guloso randomizado para encontrar um conjunto dominante em um grafo.
 * @param grafo Ponteiro para o grafo de entrada.
 * @param alfa Fator de aleatoriedade (entre 0 e 1).
 * @return Ponteiro para o grafo resultante contendo o conjunto dominante.
 */

Grafo *AlgoritmosGulosos::conjunto_dominante_randomizado(Grafo *grafo, float alfa)
{
    alfa = std::max(0.0f, std::min(1.0f, alfa));

    std::set<No *> nos_nao_cobertos;
    for (No *no : grafo->lista_adj)
    {
        nos_nao_cobertos.insert(no);
    }

    int nos_max = grafo->ordem;
    Grafo *solucao = new Grafo(nos_max);

    std::mt19937 gerador(std::chrono::system_clock::now().time_since_epoch().count());

    while (!nos_nao_cobertos.empty())
    {
        std::vector<std::pair<No *, int>> candidatos_com_utilidade;

        for (No *no_candidato : grafo->lista_adj)
        {
            int cobertura_atual = 0;
            if (nos_nao_cobertos.count(no_candidato))
            {
                cobertura_atual++;
            }
            for (Aresta *aresta : no_candidato->arestas)
            {
                No *vizinho = grafo->get_no(aresta->id_no_alvo);
                if (nos_nao_cobertos.count(vizinho))
                {
                    cobertura_atual++;
                }
            }

            if (cobertura_atual > 0)
            {
                candidatos_com_utilidade.push_back({no_candidato, cobertura_atual});
            }
        }

        if (candidatos_com_utilidade.empty())
        {
            break;
        }

        std::sort(candidatos_com_utilidade.begin(), candidatos_com_utilidade.end(),
                  [](const auto &a, const auto &b)
                  {
                      return a.second > b.second;
                  });

        int tamanho_lcr = static_cast<int>(std::ceil(alfa * candidatos_com_utilidade.size()));
        tamanho_lcr = std::max(1, tamanho_lcr);
        tamanho_lcr = std::min(tamanho_lcr, (int)candidatos_com_utilidade.size());

        std::uniform_int_distribution<> dist(0, tamanho_lcr - 1);
        int idx_aleatorio = dist(gerador);
        No *no_escolhido = candidatos_com_utilidade[idx_aleatorio].first;

        solucao->adicionar_vertice(no_escolhido->id);

        nos_nao_cobertos.erase(no_escolhido);
        for (Aresta *aresta : no_escolhido->arestas)
        {
            No *vizinho = grafo->get_no(aresta->id_no_alvo);
            nos_nao_cobertos.erase(vizinho);
        }
    }

    solucao->ordem = solucao->lista_adj.size();

    return solucao;
}

/**
 * @brief Executa o algoritmo guloso randomizado 'n' vezes e retorna a melhor solução.
 *
 * @param grafo O grafo de entrada para o qual o conjunto dominante será encontrado.
 * @param n_iteracoes O número de vezes que o algoritmo randomizado será executado.
 * @param alfa O parâmetro de aleatoriedade (0 <= alfa <= 1) a ser usado em cada execução.
 * @return Um ponteiro para o melhor objeto Grafo encontrado entre todas as iterações.
 */

Grafo* AlgoritmosGulosos::executar_randomizado_n_vezes(Grafo* grafo, int n_iteracoes, float alfa)
{
    Grafo* melhor_solucao_encontrada = nullptr;
    int menor_tamanho = -1;

    std::cout << "Executando o algoritmo randomizado " << n_iteracoes << " vezes com alfa=" << alfa << "..." << std::endl;

    for (int i = 0; i < n_iteracoes; ++i)
    {
        Grafo* solucao_atual = conjunto_dominante_randomizado(grafo, alfa);

        if (melhor_solucao_encontrada == nullptr || solucao_atual->ordem < menor_tamanho)
        {
            if (melhor_solucao_encontrada != nullptr)
            {
                delete melhor_solucao_encontrada;
            }

            melhor_solucao_encontrada = solucao_atual;
            menor_tamanho = solucao_atual->ordem;

            std::cout << "  Iteracao " << (i + 1) << ": Nova melhor solucao encontrada com tamanho " << menor_tamanho << std::endl;
        }
        else
        {
            delete solucao_atual;
        }
    }
    
    std::cout << "Execucao finalizada. Melhor tamanho de conjunto dominante encontrado: " << menor_tamanho << std::endl;

    return melhor_solucao_encontrada;
}

// --- Algoritmo Guloso Reativo ---

void aux_calcula_probabilidades(vector<float> probabilidades, vector<float> medias, int m, float melhor_qualidade)
{
    float somatorio_qualidades = 0;
    float qualidades[m];

    for (int i = 0; i < m; i++)
    {
        qualidades[i] = medias[i] != 0 ? (melhor_qualidade / medias[i]) : qualidades[i];
        somatorio_qualidades += qualidades[i];
    }

    if (somatorio_qualidades != 0)
        for (int i = 0; i < m; i++)
        {
            probabilidades[i] = qualidades[i] / somatorio_qualidades;
        }
}

int aux_escolhe_idx_alfa(mt19937 &gerador, vector<float> probabilidades, int m)
{
    float soma = 0;

    uniform_real_distribution<> dist(0.0, 1.0);
    float aleat = dist(gerador);
    cout << "al: " << aleat << endl;

    int i = 0;
    while (i < m)
    {
        cout << "p: " << probabilidades[i] << endl;
        cout << soma << endl;
        soma += probabilidades[i];

        if (soma >= aleat)
            break;
        i++;
    }

    return i;
}

int aux_gerar_indice_alfa(mt19937 &gerador, int min, int max, float alfa)
{
    uniform_int_distribution<> dist(min, static_cast<int>(alfa * max));
    return dist(gerador);
}

void quicksort_nos(vector<No *> &vec, function<bool(No *, No *)> comp, int inicio, int fim)
{
    if (inicio >= fim)
        return;

    No *pivo = vec[fim];
    int idx = inicio;

    for (int i = inicio; i < fim; i++)
    {
        if (comp(vec[i], pivo))
        {
            swap(vec[i], vec[idx]);
            idx++;
        }
    }

    swap(vec[idx], vec[fim]);

    quicksort_nos(vec, comp, inicio, idx - 1);
    quicksort_nos(vec, comp, idx + 1, fim);
}

void quicksort_nos(vector<No *> &vec, function<bool(No *, No *)> comp)
{
    if (!vec.empty())
    {
        quicksort_nos(vec, comp, 0, vec.size() - 1);
    }

    auto it = find_if(vec.rbegin(), vec.rend(), [](No *no)
                      { return no->dominante_conexo_nos_uteis.size() != 0; });

    auto cvt_it = it.base();
    if (cvt_it != vec.end())
    {
        // Apaga do zero até o fim
        vec.erase(cvt_it, vec.end());
    }
}

bool comp_nos(No *a, No *b)
{
    if (a->dominante_conexo_nos_uteis.size() > b->dominante_conexo_nos_uteis.size())
        return true;
    else
        return false;
}

/*
@brief Implementa o algoritmo guloso reativo para encontrar um conjunto dominante em um grafo.
@param grafo Ponteiro para o grafo de entrada.
@param alfas Vetor de fatores de aleatoriedade (entre 0 e 1).
@param iteracoes Número de iterações a serem realizadas.
@param tamanho_bloco Tamanho do bloco para recalcular as probabilidades.
@return Ponteiro para o grafo resultante contendo o conjunto dominante.
*/

Grafo *AlgoritmosGulosos::conjunto_dominante_reativo(Grafo *grafo, vector<float> &alfas, int iteracoes, int tamanho_bloco)
{
    int m = alfas.size();
    float p_inicial = 1 / static_cast<float>(m);
    cout << p_inicial << endl;
    vector<float> probabilidades(m, p_inicial);
    vector<float> medias(m, 0);
    float melhor_qualidade = 0;

    vector<No *> &lista_adj = grafo->lista_adj;

    // Inicializando

    for (int i = 0; i < iteracoes; i++)
    {
        vector<No *> nos_candidatos(0);
        vector<No *> nos_solucao(0);
        int nos_envolvidos = 1;
        int nos_dominantes = 0;
        mt19937 gerador(chrono::system_clock::now().time_since_epoch().count());

        if (i % tamanho_bloco == 0) // controla número de iterações antes de recalcular
        {
            aux_calcula_probabilidades(probabilidades, medias, m, melhor_qualidade); // recalcula chances
        }

        int idx_alfa = aux_escolhe_idx_alfa(gerador, probabilidades, m);
        float alfa = alfas[idx_alfa];
        cout << "idc: " << idx_alfa << endl;
        cout << "alfa: " << alfas[0] << endl;
        // Inicializando
        for (No *no : lista_adj)
        {
            no->dominante_conexo_candidato = false;
            no->dominante_conexo_dominante = false;
            for (Aresta *aresta : no->arestas)
            {
                no->dominante_conexo_nos_uteis.push_back(aresta->id_no_alvo);
            }
            nos_candidatos.push_back(no);
        }

        // ordenando todos os nós para pegar um aleatório e começar o algoritmo
        quicksort_nos(nos_candidatos, comp_nos);
        // sorteando
        int idx_no_inicial = aux_gerar_indice_alfa(gerador, 0, nos_candidatos.size() - 1, alfa);
        cout << "gerou indice: " << idx_no_inicial << endl;
        No *no_inicial = nos_candidatos[idx_no_inicial];
        no_inicial->dominante_conexo_candidato = true;
        nos_candidatos.resize(0);
        nos_candidatos.push_back(no_inicial);

        while ((nos_envolvidos < lista_adj.size()) && nos_candidatos.size() != 0)
        {
            No *novo_dominante = new No();
            int idx_aleat = aux_gerar_indice_alfa(gerador, 0, nos_candidatos.size() - 1, alfa);
            cout << "idx: " << idx_aleat << endl;
            novo_dominante = nos_candidatos[idx_aleat];
            novo_dominante->dominante_conexo_dominante = true;
            novo_dominante->dominante_conexo_nos_uteis.resize(0);
            cout << "adicionou no: " << novo_dominante->id << endl;
            nos_dominantes++;

            for (Aresta *aresta : novo_dominante->arestas)
            {
                char indice_no_alvo = aresta->id_no_alvo;
                No *no_alvo = grafo->get_no(indice_no_alvo);

                no_alvo->remover_no_util(novo_dominante->id); // diminui o grau útil do nó
                if (!no_alvo->dominante_conexo_candidato)
                {
                    nos_candidatos.push_back(no_alvo);
                    cout << "candidato: " << no_alvo->id << endl;
                    no_alvo->dominante_conexo_candidato = true;

                    nos_envolvidos++;
                    for (Aresta *aresta : no_alvo->arestas)
                    {
                        char indice_no_alvo = aresta->id_no_alvo;
                        No *no = grafo->get_no(indice_no_alvo);
                        no->remover_no_util(no_alvo->id);
                    }
                }
            }
            // ordenando candidatos(nós já adicionados vão pro final)
            quicksort_nos(nos_candidatos, comp_nos);

            for (No *no : nos_candidatos)
            {
                cout << no->dominante_conexo_nos_uteis.size() << " ";
            }
            cout << endl;
        }

        cout << "nos dominantes: " << nos_dominantes << endl;
    }

    return new Grafo(4, 1, 1, 0);
}