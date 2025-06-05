#ifndef PROCESSAGEO_H
#define PROCESSAGEO_H
    /*
    Esse cabeçalho header abre o arquivo .geo e insere em um árvore binária de busca todas as formas presentes no arquivo geo. Ele também retorna a estrutura da Arvore para ser utilizada em outras partes do codigo
    */
    typedef void* ARVORE;
    
    /**
     * 
     * @brief Função para ler o arquivo geo e processar o que está dentro dele, ela cria o arquivo svg 1 contendo as formas antes dos lançamentos
     * 
     * @param pathgeo String contendo o path para o arquivo geo para poder abrir ele
     * @param dirsaid String contendo o path para os arquivos de saida
     * @param nomegeo String contendo o nome do arquivo geo para estar presente no nome dos arquivos de saida
     * @param instru ponteiro para saber quantas instruções vao ser lidas no arquivo geo
     * @param formcriadas ponteiro para saber quantas serão as formas criadas no arquivo geo
     * @param id ponteiro para pegar o id da ultima forma criada
     * @param xcent ponteiro para pegar o valor x do baricentro do centro de gravidade
     * @param ycent ponteiro para pegar o valor y do baricentro do centro de gravidade
     * 
     * @return void
     */
    ARVORE processa_geo(char* pathgeo, char* dirsaida, char* nomegeo, int *prioMax, int *hc, double *promoRate, double epsilonConfig);
#endif