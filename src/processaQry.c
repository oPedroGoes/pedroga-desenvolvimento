#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void leitura_qry(FILE *arqQry){
    char *tipo = (char*)malloc(sizeof(char)*5);
    char *str = (char*)malloc(sizeof(char)*100);

    while(fgets(str, 100, arqQry)){
        strncpy(tipo, str, 5);
        tipo[4] = '\0';

        if(strcmp(tipo, "selr") == 0){
            printf("processando selr...");
            //processa selr
        }

        if(strcmp(tipo, "seli") == 0){
            printf("processando seli...");
            //processa seli
        }

        if(strcmp(tipo, "disp") == 0){
            printf("processando disp...");
            //processa disp
        }

        if(strcmp(tipo, "transp") == 0){
            printf("processando transp...");
            //processa transp
        }

        if(strcmp(tipo, "cln") == 0){
            printf("processando cln...");
            //processa cln
        }

        if(strcmp(tipo, "spy") == 0){
            printf("processando spy...");
            //processa spy
        }

        if(strcmp(tipo, "cmflg") == 0){
            printf("processando cmflg...");
            //processa cmflg
        }

        if(strcmp(tipo, "blow") == 0){
            printf("processando blow...");
            //processa blow
        }

    }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "smutreap.h" // Supondo que este é o header da sua SmuTreap
// #include "lista.h" // Se você usar a estrutura Lista do smutreap.h para seleções

#define MAX_LINE_LENGTH 1024
#define MAX_COLOR_LENGTH 50

// --- Protótipos das Funções de Tratamento para cada Comando ---
// Estas funções precisariam de mais parâmetros (SmuTreap, arquivos de saída, etc.)
void handle_selr(SmuTreap tree, FILE* pathTxtOut, int n, double x, double y, double w, double h) {
    fprintf(txt_output_file, "[*] selr %d %.2f %.2f %.2f %.2f\n", n, x, y, w, h);
    printf("Comando selr: n=%d, x=%.2f, y=%.2f, w=%.2f, h=%.2f\n", n, x, y, w, h);
    // TODO: Implementar lógica de selr
    // - Selecionar formas na SmuTreap
    // - Reportar no TXT
    // - Marcar no SVG
    fprintf(txt_output_file, "Resultado de selr...\n"); // Placeholder
}

void handle_seli(SmuTreap tree, FILE* pathTxtOut, int n, double x, double y) {
    fprintf(pathTxtOut, "[*] seli %d %.2f %.2f\n", n, x, y);
    printf("Comando seli: n=%d, x=%.2f, y=%.2f\n", n, x, y);
    // TODO: Implementar lógica de seli
    fprintf(pathTxtOut, "Resultado de seli...\n"); // Placeholder
}

void handle_disp(SmuTreap tree, FILE* pathTxtOut, int id, int n) {
    fprintf(pathTxtOut, "[*] disp %d %d\n", id, n);
    printf("Comando disp: id=%d, n=%d\n", id, n);
    // TODO: Implementar lógica de disp
    fprintf(pathTxtOut, "Resultado de disp...\n"); // Placeholder
}

void handle_transp(SmuTreap tree, FILE* pathTxtOut, int id, double x, double y) {
    fprintf(pathTxtOut, "[*] transp %d %.2f %.2f\n", id, x, y);
    printf("Comando transp: id=%d, x=%.2f, y=%.2f\n", id, x, y);
    // TODO: Implementar lógica de transp
    fprintf(pathTxtOut, "Resultado de transp...\n"); // Placeholder
}

void handle_cln(SmuTreap tree, FILE* pathTxtOut, int n, double dx, double dy) {
    fprintf(pathTxtOut, "[*] cln %d %.2f %.2f\n", n, dx, dy);
    printf("Comando cln: n=%d, dx=%.2f, dy=%.2f\n", n, dx, dy);
    // TODO: Implementar lógica de cln
    fprintf(pathTxtOut, "Resultado de cln...\n"); // Placeholder
}

void handle_spy(SmuTreap tree, FILE* pathTxtOut, int id) {
    fprintf(pathTxtOut, "[*] spy %d\n", id);
    printf("Comando spy: id=%d\n", id);
    // TODO: Implementar lógica de spy
    fprintf(pathTxtOut, "Resultado de spy...\n"); // Placeholder
}

void handle_cmflg(SmuTreap tree, FILE* pathTxtOut, int id, const char* cb, const char* cp, int w_px) {
    fprintf(pathTxtOut, "[*] cmflg %d %s %s %d\n", id, cb, cp, w_px);
    printf("Comando cmflg: id=%d, cb=%s, cp=%s, w=%dpx\n", id, cb, cp, w_px);
    // TODO: Implementar lógica de cmflg
    fprintf(pathTxtOut, "Resultado de cmflg...\n"); // Placeholder
}

void handle_blow(SmuTreap tree, FILE* pathTxtOut, int id) {
    fprintf(pathTxtOut, "[*] blow %d\n", id);
    printf("Comando blow: id=%d\n", id);
    // TODO: Implementar lógica de blow
    fprintf(pathTxtOut, "Resultado de blow...\n"); // Placeholder
}


