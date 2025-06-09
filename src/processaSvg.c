#include <stdio.h>
#include <stdlib.h>


#include "processaSvg.h"
#include "formas.h"

void escreverFormaSvg(SmuTreap t, Node n, Info i, double x_ancora_no, double y_ancora_no, void *aux_file_ptr) {
    if (!i || !aux_file_ptr) {
        return;
    }

    FILE *arq_svg = (FILE *)aux_file_ptr;
    DescritorTipoInfo tipo_forma = getTypeInfoSmuT(t, n);

    switch (tipo_forma) {
        case TIPO_CIRCULO: {
            CIRCLE c = (CIRCLE)i;
            int id = get_idC(c);
            double xc = get_XC(c);
            double yc = get_YC(c);
            double r_circ = get_rC(c);
            char *corb = get_cbC(c);
            char *corp = get_cpC(c);

            fprintf(arq_svg, "\t<circle id=\"%d\" cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"%s\" fill=\"%s\" fill-opacity=\"0.5\" />\n", id, xc, yc, r_circ, corb, corp);
            break;
        }
        case TIPO_RETANGULO: {
            RECTANGLE r_fig = (RECTANGLE)i;
            int id = get_idR(r_fig); 
            double xr = get_XR(r_fig); 
            double yr = get_YR(r_fig); 
            double w = get_wR(r_fig); 
            double h = get_hR(r_fig); 
            char *corb = get_cbR(r_fig); 
            char *corp = get_cpR(r_fig); 

            fprintf(arq_svg, "\t<rect id=\"%d\" x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" stroke=\"%s\" fill=\"%s\" fill-opacity=\"0.5\" />\n", id, xr, yr, w, h, corb, corp);
            // Posicionar ID no centro da âncora original do projeto (canto inferior esquerdo) pode ser uma opção, ou no centro visual.
            // Para centro visual:
            break;
        }
        case TIPO_LINHA: {
            LINHA l = (LINHA)i;
            int id = get_idL(l);
            double x1 = get_X1L(l);
            double y1 = get_Y1L(l);
            double x2 = get_X2L(l);
            double y2 = get_Y2L(l);
            char *cor = get_cL(l);

            fprintf(arq_svg, "\t<line id=\"%d\" x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" stroke=\"%s\" stroke-width=\"1\" />\n",id, x1, y1, x2, y2, cor);
            // O ID da linha será desenhado na âncora da árvore para esta linha (x_ancora_no, y_ancora_no)
            break;
        }
        case TIPO_TEXTO: {
            TEXTO txt_fig = (TEXTO)i;
            int id = get_idT(txt_fig);
            double xt = get_XT(txt_fig);
            double yt = get_YT(txt_fig);
            char *corb = get_cbT(txt_fig);
            char *corp = get_cpT(txt_fig);
            char *ancora_svg = get_ancoraT(txt_fig); // Retorna "start", "middle", ou "end"
            char *conteudo_txt = get_txtT(txt_fig);
            char *ff = get_ffT(txt_fig);
            char *fw = get_fwT(txt_fig);
            char *fs = get_fsT(txt_fig);

            // A cor da borda do texto em SVG é controlada por 'stroke', não 'fill' da borda.
            // O preenchimento do texto é 'fill'.
            fprintf(arq_svg, "\t<text id=\"%d\" x=\"%.2f\" y=\"%.2f\" stroke=\"%s\" fill=\"%s\" font-family=\"%s\" font-weight=\"%s\" font-size=\"%s\" text-anchor=\"%s\" dominant-baseline=\"middle\">",
                    id, xt, yt, corb, corp, ff, fw, fs, ancora_svg);

            // Itera pelo conteudo_txt e escapa os caracteres especiais
            if (conteudo_txt) {
                char *p = conteudo_txt;
                while (*p) {
                    switch (*p) {
                        case '<':
                            fprintf(arq_svg, "&lt;");
                            break;
                        case '>':
                            fprintf(arq_svg, "&gt;");
                            break;
                        case '&':
                            fprintf(arq_svg, "&amp;");
                            break;
                        case '"': // Embora não estritamente necessário para o conteúdo da tag, é seguro escapar.
                            fprintf(arq_svg, "&quot;");
                            break;
                        case '\'': // Similarmente, seguro escapar.
                            fprintf(arq_svg, "&apos;");
                            break;
                        default:
                            fputc(*p, arq_svg);
                            break;
                    }
                    p++;
                }
            }
            // Imprime a tag de fechamento do texto
            fprintf(arq_svg, "</text>\n");
            break;
        }
        default:
            fprintf(stderr, "Aviso (escreverFormaSvg): Tipo de forma desconhecido (%d) encontrado no no com ancora (%.2f, %.2f).\n", tipo_forma, x_ancora_no, y_ancora_no);
            break;
    }
}

void visitaListaSvg(Item anotacao, void *aux){
    if(!anotacao || !aux){
        fprintf(stderr, "(visitaListaSvg) Erro: parametros invalidos");
        return;
    }

    FILE *arqSvg2 = (FILE*)aux;

    const char *buff = (const char*)anotacao;
    fprintf(arqSvg2, "%s\n", buff);
    return;
}
