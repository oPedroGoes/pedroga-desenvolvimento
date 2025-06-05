#ifndef processaQry_h
#define processaQry_h

typedef void* ARVORE;
typedef void* LISTA;

ARVORE processa_qry(ARVORE t, const char *pathQry, const char *pathSaida, const char *nomeQry, LISTA *array_anotacoes, 
                        LISTA lista_anotacoes_svg /*int idUltimaForma?*/);

#endif // PROCESSA_QRY_H