#ifndef PROCESSAGEO_H
#define PROCESSAGEO_H

    /*
    Esse cabeçalho header abre o arquivo .geo e insere em um árvore binária de busca todas as formas presentes no arquivo geo. Ele também retorna a estrutura da Arvore para ser utilizada em outras partes do codigo
    */
    #include "SmuTreap.h"
    
    /**
     * @file processaGeo.h
     * @brief Módulo responsável por processar o arquivo .geo, criar e popular a árvore de formas geométricas,
     *        e gerar o SVG inicial do projeto.
     *
     * Este TAD centraliza a leitura do arquivo de entrada .geo, a criação das formas geométricas (círculo, retângulo,
     * linha, texto), a inserção dessas formas em uma SmuTreap (árvore binária de busca balanceada), e a geração do
     * arquivo SVG inicial que representa o estado do sistema antes de qualquer operação de consulta.
     *
     * O módulo também gerencia parâmetros de configuração como prioridade máxima, hit count, taxa de promoção e epsilon,
     * além de atualizar o maior ID de forma lido.
     */
    
    /**
     * @brief Lê o arquivo .geo e insere todas as formas encontradas na SmuTreap fornecida.
     *
     * Esta função percorre cada linha do arquivo .geo, identifica o tipo de forma (círculo, retângulo, linha, texto),
     * cria a estrutura correspondente, e insere na árvore SmuTreap usando a função de cálculo de bounding box apropriada.
     * Também atualiza o maior ID de forma encontrado.
     *
     * @param arqGeo Ponteiro para o arquivo .geo já aberto para leitura.
     * @param t SmuTreap já inicializada onde as formas serão inseridas.
     * @param funcCalcBb Função para cálculo do bounding box de cada forma.
     * @param idMax Ponteiro para inteiro que será atualizado com o maior ID de forma lido.
     * @return SmuTreap populada com as formas do arquivo .geo.
     */
    SmuTreap leitura_geo(FILE *arqGeo, SmuTreap t, FCalculaBoundingBox funcCalcBb, int *idMax);
    
    /**
     * @brief Processa o arquivo .geo, gera o SVG inicial e retorna a árvore de formas.
     *
     * Esta função é o ponto de entrada principal para o processamento do arquivo .geo. Ela:
     *   - Abre o arquivo .geo para leitura.
     *   - Cria o arquivo SVG inicial no diretório de saída, com o nome base do .geo.
     *   - Inicializa a SmuTreap com os parâmetros fornecidos (ou valores padrão).
     *   - Lê e insere todas as formas do .geo na árvore, atualizando o maior ID.
     *   - Percorre a árvore e escreve todas as formas no SVG inicial.
     *   - Fecha os arquivos e retorna a árvore SmuTreap populada.
     *
     * Parâmetros de configuração (prioMax, hc, promoRate) podem ser passados como ponteiros (se NULL, valores padrão são usados).
     *
     * @param pathgeo Caminho para o arquivo .geo de entrada.
     * @param dirsaida Diretório onde o SVG inicial será salvo.
     * @param nomeBaseGeo Nome base do arquivo .geo (usado para nomear o SVG).
     * @param idMax Ponteiro para inteiro que será atualizado com o maior ID de forma lido.
     * @param prioMax Ponteiro para prioridade máxima da SmuTreap (ou NULL para padrão).
     * @param hc Ponteiro para hit count da SmuTreap (ou NULL para padrão).
     * @param promoRate Ponteiro para taxa de promoção da SmuTreap (ou NULL para padrão).
     * @param epsilonConfig Valor de epsilon para comparações de ponto/flutuante.
     * @return SmuTreap populada com as formas do arquivo .geo, pronta para operações e consultas.
     *
     * @note Em caso de erro na abertura de arquivos ou alocação de memória, a função encerra o programa.
     */
    SmuTreap processa_geo(const char* pathgeo, const char* dirsaida, const char* nomeBaseGeo,
                        int *idMax, int *prioMax, int *hc, double *promoRate, double epsilonConfig);
    
#endif // PROCESSAGEO_H