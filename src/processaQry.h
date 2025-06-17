#ifndef PROCESSAQRY_H
#define PROCESSAQRY_H

#include "SmuTreap.h"
#include "Lista.h"

/**
 * @file processaQry.h
 * @brief Módulo responsável por processar o arquivo .qry, executar comandos de consulta e modificação,
 *        e gerar as saídas TXT e SVG correspondentes.
 *
 * Este TAD centraliza a leitura e execução dos comandos presentes no arquivo .qry, aplicando-os sobre a
 * estrutura de dados SmuTreap que armazena as formas geométricas. Ele gerencia operações como seleção,
 * clonagem, translação, alteração de propriedades, disparos e explosões, além de gerar relatórios e
 * anotações visuais no SVG de saída.
 *
 * O módulo também integra listas de seleções, anotações SVG e mantém o controle do maior ID de forma.
 */

/**
 * @brief Processa o arquivo .qry, executando todos os comandos e gerando as saídas.
 *
 * Esta função é o ponto de entrada principal para o processamento do arquivo .qry. Ela:
 *   - Abre o arquivo .qry para leitura.
 *   - Cria os arquivos de saída SVG e TXT, nomeados a partir dos parâmetros fornecidos.
 *   - Inicializa o contexto de execução, incluindo ponteiros para a árvore de formas, listas de seleções,
 *     lista de anotações SVG e controle de IDs.
 *   - Lê e executa cada comando do .qry, modificando a árvore de formas conforme necessário.
 *   - Gera o SVG final com o estado atualizado das formas e as anotações visuais.
 *   - Gera o relatório TXT detalhando as operações realizadas.
 *   - Libera todos os recursos alocados ao final do processamento.
 *
 * @param t SmuTreap contendo as formas geométricas (pode ser modificada pelos comandos).
 * @param pathQry Caminho para o arquivo .qry de entrada.
 * @param pathSaida Diretório onde os arquivos de saída serão salvos.
 * @param nomeBaseGeo Nome base do arquivo .geo (usado para nomear as saídas).
 * @param nomeBaseQry Nome base do arquivo .qry (usado para nomear as saídas).
 * @param array_selecoes Array de listas de seleções (tamanho 100), compartilhado com a main.
 * @param lista_anotacoes_svg Lista de anotações SVG a serem inseridas no SVG de saída.
 * @param idMax Ponteiro para inteiro que controla o maior ID de forma já utilizado.
 * @param epsilon Valor de tolerância para comparações de ponto/flutuante.
 * @return SmuTreap modificada após a execução dos comandos do .qry.
 *
 * @note Em caso de erro na abertura de arquivos ou alocação de memória, a função encerra o programa.
 */
SmuTreap processa_qry(SmuTreap t, const char *pathQry, const char *pathSaida, const char *nomeBaseGeo, const char *nomeBaseQry,
                      Lista *array_selecoes, Lista lista_anotacoes_svg, int *idMax, double epsilon);

#endif