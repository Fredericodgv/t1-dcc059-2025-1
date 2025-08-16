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

std::vector<char> AlgoritmosGulosos::conjunto_dominante_conexo(Grafo *grafo)
{
    if (grafo->lista_adj.empty())
        return {};

    std::set<No *> nos_nao_cobertos;
    for (No *no : grafo->lista_adj)
    {
        if (no)
            nos_nao_cobertos.insert(no);
    }

    std::vector<char> solucao;
    std::set<No *> candidatos;

    No *no_inicial = nullptr;
    int max_grau = -1;
    for (No *no : grafo->lista_adj)
    {
        if (no && (int)no->arestas.size() > max_grau)
        {
            max_grau = no->arestas.size();
            no_inicial = no;
        }
    }

    if (!no_inicial)
        return {};

    solucao.push_back(no_inicial->id);
    nos_nao_cobertos.erase(no_inicial);
    for (Aresta *aresta : no_inicial->arestas)
    {
        No *vizinho = grafo->get_no(aresta->id_no_alvo);
        nos_nao_cobertos.erase(vizinho);
        if (vizinho)
            candidatos.insert(vizinho);
    }

    while (!nos_nao_cobertos.empty() && !candidatos.empty())
    {
        No *melhor_no_candidato = nullptr;
        int max_cobertura = -1;

        for (No *no_candidato : candidatos)
        {
            int cobertura_atual = 0;
            if (nos_nao_cobertos.count(no_candidato))
                cobertura_atual++;
            for (Aresta *aresta : no_candidato->arestas)
            {
                if (nos_nao_cobertos.count(grafo->get_no(aresta->id_no_alvo)))
                    cobertura_atual++;
            }

            if (cobertura_atual > max_cobertura)
            {
                max_cobertura = cobertura_atual;
                melhor_no_candidato = no_candidato;
            }
        }

        if (!melhor_no_candidato || max_cobertura == 0)
            break;

        solucao.push_back(melhor_no_candidato->id);
        candidatos.erase(melhor_no_candidato);
        nos_nao_cobertos.erase(melhor_no_candidato);

        for (Aresta *aresta : melhor_no_candidato->arestas)
        {
            No *vizinho = grafo->get_no(aresta->id_no_alvo);
            nos_nao_cobertos.erase(vizinho);

            bool na_solucao = (std::find(solucao.begin(), solucao.end(), vizinho->id) != solucao.end());
            if (vizinho && !na_solucao)
            {
                candidatos.insert(vizinho);
            }
        }
    }
    return solucao;
}

/**
 * @brief Implementa o algoritmo guloso randomizado para encontrar um conjunto dominante em um grafo.
 * @param grafo Ponteiro para o grafo de entrada.
 * @param alfa Fator de aleatoriedade (entre 0 e 1).
 * @return Ponteiro para o grafo resultante contendo o conjunto dominante.
 */

