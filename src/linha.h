#ifndef linha_h
#define linha_h
    
    typedef void* LINHA;

    /**
     * 
     * @brief Função para criar a estrutura de uma linha
     * 
     * @param i ID da linha
     * @param x1 Inicio do x da linha
     * @param y1 Inicio do y da linha
     * @param x2 fim do x da linha
     * @param y2 fim do y da linha
     * @param c cor da linha 
     * 
     * @return Retorna um ponteiro do tipo void para essa estrutura
     */
    LINHA cria_linha(int i, float x1, float y1, float x2, float y2, char* c);

    /**
     * 
     * @brief Pegar por meio de um TAD o ID da linha
     * 
     * @param l Estrutura da linha para pegar a informação
     * 
     * @return retorna um inteiro
     */
    int get_idL(LINHA l);

    void set_idL(LINHA l, int i);

    /**
     * 
     * @brief Pegar por meio de um TAD o inicio do eixo x da linha
     * 
     * @param l Estrutura da linha para pegar a informação
     * 
     * @return retorna um float
     */
    float get_X1L(LINHA l);

    /**
     * 
     * @brief Pegar por meio de um TAD o inicio do eixo y da linha
     * 
     * @param l Estrutura da linha para pegar a informação
     * 
     * @return retorna um float
     */
    float get_Y1L(LINHA l);

    /**
     * 
     * @brief Pegar por meio de um TAD o fim do eixo x da linha
     * 
     * @param l Estrutura da linha para pegar a informação
     * 
     * @return retorna um float
     */
    float get_X2L(LINHA l);

    /**
     * 
     * @brief Pegar por meio de um TAD o fim do eixo y da linha
     * 
     * @param l Estrutura da linha para pegar a informação
     * 
     * @return retorna um float
     */
    float get_Y2L(LINHA l);

    /**
     * 
     * @brief Pegar por meio de um TAD a cor da linha
     * 
     * @param l Estrutura da linha para pegar a informação
     * 
     * @return retorna uma string contendo a cor
     */
    char* get_cL(LINHA l);

    /**
     * 
     * @brief Pegar por meio de um TAD a area da linha
     * 
     * @param l Estrutura da linha para pegar a informação
     * 
     * @return retorna um float
     */
    float get_areaL(LINHA l);

    /** 
     *
     * @brief Muda o x inicial da linha 
     * 
     * @param l Estrutura da linha para mudar a informação
     * @param x Float x contendo a nova localização
     * 
     * @return void
     */
    void set_x1L(LINHA l, float x);

    /** 
     *
     * @brief Muda o y inicial da linha 
     * 
     * @param l Estrutura da linha para mudar a informação
     * @param y Float y contendo a nova localização
     * 
     * @return void
     */
    void set_y1L(LINHA l, float y);

    /** 
     *
     * @brief Muda o x final da linha 
     * 
     * @param l Estrutura da linha para mudar a informação
     * @param x Float x contendo a nova localização
     * 
     * @return void
     */
    void set_x2L(LINHA l, float x);

    /** 
     *
     * @brief Muda o y final da linha 
     * 
     * @param l Estrutura da linha para mudar a informação
     * @param y Float y contendo a nova localização
     * 
     * @return void
     */
    void set_y2L(LINHA l, float y);

    /** 
     *
     * @brief Muda a cor da linha 
     * 
     * @param l Estrutura da linha para mudar a informação
     * @param c String contendo a nova cor
     * 
     * @return void
     */
    void set_cL(LINHA l, char* c);

    /** 
     *
     * @brief Função para desalocar a memória da estrutura da linha
     * 
     * @param l Estrutura da linha para ter a memória desalocada
     * 
     * @return void
     */
    void kill_linha(LINHA l);
#endif