#include "Gerenciador.h"
#include "AlgoritmosGulosos.h"

void Gerenciador::imprimir_vector_tela(vector<char> &resultado)
{
    for (int i = 0; i < resultado.size(); i++)
    {
        cout << resultado[i];
        if (i != resultado.size() - 1)
            cout << ", ";
    }
    cout << endl;
}

void Gerenciador::imprimir_vector_arquivo(vector<char> &resultado, string nome_arquivo, string descricao)
{
    ofstream arquivo(nome_arquivo);
    if (arquivo.is_open())
    {
        arquivo << descricao;
        for (int i = 0; i < resultado.size(); i++)
        {
            arquivo << resultado[i];
            if (i != resultado.size() - 1)
                arquivo << ", ";
        }
        arquivo << endl;
        arquivo.close();
    }
    else
    {
        cout << "Erro ao abrir o arquivo para escrita." << endl;
    }
}
void Gerenciador::comandos(Grafo *grafo)
{
    cout << "Digite uma das opcoes abaixo e pressione enter:" << endl
         << endl;
    cout << "(a) Fecho transitivo direto de um no;" << endl;
    cout << "(b) Fecho transitivo indireto de um no;" << endl;
    cout << "(c) Caminho minimo (Djikstra);" << endl;
    cout << "(d) Caminho minimo (Floyd);" << endl;
    cout << "(e) Arvore Geradora Minima (Algoritmo de Prim);" << endl;
    cout << "(f) Arvore Geradora Minima (Algoritmo de Kruskal);" << endl;
    cout << "(g) Arvore de caminhamento em profundidade;" << endl;
    cout << "(h) Raio, diametro, centro e periferia do grafo;" << endl;
    cout << "(j) Guloso" << endl;
    cout << "(k) Guloso randomizado" << endl;
    cout << "(l) Guloso reativo" << endl;
    cout << "(0) Sair;" << endl
         << endl;

    char resp;
    cin >> resp;
    switch (resp)
    {
    case 'a':
    {

        char id_no = get_id_entrada();
        vector<char> fecho_transitivo_direto = grafo->fecho_transitivo_direto(id_no);

        cout << "Fecho transitivo direto: ";
        imprimir_vector_tela(fecho_transitivo_direto);
        cout << endl;

        if (pergunta_imprimir_arquivo("fecho_trans_dir.txt"))
        {
            cout << "Metodo de impressao em arquivo nao implementado" << endl
                 << endl;
        }

        break;
    }

    case 'b':
    {

        char id_no = get_id_entrada();
        vector<char> fecho_transitivo_indireto = grafo->fecho_transitivo_indireto(id_no);
        cout << "Fecho transitivo indireto: ";
        imprimir_vector_tela(fecho_transitivo_indireto);
        cout << endl;

        if (pergunta_imprimir_arquivo("fecho_trans_indir.txt"))
        {
            cout << "Metodo de impressao em arquivo nao implementado" << endl;
        }

        ;
        break;
    }

    case 'c':
    {

        char id_no_1 = get_id_entrada();
        char id_no_2 = get_id_entrada();
        vector<char> caminho_minimo_dijkstra = grafo->caminho_minimo_dijkstra(id_no_1, id_no_2);

        bool impossivel = false;
        for (int i = 0; i < caminho_minimo_dijkstra.size(); i++)
        {
            if (caminho_minimo_dijkstra[i] == 0)
                impossivel = true;
        }

        if (impossivel)
        {
            cout << "Algo deu errado, caminho buscado pode nao ser possivel" << endl;
        }
        else
        {
            cout << "Caminho: ";
            imprimir_vector_tela(caminho_minimo_dijkstra);
            cout << endl;
        }

        if (pergunta_imprimir_arquivo("caminho_minimo_dijkstra.txt"))
        {
            imprimir_vector_arquivo(caminho_minimo_dijkstra, "output/caminho_minimo_dijkstra.txt", "Caminho: ");
        }

        break;
    }

    case 'd':
    {

        char id_no_1 = get_id_entrada();
        char id_no_2 = get_id_entrada();
        vector<char> caminho_minimo_floyd = grafo->caminho_minimo_floyd(id_no_1, id_no_2);

        cout << "Caminho: ";
        for (int i = 0; i < caminho_minimo_floyd.size(); i++)
        {
            cout << caminho_minimo_floyd[i];
            if (i != caminho_minimo_floyd.size() - 1)
                cout << ", ";
        }
        cout << endl;
        if (pergunta_imprimir_arquivo("caminho_minimo_floyd.txt"))
        {
            imprimir_vector_arquivo(caminho_minimo_floyd, "output/caminho_minimo_floyd.txt", "Caminho: ");
        }

        break;
    }
    case 'e':
    {

        int tam;
        cout << "Digite o tamanho do subconjunto: ";
        cin >> tam;

        if (tam > 0 && tam <= grafo->ordem)
        {

            vector<char> ids = get_conjunto_ids(grafo, tam);
            Grafo *arvore_geradora_minima_prim = grafo->arvore_geradora_minima_prim(ids);
            if (arvore_geradora_minima_prim == nullptr)
            {
                cout << "Grafo nao ponderado nas arestas, direcionado ou desconexo." << endl;
                break;
            }
            arvore_geradora_minima_prim->imprimir_lista_adjacencia();

            if (pergunta_imprimir_arquivo("agm_prim.txt"))
            {
                imprimir_grafo_arquivo(arvore_geradora_minima_prim, "agm_prim.txt");
            }

            delete arvore_geradora_minima_prim;
        }
        else
        {
            cout << "Valor invalido" << endl;
        }

        break;
    }

    case 'f':
    {

        int tam;
        cout << "Digite o tamanho do subconjunto: ";
        cin >> tam;

        if (tam > 0 && tam <= grafo->ordem)
        {

            vector<char> ids = get_conjunto_ids(grafo, tam);
            Grafo *arvore_geradora_minima_kruskal = grafo->arvore_geradora_minima_kruskal(ids);
            if (arvore_geradora_minima_kruskal == nullptr)
            {
                cout << "Grafo nao ponderado nas arestas, direcionado ou desconexo." << endl;

                break;
            }

            arvore_geradora_minima_kruskal->imprimir_lista_adjacencia();

            if (pergunta_imprimir_arquivo("agm_kruskal.txt"))
            {
                imprimir_grafo_arquivo(arvore_geradora_minima_kruskal, "agm_kruskal.txt");
            }

            delete arvore_geradora_minima_kruskal;
        }
        else
        {
            cout << "Valor invalido" << endl;
        }

        break;
    }

    case 'g':
    {

        char id_no = get_id_entrada();
        Grafo *arvore_caminhamento_profundidade = grafo->arvore_caminhamento_profundidade(id_no);
        arvore_caminhamento_profundidade->imprimir_lista_adjacencia();

        if (pergunta_imprimir_arquivo("arvore_caminhamento_profundidade.txt"))
        {
            cout << "Metodo de impressao em arquivo nao implementado" << endl;
        }

        delete arvore_caminhamento_profundidade;
        break;
    }

    case 'h':
    {
        int raio = grafo->raio();
        cout << "Raio: " << raio << endl;
        int diametro = grafo->diametro();
        cout << "Diametro: " << diametro << endl;
        cout << endl;

        vector<char> centro = grafo->centro();
        cout << "Centro: ";
        imprimir_vector_tela(centro);
        cout << endl;

        vector<char> periferia = grafo->periferia();
        cout << "Periferia: ";
        imprimir_vector_tela(periferia);
        cout << endl;

        if (pergunta_imprimir_arquivo("arvore_caminhamento_profundidade.txt"))
        {
            ofstream arquivo("output/raio_diametro_centro_periferia.txt");
            if (arquivo.is_open())
            {
                arquivo << "Raio: " << raio << endl;
                arquivo << "Diametro: " << diametro << endl;
                arquivo << "Centro: ";
                for (int i = 0; i < centro.size(); i++)
                {
                    arquivo << centro[i];
                    if (i != centro.size() - 1)
                        arquivo << ", ";
                }
                arquivo << endl;

                arquivo << "Periferia: ";
                for (int i = 0; i < periferia.size(); i++)
                {
                    arquivo << periferia[i];
                    if (i != periferia.size() - 1)
                        arquivo << ", ";
                }
                arquivo << endl;

                arquivo.close();
            }
            else
            {
                cout << "Erro ao abrir o arquivo para escrita." << endl;
            }
        }

        break;
    }
    case 'i':
    {

        vector<char> articulacao = grafo->vertices_de_articulacao();
        cout << "Metodo de impressao em tela nao implementado" << endl
             << endl;

        if (pergunta_imprimir_arquivo("arvore_caminhamento_profundidade.txt"))
        {
            cout << "Metodo de impressao em arquivo nao implementado" << endl;
        }

        break;
    }

    /**
     * @brief Algoritmo Guloso
     *
     * @details Este algoritmo executa o conjunto dominante guloso, que é uma abordagem gulosa para encontrar um conjunto dominante em um grafo.
     */
    case 'j':
    {
        vector<char> resultante = AlgoritmosGulosos::conjunto_dominante(grafo);
        cout << "Conjunto Dominante (Guloso): ";
        imprimir_vector_tela(resultante);
        cout << endl;

        break;
    }
    /**
     * @brief Algoritmo Guloso Randomizado
     *
     * @details Este algoritmo executa o conjunto dominante randomizado n vezes, onde n é o número de iterações fornecido pelo usuário.
     */
    case 'k':
    {
        float alfa;

        int interacoes;
        cout << "Digite o numero de iteracoes: " << endl;
        cin >> interacoes;

        cout << "Digite o valor de alfa (0 a 1): ";
        cin >> alfa;

        while (alfa < 0 || alfa > 1)
        {
            cout << "Valor invalido, digite novamente." << endl;
            cin >> alfa;
        }

        vector<char> resultante = AlgoritmosGulosos::executar_randomizado_n_vezes(grafo, interacoes, alfa);

        cout << "Conjunto Dominante (Guloso Randomizado): ";
        imprimir_vector_tela(resultante);
        cout << endl;

        break;
    }
    /**
     * @brief Algoritmo Guloso Reativo
     *
     * @details Este algoritmo executa o conjunto dominante reativo n vezes, onde n é o número de iterações fornecido pelo usuário.
     */
    case 'l':
    {
        vector<float> alfas = {0.1f, 0.3f, 0.5f, 0.7f, 0.9f};
        int iteracoes, iteracoes_reativo;
        cout << "Digite o numero de iteracoes: " << endl;
        cin >> iteracoes;
        cout << "Digite o numero de iteracoes reativo: " << endl;
        cin >> iteracoes_reativo;

        vector<char> resultante = AlgoritmosGulosos::executar_reativo_n_vezes(grafo, iteracoes, alfas, iteracoes_reativo, 5);

        cout << "Conjunto Dominante (Guloso Reativo): ";
        imprimir_vector_tela(resultante);
        cout << endl;

        break;
    }

    case '0':
    {
        exit(0);
    }
    default:
    {
        cout << "Opção inválida" << endl;
    }
    }

    comandos(grafo);
}

