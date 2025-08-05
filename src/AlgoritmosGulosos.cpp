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

std::vector<char> AlgoritmosGulosos::conjunto_dominante(Grafo *grafo)
{
    std::set<No *> nos_nao_cobertos;
    for (No *no : grafo->lista_adj)
    {
        nos_nao_cobertos.insert(no);
    }

    int nos_max = grafo->ordem;
    std::vector<char> solucao;

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
            solucao.push_back(melhor_no_candidato->id);

            nos_nao_cobertos.erase(melhor_no_candidato);
            for (Aresta *aresta : melhor_no_candidato->arestas)
            {
                No *vizinho = grafo->get_no(aresta->id_no_alvo);
                nos_nao_cobertos.erase(vizinho);
            }
        }
    }

    solucao.resize(solucao.size());

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

    std::set<No *> nos_nao_cobertos;
    for (No *no : grafo->lista_adj)
    {
        nos_nao_cobertos.insert(no);
    }

    int nos_max = grafo->ordem;
    std::vector<char> solucao;

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

        solucao.push_back(no_escolhido->id);

        nos_nao_cobertos.erase(no_escolhido);
        for (Aresta *aresta : no_escolhido->arestas)
        {
            No *vizinho = grafo->get_no(aresta->id_no_alvo);
            nos_nao_cobertos.erase(vizinho);
        }
    }

    solucao.resize(solucao.size());

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
        std::cout << "\n--- Execucao Randomizada Principal " << (i + 1) << "/" << n_iteracoes << " ---" << std::endl;

        std::vector<char> solucao_atual = conjunto_dominante_randomizado(grafo, alfa);

        if (melhor_solucao_encontrada.empty() || solucao_atual.size() < melhor_solucao_encontrada.size())
        {
            melhor_solucao_encontrada = solucao_atual;

            std::cout << "  --> Nova melhor solucao geral encontrada com tamanho " << melhor_solucao_encontrada.size() << std::endl;
        }
        else
        {
            std::cout << "  --> Solucao encontrada (" << solucao_atual.size()
                      << ") nao superou a melhor (" << melhor_solucao_encontrada.size() << "). Descartando." << std::endl;
        }
    }

    if (!melhor_solucao_encontrada.empty())
    {
        std::cout << "\nExecucao finalizada. Melhor tamanho de conjunto dominante encontrado: " << melhor_solucao_encontrada.size() << std::endl;
    }
    else
    {
        std::cout << "\nExecucao finalizada. Nenhuma solucao valida foi encontrada." << std::endl;
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

    std::cout << "Executando o algoritmo REATIVO " << n_execucoes << " vezes..." << std::endl;

    for (int i = 0; i < n_execucoes; ++i)
    {
        std::cout << "\n--- Execucao Reativa Principal " << (i + 1) << "/" << n_execucoes << " ---" << std::endl;

        std::vector<char> solucao_atual = conjunto_dominante_reativo(grafo, alfas, iteracoes_reativo, tamanho_bloco);

        if (melhor_solucao_encontrada.empty() || solucao_atual.size() < melhor_solucao_encontrada.size())
        {
            melhor_solucao_encontrada = solucao_atual;

            std::cout << "  --> Nova melhor solucao geral encontrada com tamanho " << melhor_solucao_encontrada.size() << std::endl;
        }
        else
        {
            std::cout << "  --> Solucao encontrada (" << solucao_atual.size() << ") nao superou a melhor (" << melhor_solucao_encontrada.size() << "). Descartando." << std::endl;
            solucao_atual.clear();
        }
    }

    std::cout << "\nExecucao finalizada. Melhor tamanho de conjunto dominante encontrado: " << melhor_solucao_encontrada.size() << std::endl;

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