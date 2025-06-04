#ifndef PROCESSA_QRY_H
#define PROCESSA_QRY_H

#include "SmuTreap.h" // Do diretório src do aluno
#include "Lista.h"    // Do diretório src do aluno

// Define os tipos de formas, idealmente de um header compartilhado
#ifndef TIPO_FORMAS_DEFINED
#define TIPO_FORMAS_DEFINED
#define TIPO_CIRCULO 1
#define TIPO_RETANGULO 2
#define TIPO_TEXTO 3
#define TIPO_LINHA 4
#endif

// Tipos opacos para marcadores e estilos SVG
typedef void* SVGPointMarker;
typedef void* SVGRectMarker;
typedef void* SVGSymbolMarker;
typedef void* SVGStyleOverride;

// --- Funções para SVGPointMarker ---
SVGPointMarker criar_svg_point_marker(double x, double y, const char* cor, double raio);
void destruir_svg_point_marker(SVGPointMarker marker);
double get_svg_point_marker_x(SVGPointMarker marker);
double get_svg_point_marker_y(SVGPointMarker marker);
const char* get_svg_point_marker_cor(SVGPointMarker marker);
double get_svg_point_marker_raio(SVGPointMarker marker);

// --- Funções para SVGRectMarker ---
SVGRectMarker criar_svg_rect_marker(double x, double y, double w, double h, const char* cor);
void destruir_svg_rect_marker(SVGRectMarker marker);
double get_svg_rect_marker_x(SVGRectMarker marker);
double get_svg_rect_marker_y(SVGRectMarker marker);
double get_svg_rect_marker_w(SVGRectMarker marker);
double get_svg_rect_marker_h(SVGRectMarker marker);
const char* get_svg_rect_marker_cor(SVGRectMarker marker);

// --- Funções para SVGSymbolMarker ---
SVGSymbolMarker criar_svg_symbol_marker(double x, double y, char symbol, const char* cor);
void destruir_svg_symbol_marker(SVGSymbolMarker marker);
double get_svg_symbol_marker_x(SVGSymbolMarker marker);
double get_svg_symbol_marker_y(SVGSymbolMarker marker);
char get_svg_symbol_marker_symbol(SVGSymbolMarker marker);
const char* get_svg_symbol_marker_cor(SVGSymbolMarker marker);

// --- Funções para SVGStyleOverride ---
SVGStyleOverride criar_svg_style_override(int id, const char* cb, const char* cp, int stroke_width, const char* tipo_forma_nome);
void destruir_svg_style_override(SVGStyleOverride style);
int get_svg_style_override_id(SVGStyleOverride style);
const char* get_svg_style_override_cb(SVGStyleOverride style);
const char* get_svg_style_override_cp(SVGStyleOverride style);
int get_svg_style_override_stroke_width(SVGStyleOverride style);
const char* get_svg_style_override_tipo_forma_nome(SVGStyleOverride style);


/**
 * @brief Processa um arquivo de consulta .qry, atualizando a SmuTreap e preparando dados para os arquivos de saída.
 *
 * @param pathQry Caminho completo para o arquivo .qry de entrada.
 * @param pathGeoBaseName Nome base do arquivo .geo original (ex: "a01").
 * @param pathQryBaseName Nome base do arquivo .qry (ex: "q").
 * @param dirSaida Diretório onde os arquivos de saída (.txt, .svg) devem ser colocados.
 * @param tree Ponteiro para a estrutura de dados SmuTreap a ser consultada e modificada.
 * @param epsilon Valor epsilon para comparações de ponto flutuante.
 * @param pSvgPointMarkers Lista para armazenar SVGPointMarker.
 * @param pSvgRectMarkers Lista para armazenar SVGRectMarker.
 * @param pSvgSymbolMarkers Lista para armazenar SVGSymbolMarker.
 * @param pSvgStyleOverrides Lista para armazenar SVGStyleOverride.
 * @param pMaxId Ponteiro para o ID máximo atual, a ser atualizado para novas formas (ex: clones).
 */
void processar_arquivo_qry(const char* pathQry,
                           const char* pathGeoBaseName,
                           const char* pathQryBaseName,
                           const char* dirSaida,
                           SmuTreap tree,
                           double epsilon,
                           Lista pSvgPointMarkers,
                           Lista pSvgRectMarkers,
                           Lista pSvgSymbolMarkers,
                           Lista pSvgStyleOverrides,
                           int* pMaxId);

/**
 * @brief Gera o arquivo SVG final incorporando todas as alterações e marcadores do processamento QRY.
 *
 * @param pathSvgOut Caminho completo para o arquivo .svg de saída.
 * @param tree A SmuTreap contendo o estado atual das formas.
 * @param svgPointMarkers Lista de SVGPointMarker.
 * @param svgRectMarkers Lista de SVGRectMarker.
 * @param svgSymbolMarkers Lista de SVGSymbolMarker.
 * @param svgStyleOverrides Lista de SVGStyleOverride.
 */
void gerar_svg_final_qry(const char* pathSvgOut,
                         SmuTreap tree,
                         Lista svgPointMarkers,
                         Lista svgRectMarkers,
                         Lista svgSymbolMarkers,
                         Lista svgStyleOverrides);

#endif // PROCESSA_QRY_H