char Gerenciador::get_id_entrada()
{
    cout << "Digite o id de um no: ";
    char id;
    cin >> id;
    cout << endl;
    return id;
}

vector<char> Gerenciador::get_conjunto_ids(Grafo *grafo, int tam)
{
    vector<char> ids = {};
    while ((int)ids.size() < tam)
    {
        char id_no = get_id_entrada();
        bool existe = false;
        for (No *no : grafo->lista_adj)
        {
            if (no->id == id_no)
            {
                existe = true;
                break;
            }
        }

        if (!existe)
        {
            cout << "Vertice nao existe" << endl;
        }
        else
        {
            bool repetido = find(ids.begin(), ids.end(), id_no) != ids.end();
            if (repetido)
            {
                cout << "Valor repetido" << endl;
            }
            else
            {
                ids.push_back(id_no);
            }
        }
    }

    return ids;
}

bool Gerenciador::pergunta_imprimir_arquivo(string nome_arquivo)
{

    cout << "Imprimir em arquivo externo? (" << nome_arquivo << ")" << endl;
    cout << "(1) Sim;" << endl;
    cout << "(2) Nao." << endl;
    int resp;
    cin >> resp;
    cout << endl;

    switch (resp)
    {
    case 1:
        return true;
    case 2:
        return false;
    default:
        cout << "Resposta invalida" << endl;
        return pergunta_imprimir_arquivo(nome_arquivo);
    }
}

