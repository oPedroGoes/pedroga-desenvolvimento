#ifndef retangulo_h
#define retangulo_h
    
    typedef void* RETANGULO;

    /**
     * Uma estrutura de retângulo (RETANGULO) é uma abstração que representa um retângulo no plano bidimensional.
     * Cada retângulo possui um identificador único (ID), coordenadas (x, y) que definem sua âncora (ponto de referência),
     * uma largura (w), uma altura (h), uma cor da borda (cb) e uma cor de preenchimento (cp). Além disso, a estrutura
     * permite calcular a área do retângulo com base em sua largura e altura.
     *
     * A estrutura prove operações para criação, destruição e manipulação de retângulos, incluindo a obtenção
     * e modificação de seus atributos, como coordenadas, dimensões e cores.
     *
     * As operações incluem:
     * - Criação de um retângulo com todos os seus atributos.
     * - Obtenção do ID, coordenadas, dimensões, cores e área do retângulo.
     * - Modificação das coordenadas e cores do retângulo.
     * - Liberação da memória alocada para o retângulo.
     */

    /**
     * 
     * @brief Função para criar a estrutura de um retangulo
     * 
     * @param i ID do retangulo
     * @param x Eixo x da localização do retangulo
     * @param y Eixo y da localização do retangulo
     * @param w Comprimento do retangulo
     * @param h Altura do retangulo
     * @param cb cor da borda
     * @param cp cor do preenchimento 
     * 
     * @return Retorna um ponteiro do tipo void para essa estrutura
     */
    RETANGULO cria_retangulo(int i, float x, float y, float w, float h, char* cb, char* cp);

    /**
     * 
     * @brief Pegar por meio de um TAD o ID do retangulo
     * 
     * @param r Estrutura do retangulo para pegar a informação
     * 
     * @return retorna um inteiro
     */
    int get_idR(RETANGULO r);

    /**
     * 
     * @brief Pegar por meio de um TAD o eixo x do retangulo
     * 
     * @param r Estrutura do retangulo para pegar a informação
     * 
     * @return retorna um float
     */
    float get_XR(RETANGULO r);

    /**
     * 
     * @brief Pegar por meio de um TAD o eixo y do retangulo
     * 
     * @param r Estrutura do retangulo para pegar a informação
     * 
     * @return retorna um float
     */
    float get_YR(RETANGULO r);

    /**
     * 
     * @brief Pegar por meio de um TAD o comprimento do retangulo
     * 
     * @param r Estrutura do retangulo para pegar a informação
     * 
     * @return retorna um float
     */
    float get_wR(RETANGULO r);

    /**
     * 
     * @brief Pegar por meio de um TAD a altura do retangulo
     * 
     * @param r Estrutura do retangulo para pegar a informação
     * 
     * @return retorna um float
     */
    float get_hR(RETANGULO r);

    /**
     * 
     * @brief Pegar por meio de um TAD a cor da borda do retangulo
     * 
     * @param r Estrutura do retangulo para pegar a informação
     * 
     * @return retorna um float
     */
    char* get_cbR(RETANGULO r);

    /**
     * 
     * @brief Pegar por meio de um TAD a cor do preenchimento do retangulo
     * 
     * @param r Estrutura do retangulo para pegar a informação
     * 
     * @return retorna um float
     */
    char* get_cpR(RETANGULO r);

    void set_idR(RETANGULO r, int i);

    /**
     * 
     * @brief Pegar por meio de um TAD a area do retangulo
     * 
     * @param r Estrutura do retangulo para pegar a informação
     * 
     * @return retorna um float
     */
    float get_areaR(RETANGULO r);
    
    /**
     * 
     * @brief Muda o eixo x do retangulo
     * 
     * @param r Estrutura do retangulo
     * @param x Float contendo o novo eixo x
     * 
     * @return void
     */
    void set_xR(RETANGULO r, float x);

    /**
     * 
     * @brief Muda o eixo y do retangulo
     * 
     * @param r Estrutura do retangulo
     * @param y Float contendo o novo eixo y
     * 
     * @return void
     */
    void set_yR(RETANGULO r, float y);

    /**
     * 
     * @brief Muda o eixo a cor da borda do retangulo
     * 
     * @param r Estrutura do retangulo
     * @param cb String contendo a nova cor da borda
     * 
     * @return void
     */
    void set_cbR(RETANGULO r, char* cb);

    /**
     * 
     * @brief Muda o eixo a cor do preenchimento do retangulo
     * 
     * @param r Estrutura do retangulo
     * @param cp String contendo a nova cor do preenchimento
     * 
     * @return void
     */
    void set_cpR(RETANGULO r, char* cp);

    /**
     * 
     * @brief Desaloca a memoria de uma estrutura do retangulo
     * 
     * @param r Estrutura do retangulo
     * 
     * @return void
     */
    void kill_retangulo(RETANGULO r);
#endif