#ifndef texto_h
#define texto_h
    
    typedef void* TEXTO;

    /**
     * Uma estrutura de texto (TEXTO) é uma abstração que representa um texto no plano bidimensional.
     * Cada texto possui um identificador único (ID), coordenadas (x, y) que definem sua localização,
     * uma cor da borda (cb), uma cor de preenchimento (cp), uma âncora (a) que indica o ponto de referência,
     * o conteúdo do texto (txt), a família da fonte (ff), o peso da fonte (fw) e o tamanho da fonte (fs).
     * Além disso, a estrutura permite calcular a área do texto com base em seu comprimento e altura.
     *
     * A estrutura prove operações para criação, destruição e manipulação de textos, incluindo a obtenção
     * e modificação de seus atributos, como coordenadas, cores, conteúdo e propriedades da fonte.
     *
     * As operações incluem:
     * - Criação de um texto com todos os seus atributos.
     * - Obtenção do ID, coordenadas, cores, âncora, conteúdo, família da fonte, peso da fonte e tamanho da fonte.
     * - Modificação das coordenadas, cores e conteúdo do texto.
     * - Liberação da memória alocada para o texto.
     */

    /**
     * 
     * @brief Função para criar a estrutura de um texto
     * 
     * @param i ID do texto
     * @param x Eixo x da localização do texto
     * @param y Eixo y da localização do texto
     * @param cb cor da borda
     * @param cp cor do preenchimento 
     * @param a Char para indicar onde fica a âncora do texto
     * @param txt O texto em si da estrutura
     * @param ff Familia do texto
     * @param fw Weight do texto
     * @param fs Tamanho do texto
     * 
     * @return Retorna um ponteiro do tipo void para essa estrutura
     */
    TEXTO cria_texto(int i, float x, float y, char* cb, char* cp, char a, char* txt, char* ff, char* fw, char* fs);

    /**
     * 
     * @brief Pegar por meio de um TAD o ID do texto
     * 
     * @param t Estrutura do texto para pegar a informação
     * 
     * @return retorna um inteiro
     */
    int get_idT(TEXTO t);

    /**
     * 
     * @brief Pegar por meio de um TAD o eixo x do texto
     * 
     * @param t Estrutura do texto para pegar a informação
     * 
     * @return retorna um float
     */
    float get_XT(TEXTO t);

    /**
     * 
     * @brief Pegar por meio de um TAD o eixo y do texto
     * 
     * @param t Estrutura do texto para pegar a informação
     * 
     * @return retorna um float
     */
    float get_YT(TEXTO t);
    /**
     * 
     * @brief Pegar por meio de um TAD a cor da borda do texto
     * 
     * @param t Estrutura do texto para pegar a informação
     * 
     * @return retorna uma string
     */
    char* get_cbT(TEXTO t);

    /**
     * 
     * @brief Pegar por meio de um TAD a cor do preenchimento do texto
     * 
     * @param t Estrutura do texto para pegar a informação
     * 
     * @return retorna uma string
     */
    char* get_cpT(TEXTO t);
    
    /**
     * 
     * @brief Pegar por meio de um TAD a ancora do texto
     * 
     * @param t Estrutura do texto para pegar a informação
     * 
     * @return retora uma string para o svg dizendo onde esta a ancora do texto
     */
    char* get_ancoraT(TEXTO t);

    /**
     * 
     * @brief Pegar por meio de um TAD o texto em si da estrutura texto
     * 
     * @param t Estrutura do texto para pegar a informação
     * 
     * @return retorna uma string
     */
    char* get_txtT(TEXTO t);

    /**
     * 
     * @brief Pegar por meio de um TAD a familia do texto
     * 
     * @param t Estrutura do texto para pegar a informação
     * 
     * @return retorna uma string
     */
    char* get_ffT(TEXTO t);

    /**
     * 
     * @brief Pegar por meio de um TAD o tamanho do texto
     * 
     * @param t Estrutura do texto para pegar a informação
     * 
     * @return retorna uma string
     */
    char* get_fsT(TEXTO t);

    /**
     * 
     * @brief Pegar por meio de um TAD o Weight do texto
     * 
     * @param t Estrutura do texto para pegar a informação
     * 
     * @return retorna uma string
     */
    char* get_fwT(TEXTO t);

    /**
     * 
     * @brief Pegar por meio de um TAD a quantidade de caracteres presentes no texto do texto
     * 
     * @param t Estrutura do texto para pegar a informação
     * 
     * @return retorna um inteiro
     */
    int tamanho_dotextoT(TEXTO t);

    /**
     * 
     * @brief Pegar por meio de um TAD o comprimento do texto usado para calcular a área
     * 
     * @param t Estrutura do texto para pegar a informação
     * 
     * @return retorna um inteiro
     */
    int comprimento_da_linhaT(TEXTO t);
    
    /**
     * 
     * @brief Pegar por meio de um TAD o inicio do texto
     * 
     * @param t Estrutura do texto para pegar a informação
     * 
     * @return retorna um float
     */
    float get_eixoX1T(TEXTO t);

    void set_idT(TEXTO t, int i);
    
    /**
     * 
     * @brief Pegar por meio de um TAD o fim do texto
     * 
     * @param t Estrutura do texto para pegar a informação
     * 
     * @return retorna um float
     */
    float get_eixoX2T(TEXTO t);

    /**
     * 
     * @brief Muda o eixo x do texto
     * 
     * @param t estrutura do texto que vai ter o eixo x mudado
     * @param x novo eixo x
     * 
     * @return void
     */
    void set_xT(TEXTO t, float x);

    /**
     * 
     * @brief Muda o eixo y do texto
     * 
     * @param t estrutura do texto que vai ter o eixo y mudado
     * @param y novo eixo y
     * 
     * @return void
     */
    void set_yT(TEXTO t, float y);

    /**
     * 
     * @brief Muda a cor da borda do texto
     * 
     * @param t estrutura do texto que vai ter a cor da borda mudada
     * @param cb nova cor da borda
     * 
     * @return void
     */
    void set_cbT(TEXTO t, char* cb);

    /**
     * 
     * @brief Muda a cor do preenchimento do texto
     * 
     * @param t estrutura do texto que vai ter a cor do preenchimento mudado
     * @param cb nova cor do preenchimento
     * 
     * @return void
     */
    void set_cpT(TEXTO t, char* cp);

    /**
     * 
     * @brief Pegar por meio de um TAD a area do texto
     * 
     * @param t Estrutura do texto para pegar a informação
     * 
     * @return retorna um float
     */
    float get_areaT(TEXTO t);

    /**
     * 
     * @brief Pegar por meio de um TAD o charactere indicando a ancora do texto
     * 
     * @param t Estrutura do texto para pegar a informação
     * 
     * @return retorna um char
     */
    char get_charancoraT(TEXTO t);

    /**
     * 
     * @brief desaloca a memória do texto
     * 
     * @param t Estrutura do texto para ser desalocado
     * 
     * @return void
     */
    void kill_texto(TEXTO t);
#endif