std::vector<char> AlgoritmosGulosos::conjunto_dominante_randomizado(Grafo *grafo, float alfa)
{
    alfa = std::max(0.0f, std::min(1.0f, alfa));
    if (grafo->lista_adj.empty())
        return {};

    std::set<No *> nos_nao_cobertos;
    for (No *no : grafo->lista_adj)
    {
        if (no)
            nos_nao_cobertos.insert(no);
    }

    std::vector<char> solucao;
    std::set<No *> candidatos;
    std::mt19937 gerador(std::chrono::system_clock::now().time_since_epoch().count());

    // Cria lista para selecionar o nó inicial
    std::vector<std::pair<No *, int>> candidatos_iniciais;
    for (No *no : grafo->lista_adj)
    {
        if (no)
        {
            candidatos_iniciais.push_back({no, (int)no->arestas.size()});
        }
    }

    // Ordena candidatos iniciais por grau
    std::sort(candidatos_iniciais.begin(), candidatos_iniciais.end(),
              [](const auto &a, const auto &b)
              { return a.second > b.second; });

    // Seleciona o nó inicial aleatoriamente
    int tamanho_lcr_inicial = static_cast<int>(std::ceil(alfa * candidatos_iniciais.size()));
    tamanho_lcr_inicial = std::max(1, tamanho_lcr_inicial);
    tamanho_lcr_inicial = std::min(tamanho_lcr_inicial, (int)candidatos_iniciais.size());

    // Seleciona um nó inicial aleatório entre os candidatos iniciais
    std::uniform_int_distribution<> dist_inicial(0, tamanho_lcr_inicial - 1);
    No *no_inicial = candidatos_iniciais[dist_inicial(gerador)].first;

    // Adiciona o nó inicial à solução
    solucao.push_back(no_inicial->id);
    nos_nao_cobertos.erase(no_inicial);

    // Adiciona os vizinhos do nó inicial aos candidatos e remove eles dos não cobertos
    for (Aresta *aresta : no_inicial->arestas)
    {
        No *vizinho = grafo->get_no(aresta->id_no_alvo);
        nos_nao_cobertos.erase(vizinho);
        if (vizinho)
            candidatos.insert(vizinho);
    }

    // Enquanto houver nós não cobertos e candidatos disponíveis
    while (!nos_nao_cobertos.empty() && !candidatos.empty())
    {
        std::vector<std::pair<No *, int>> candidatos_com_utilidade;

        // Calcula a utilidade de cada candidato
        for (No *no_candidato : candidatos)
        {
            int cobertura_atual = 0;
            if (nos_nao_cobertos.count(no_candidato))
                cobertura_atual++;
            for (Aresta *aresta : no_candidato->arestas)
            {
                if (nos_nao_cobertos.count(grafo->get_no(aresta->id_no_alvo)))
                    cobertura_atual++;
            }
            if (cobertura_atual > 0)
            {
                candidatos_com_utilidade.push_back({no_candidato, cobertura_atual});
            }
        }

        // Se não houver candidatos com utilidade, sai do loop
        if (candidatos_com_utilidade.empty())
            break;

        // Ordena candidatos por utilidade
        std::sort(candidatos_com_utilidade.begin(), candidatos_com_utilidade.end(),
                  [](const auto &a, const auto &b)
                  { return a.second > b.second; });

        // Seleciona um candidato aleatório baseado na utilidade
        int tamanho_lcr = static_cast<int>(std::ceil(alfa * candidatos_com_utilidade.size()));
        tamanho_lcr = std::max(1, tamanho_lcr);
        tamanho_lcr = std::min(tamanho_lcr, (int)candidatos_com_utilidade.size());

        std::uniform_int_distribution<> dist(0, tamanho_lcr - 1);
        No *no_escolhido = candidatos_com_utilidade[dist(gerador)].first;

        // Adiciona o nó escolhido à solução
        solucao.push_back(no_escolhido->id);
        candidatos.erase(no_escolhido);
        nos_nao_cobertos.erase(no_escolhido);
        for (Aresta *aresta : no_escolhido->arestas)
        {
            No *vizinho = grafo->get_no(aresta->id_no_alvo);
            nos_nao_cobertos.erase(vizinho);

            bool na_solucao = (std::find(solucao.begin(), solucao.end(), vizinho->id) != solucao.end());
            if (vizinho && !na_solucao)
            {
                candidatos.insert(vizinho);
            }
        }
    }
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

std::vector<char> AlgoritmosGulosos::executar_randomizado_n_vezes(Grafo *grafo, int n_iteracoes, float alfa)
{
    std::vector<char> melhor_solucao_encontrada;

    std::cout << "Executando o algoritmo RANDOMIZADO " << n_iteracoes << " vezes..." << std::endl;

    for (int i = 0; i < n_iteracoes; ++i)
    {
        // std::cout << "\n--- Execucao Randomizada Principal " << (i + 1) << "/" << n_iteracoes << " ---" << std::endl;

        std::vector<char> solucao_atual = conjunto_dominante_randomizado(grafo, alfa);

        if (melhor_solucao_encontrada.empty() || solucao_atual.size() < melhor_solucao_encontrada.size())
        {
            melhor_solucao_encontrada = solucao_atual;

            // std::cout << "  --> Nova melhor solucao geral encontrada com tamanho " << melhor_solucao_encontrada.size() << std::endl;
        }
        else
        {
            // std::cout << "  --> Solucao encontrada (" << solucao_atual.size()
            //           << ") nao superou a melhor (" << melhor_solucao_encontrada.size() << "). Descartando." << std::endl;
        }
    }

    if (!melhor_solucao_encontrada.empty())
    {
        // std::cout << "\nExecucao finalizada. Melhor tamanho de conjunto dominante encontrado: " << melhor_solucao_encontrada.size() << std::endl;
    }
    else
    {
        // std::cout << "\nExecucao finalizada. Nenhuma solucao valida foi encontrada." << std::endl;
    }

    return melhor_solucao_encontrada;
}

// --- Algoritmo Guloso Reativo ---

std::vector<float> aux_calcula_probabilidades(const std::vector<float> &medias, int m)
{
    const float prob_minima = 0.05;

    std::vector<float> novas_probabilidades(m);
    float somatorio_medias = 0.0f;

    for (int i = 0; i < m; i++)
    {
        somatorio_medias += medias[i];
    }

    if (somatorio_medias > 0)
    {
        for (int i = 0; i < m; i++)
        {
            novas_probabilidades[i] = medias[i] / somatorio_medias;
        }

        float somatorio_ajustado = 0;
        for (int i = 0; i < m; i++)
        {
            if (novas_probabilidades[i] < prob_minima)
            {
                novas_probabilidades[i] = prob_minima;
            }
            somatorio_ajustado += novas_probabilidades[i];
        }

        for (int i = 0; i < m; i++)
        {
            novas_probabilidades[i] /= somatorio_ajustado;
        }
    }
    else
    {
        for (int i = 0; i < m; i++)
        {
            novas_probabilidades[i] = 1.0f / m;
        }
    }

    return novas_probabilidades;
}

int aux_escolhe_idx_alfa(mt19937 &gerador, vector<float> probabilidades, int m)
{
    float soma = 0;

    uniform_real_distribution<> dist(0.0, 1.0);
    float aleat = dist(gerador);

    int i = 0;
    while (i < m)
    {
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

bool comp_nos(No *a, No *b)
{
    if (a->dominante_conexo_nos_uteis.size() > b->dominante_conexo_nos_uteis.size())
        return true;
    else
        return false;
}

/**
 * @brief Executa o algoritmo guloso reativo 'n' vezes e retorna a melhor solução.
 *
 *
 * @param grafo O grafo de entrada.
 * @param n_execucoes O número de vezes que o algoritmo reativo completo será executado.
 * @param alfas O vetor de valores alfa a ser usado pelo algoritmo reativo.
 * @param iteracoes_reativo O número de iterações INTERNAS do laço reativo.
 * @param tamanho_bloco O tamanho do bloco para atualização de probabilidades no reativo.
 * @return Um ponteiro para o melhor objeto Grafo encontrado entre todas as execuções.
 */
std::vector<char> AlgoritmosGulosos::executar_reativo_n_vezes(Grafo *grafo, int n_execucoes, std::vector<float> &alfas, int iteracoes_reativo, int tamanho_bloco)
{
    std::vector<char> melhor_solucao_encontrada;
    double media_solucoes = 0.0;

    std::cout << "Executando o algoritmo REATIVO " << n_execucoes << " vezes..." << std::endl;

    for (int i = 0; i < n_execucoes; ++i)
    {
        // std::cout << "\n--- Execucao Reativa Principal " << (i + 1) << "/" << n_execucoes << " ---" << std::endl;

        std::vector<char> solucao_atual = conjunto_dominante_reativo(grafo, alfas, iteracoes_reativo, tamanho_bloco);
        media_solucoes += solucao_atual.size()*1.0;

        if (melhor_solucao_encontrada.empty() || solucao_atual.size() < melhor_solucao_encontrada.size())
        {
            melhor_solucao_encontrada = solucao_atual;

            // std::cout << "  --> Nova melhor solucao geral encontrada com tamanho " << melhor_solucao_encontrada.size() << std::endl;
        }
        else
        {
            // std::cout << "  --> Solucao encontrada (" << solucao_atual.size() << ") nao superou a melhor (" << melhor_solucao_encontrada.size() << "). Descartando." << std::endl;
            solucao_atual.clear();
        }
    }

    // std::cout << "\nExecucao finalizada. Melhor tamanho de conjunto dominante encontrado: " << melhor_solucao_encontrada.size() << std::endl;
    std::cout << "\nMedia de solucoes: " << media_solucoes / n_execucoes << std::endl;

    return melhor_solucao_encontrada;
}

/**
 * @brief Implementa o algoritmo guloso reativo para encontrar um conjunto dominante em um grafo.
 * @param grafo Ponteiro para o grafo de entrada.
 * @param alfas Vetor de fatores de aleatoriedade (entre 0 e 1).
 * @param iteracoes Número de iterações a serem realizadas.
 * @param tamanho_bloco Tamanho do bloco para recalcular as probabilidades.
 * @return Ponteiro para o grafo resultante contendo o conjunto dominante.
 */

std::vector<char> AlgoritmosGulosos::conjunto_dominante_reativo(Grafo *grafo, vector<float> &alfas, int iteracoes, int tamanho_bloco)
{
    int m = alfas.size();
    float p_inicial = 1 / static_cast<float>(m);
    vector<float> probabilidades(m, p_inicial);
    vector<float> medias(m, 0);
    vector<int> quantidade_usos_alfas(m, 0);
    std::vector<char> melhor_solucao_geral;
    float melhor_qualidade = 0.0f;

    for (int i = 0; i < iteracoes; i++)
    {
        mt19937 gerador(chrono::system_clock::now().time_since_epoch().count());

        int idx_alfa = aux_escolhe_idx_alfa(gerador, probabilidades, m);
        float alfa_escolhido = alfas[idx_alfa];

        std::vector<char> solucao_nesta_iteracao = conjunto_dominante_randomizado(grafo, alfa_escolhido); // Corrigido para usar 'alfa'

        if (solucao_nesta_iteracao.empty())
        {
            cout << "Nenhum no encontrado na iteracao " << i + 1 << ". Descartando." << std::endl;
            continue;
        }

        float qualidade_atual = 1.0f / static_cast<float>(solucao_nesta_iteracao.size());

        medias[idx_alfa] = (medias[idx_alfa] * quantidade_usos_alfas[idx_alfa] + qualidade_atual) / (quantidade_usos_alfas[idx_alfa] + 1);
        quantidade_usos_alfas[idx_alfa]++;

        if (melhor_solucao_geral.empty() || solucao_nesta_iteracao.size() < melhor_solucao_geral.size())
        {
            melhor_solucao_geral = solucao_nesta_iteracao;
            melhor_qualidade = 1.0f / static_cast<float>(melhor_solucao_geral.size());
        }

        if (i > 0 && i % tamanho_bloco == 0)
        {
            probabilidades = aux_calcula_probabilidades(medias, m);
        }
    }

    return melhor_solucao_geral;
}