Grafo *Gerenciador::ler_arquivo(const string &nome_arquivo)
{
    // Abre o arquivo
    ifstream arquivo(nome_arquivo);

    // Verifica se o arquivo foi aberto corretamente
    if (!arquivo.is_open())
    {
        cerr << "Erro ao abrir o arquivo: " << nome_arquivo << endl;
    }

    // Le o cabeçalho do grafo
    bool in_direcionado, in_ponderado_aresta, in_ponderado_vertice;
    arquivo >> in_direcionado >> in_ponderado_aresta >> in_ponderado_vertice;

    int ordem;
    arquivo >> ordem;

    // Cria o grafo
    Grafo *grafo = new Grafo(ordem, in_direcionado, in_ponderado_aresta, in_ponderado_vertice);

    char id;
    int peso = 0;
    // Le os nos do grafo
    for (int i = 0; i < ordem; ++i)
    {
        arquivo >> id;
        if (in_ponderado_vertice)
        {
            arquivo >> peso;
        }

        // Adiciona o no ao grafo
        grafo->adicionar_vertice(id, peso);
    }

    // Le as arestas do grafo
    char id_no_origem, id_no_destino;
    int peso_aresta;

    while (arquivo >> id_no_origem >> id_no_destino)
    {
        if (in_ponderado_aresta)
        {
            arquivo >> peso_aresta;
        }
        else
        {
            peso_aresta = 0; // Peso padrão se não for ponderado
        }

        // Adiciona a aresta ao grafo
        grafo->adicionar_aresta_grafo(id_no_origem, id_no_destino, peso_aresta);
    }

    arquivo.close();

    return grafo;
}