// Função principal para processar o arquivo .qry
void processar_arquivo_qry(const char* pathQry, const char* pathTxtOut, SmuTreap tree) {
    FILE *qry_file = fopen(pathQry, "r");
    if (qry_file == NULL) {
        perror("Erro ao abrir o arquivo .qry");
        return;
    }

    FILE *txt_out = fopen(pathTxtOut, "w"); // Abrir arquivo de saída .txt
    if (txt_out == NULL) {
        perror("Erro ao criar o arquivo .txt de saida");
        fclose(qry_file);
        return;
    }

    char *linha = (char*)malloc(sizeof(char)*MAX_LINE_LENGTH);
    char *comm = (char*)malloc(sizeof(char)*7);
    while (fgets(linha, sizeof(linha), qry_file)) {
        // Remover nova linha do final, se houver
        linha[strcspn(linha, "\n")] = 0;
        linha[strcspn(linha, "\r")] = 0; // Para compatibilidade Windows/Linux

        strncpy(comm, linha, 7);
        comm[6] = '\0';


        // Processar com base no comando
        if (strcmp(comm, "selr  ") == 0) {
            int n;
            double x, y, w, h;
            // sscanf é uma boa forma de parsear o restante da linha_original
            if (sscanf(linha, "selr %d %lf %lf %lf %lf", &n, &x, &y, &w, &h) == 5) {
                handle_selr(tree, txt_out, n, x, y, w, h);
            } else {
                fprintf(txt_out, "[*] %s\nErro: parametros invalidos para selr\n", linha);
            }
        } else if (strcmp(comm, "seli  ") == 0) {
            int n;
            double x, y;
            if (sscanf(linha, "seli %d %lf %lf", &n, &x, &y) == 3) {
                handle_seli(tree, txt_out, n, x, y);
            } else {
                fprintf(txt_out, "[*] %s\nErro: parametros invalidos para seli\n", linha);
            }
        } else if (strcmp(comm, "disp  ") == 0) {
            int id, n_sel;
            if (sscanf(linha, "disp %d %d", &id, &n_sel) == 2) {
                handle_disp(tree, txt_out, id, n_sel);
            } else {
                fprintf(txt_out, "[*] %s\nErro: parametros invalidos para disp\n", linha);
            }
        } else if (strcmp(comm, "transp") == 0) {
            int id;
            double x, y;
            if (sscanf(linha, "transp %d %lf %lf", &id, &x, &y) == 3) {
                handle_transp(tree, txt_out, id, x, y);
            } else {
                fprintf(txt_out, "[*] %s\nErro: parametros invalidos para transp\n", linha);
            }
        } else if (strcmp(comm, "cln   ") == 0) {
            int n;
            double dx, dy;
            if (sscanf(linha, "cln %d %lf %lf", &n, &dx, &dy) == 3) {
                handle_cln(tree, txt_out, n, dx, dy);
            } else {
                fprintf(txt_out, "[*] %s\nErro: parametros invalidos para cln\n", linha);
            }
        } else if (strcmp(comm, "spy   ") == 0) {
            int id;
            if (sscanf(linha, "spy %d", &id) == 1) {
                handle_spy(tree, txt_out, id);
            } else {
                fprintf(txt_out, "[*] %s\nErro: parametros invalidos para spy\n", linha);
            }
        } else if (strcmp(comm, "cmflg ") == 0) {
            int id, w_px;
            char cb[MAX_COLOR_LENGTH], cp[MAX_COLOR_LENGTH];
            // Formato: cmflg id corBorda corPreenchimento larguraPx
            if (sscanf(linha, "cmflg %d %s %s %d", &id, cb, cp, &w_px) == 4) {
                handle_cmflg(tree, txt_out, id, cb, cp, w_px);
            } else {
                fprintf(txt_out, "[*] %s\nErro: parametros invalidos para cmflg\n", linha);
            }
        } else if (strcmp(comm, "blow  ") == 0) {
            int id;
            if (sscanf(linha, "blow %d", &id) == 1) {
                handle_blow(tree, txt_out, id);
            } else {
                fprintf(txt_out, "[*] %s\nErro: parametros invalidos para blow\n", linha);
            }
        } else {
            // Comando desconhecido
            fprintf(txt_out, "[*] %s\nComando desconhecido: %s\n", linha, comm);
            printf("Comando desconhecido: %s\n", comm);
        }
    }
    free(linha);
    free(comm);
    fclose(qry_file);
    fclose(txt_out);
    printf("Processamento do arquivo QRY concluido. Saida em: %s\n", pathTxtOut);
}

// Exemplo de como você poderia chamar isso no seu main.c
// int main(int argc, char *argv[]) {
//     // ... parsear argumentos de linha de comando para obter:
//     // qry_file_path, geo_file_path, output_dir, etc.
//     // SmuTreap minhaArvore = newSmuTreap(...);
//     // popularSmuTreapComGeo(minhaArvore, geo_file_path, ...);

//     // const char* qry_path = "caminho/para/seu/arquivo.qry"; // Vindo de -q
//     // const char* txt_out_path = "caminho/para/seu/saida.txt"; // Construído a partir de -o e nomes de arquivos

//     // if (existe_arquivo_qry) {
//     //     processar_arquivo_qry(qry_path, txt_out_path, minhaArvore);
//     // }
    
//     // ... gerar SVG final, .dot, etc.
//     // killSmuTreap(minhaArvore);
//     return 0;
// }