void Gerenciador::imprimir_grafo_arquivo(Grafo *grafo, string nome_arquivo)
{
    ofstream arquivo("output/" + nome_arquivo);
    if (!arquivo.is_open())
    {
        cerr << "Erro ao abrir o arquivo para escrita: output/" << nome_arquivo << endl;
        return;
    }

    // Ordena os nós por ID para consistência (igual à função de impressão)
    vector<No *> nos_ordenados = grafo->lista_adj;
    sort(nos_ordenados.begin(), nos_ordenados.end(),
         [](No *a, No *b)
         { return a->id < b->id; });

    arquivo << "Grafo:" << endl;

    for (No *no : nos_ordenados)
    {
        if (no == nullptr)
            continue;

        // Imprime o nó de origem
        arquivo << no->id;
        if (grafo->in_ponderado_vertice)
        {
            arquivo << "(" << no->peso << ")";
        }
        arquivo << ": ";

        // Ordena arestas por ID de destino (igual à função de impressão)
        vector<Aresta *> arestas_ordenadas = no->arestas;
        sort(arestas_ordenadas.begin(), arestas_ordenadas.end(),
             [](Aresta *a, Aresta *b)
             { return a->id_no_alvo < b->id_no_alvo; });

        // Imprime as arestas
        bool first = true;
        for (Aresta *aresta : arestas_ordenadas)
        {
            if (aresta == nullptr)
                continue;

            if (!first)
            {
                arquivo << " -> ";
            }
            first = false;

            arquivo << aresta->id_no_alvo;
            if (grafo->in_ponderado_aresta)
            {
                arquivo << "(" << aresta->peso << ")";
            }
        }

        arquivo << endl;
    }

    arquivo.close();
    cout << "Grafo exportado para: output/" << nome_arquivo << endl;
}