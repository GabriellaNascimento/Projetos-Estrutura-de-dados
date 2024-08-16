#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct _no {
    char* nome;
    int paginas;
    struct _no *prox;
    struct _no *ant;
} No;

typedef struct _noFilaHist {
    char* nome_i;
    char* nome_d;
    int paginas;
    char* historico;
    struct _noFilaHist *prox;
    struct _noFilaHist *ant;
} NoFilaHist;

typedef struct _lista {
    No *inicio;
    No *fim;
    size_t tam;
} Lista;

typedef struct _listaHist {
    NoFilaHist *inicio;
    NoFilaHist *fim;
    size_t tam;
} ListaHist;

typedef struct _pilha {
    Lista *dados;
} Pilha;

typedef struct _fila {
    Lista *dados;
} Fila;

typedef struct _filaHist {
    ListaHist *dados;
} FilaHist;

No *cria_no(char *nome_doc, int quant_paginas) {
    No *no_criado = (No*) calloc(1, sizeof(No));
    no_criado->nome = (char*) malloc(strlen(nome_doc) + 1);
    strcpy(no_criado->nome, nome_doc);
    no_criado->paginas = quant_paginas;
    no_criado->prox = NULL;
    no_criado->ant = NULL;
    return no_criado;
}

NoFilaHist *cria_noFilaHist (char *nome_i, char *nome_d, int quant_paginas, char *historico) {
    NoFilaHist *no_criado = (NoFilaHist*) calloc(1, sizeof(NoFilaHist));
    no_criado->nome_i = (char*) malloc(strlen(nome_i) + 1);
    strcpy(no_criado->nome_i, nome_i);
    no_criado->nome_d = (char*) malloc(strlen(nome_d) + 1);
    strcpy(no_criado->nome_d, nome_d);
    no_criado->paginas = quant_paginas;
    no_criado->historico = (char*) malloc(strlen(historico) + 1);
    strcpy(no_criado->historico, historico);
    no_criado->prox = NULL;
    no_criado->ant = NULL;
    //printf("Criado NoPilha: Impressora %s, Documento %s, Páginas %d\n", no_criado->nome_i, no_criado->nome_d, no_criado->paginas);
    return no_criado;
}

Lista *cria_lista() {
    Lista *L = (Lista*) calloc(1, sizeof(Lista));
    L->inicio = NULL;
    L->fim = NULL;
    L->tam = 0;
    return L;
}

ListaHist *cria_listaHist() {
    ListaHist *L = (ListaHist*) calloc(1, sizeof(ListaHist));
    L->inicio = NULL;
    L->fim = NULL;
    L->tam = 0;
    //printf("OK\n");
    return L;
}

Fila *cria_estrutura() {
    Fila *F = (Fila *) calloc(1, sizeof(Fila));
    F->dados = cria_lista();
    return F;
}

Pilha *cria_estruturaP(){
    Pilha *F = (Pilha *) calloc(1, sizeof(Pilha));
    F->dados = cria_lista();
    return F;
}

FilaHist *cria_filaHist(){
    FilaHist *F = (FilaHist *) calloc(1, sizeof(FilaHist));
    F->dados = cria_listaHist();
    //printf("todi certo\n");
    return F;
}


int lista_vazia(const Lista *L) {
    return (L->tam == 0);
}

int lista_vaziaHist(const ListaHist *L) {
    return (L->tam == 0);
}

int nomeNesta(const FilaHist *L, const char *nome){
    if (L == NULL || nome == NULL) {
        fprintf(stderr, "Erro: lista ou nome nulos.\n");
        return -1;  // Valor de erro para parâmetros inválidos
    }

    NoFilaHist *atual = L->dados->inicio;
    while (atual != NULL) {
        if (strcmp(atual->nome_i, nome) == 0) {
            return 0;  // Nome encontrado
        }
        atual = atual->ant;
    }
    return 1;  // Nome não encontrado
}

void adicionar_noFila(Fila *F, char *nome_doc, int quant_paginas) {
    No *no = cria_no(nome_doc, quant_paginas);

    if (lista_vazia(F->dados)) {
        F->dados->inicio = no;
        F->dados->fim = no;
    } else {
        F->dados->fim->prox = no;
        no->ant = F->dados->fim;
        F->dados->fim = no;
    }

    F->dados->tam++;
}

void adicionar_noPilha(Pilha *P, char *nome_doc, int quant_paginas) {
    No *no = cria_no(nome_doc, quant_paginas);

    if (lista_vazia(P->dados)) {
        P->dados->inicio = no;
        P->dados->fim = no;
    } else {
        P->dados->inicio->ant = no;
        no->prox = P->dados->inicio;
        P->dados->inicio = no;
    }

    P->dados->tam++;
    //printf("Adicionado na pilha: Impressora %s, Documento %s, Páginas %d\n", nome_i, nome_doc, quant_paginas);
}

void adicionar_noFilaHist(FilaHist *F, char *nome_i, char *nome_doc, int quant_paginas, char *historico) {
    NoFilaHist *no = cria_noFilaHist(nome_i, nome_doc, quant_paginas, historico);

    if (lista_vaziaHist(F->dados)) {
        F->dados->inicio = no;
        F->dados->fim = no;
    } else {
        F->dados->fim->prox = no;
        no->ant = F->dados->fim;
        F->dados->fim = no;
    }

    F->dados->tam++;
}

void imprimir_fila(Fila *F) {
    No *atual = F->dados->inicio;
    while (atual != NULL) {
        //printf("Documento: %s, Páginas: %d\n", atual->nome, atual->paginas);
        atual = atual->prox;
    }
}

void imprimir_pilha(Pilha *P, FILE* output) {
    No *atual = P->dados->inicio;
    while (atual != NULL) {
        fprintf(output, "%s-%dp\n", atual->nome, atual->paginas);
        //printf("Documento: %s, Páginas: %d\n", atual->nome, atual->paginas);
        atual = atual->prox;
    }
}

void imprimir_filaHist(FilaHist *F) {
    NoFilaHist *atual = F->dados->inicio;
    while (atual != NULL) {
        //printf("[%s] %s\n", atual->nome_i, atual->nome_d);
        atual = atual->prox;
    }
}

void remove_doc(Fila *fila_d, No *d) {
    if (d == NULL) {
        printf("Documento nulo não pode ser removido.\n");
        return;
    }

    if (d == fila_d->dados->inicio) {
        fila_d->dados->inicio = d->prox;
    }

    if (d == fila_d->dados->fim) {
        fila_d->dados->fim = d->ant;
    }

    if (d->ant != NULL) {
        d->ant->prox = d->prox;
    }

    if (d->prox != NULL) {
        d->prox->ant = d->ant;
    }

    free(d->nome);
    free(d);

    fila_d->dados->tam--;
}

void remove_doc_p(FilaHist *fila_d, NoFilaHist *d) {
    if (d == NULL) {
        printf("Documento nulo não pode ser removido.\n");
        return;
    }

    if (d == fila_d->dados->inicio) {
        fila_d->dados->inicio = d->prox;
    }

    if (d == fila_d->dados->fim) {
        fila_d->dados->fim = d->ant;
    }

    if (d->ant != NULL) {
        d->ant->prox = d->prox;
    }

    if (d->prox != NULL) {
        d->prox->ant = d->ant;
    }

    free(d->nome_i);
    free(d->nome_d);
    free(d->historico);
    free(d);

    fila_d->dados->tam--;
}

int imprime(int num) {
    return num - 1;
}

void impressao(Fila *I, Fila *D, int num_impressoras, int num_documentos, FILE* output) {
    //printf("\n\n\niniciando impressão\n");

    FilaHist *P = cria_filaHist();

    FilaHist *F_doc = cria_filaHist();
    Pilha *P_aux = cria_estruturaP();

    int controla_i = 0;
    No *i_atual = I->dados->inicio;
    No *doc_atual = D->dados->inicio;

    int pag = 0;
    No *temp_doc = D->dados->inicio;
    for (int b = 0; b < num_documentos; b++) {
        pag += temp_doc->paginas;
        temp_doc = temp_doc->prox;
    }
    //printf("Total de páginas a imprimir: %d\n", pag);

    for (int i = 0; i < pag; i++) {
        if (controla_i == num_impressoras) {
            controla_i = 0;
            i_atual = I->dados->inicio;
        }

        //printf("Impressora: %s\n", i_atual->nome);

        if (i_atual->paginas == 0) {//ESTA OCIOSA


            if(doc_atual->paginas == 1){
                i_atual->paginas = 2;
            }else{
                i_atual->paginas = doc_atual->paginas;
            }

            adicionar_noFilaHist(F_doc, i_atual->nome, doc_atual->nome, doc_atual->paginas, doc_atual->nome);
            char hist[200];
            char str[20];
            sprintf(str, "%d", doc_atual->paginas);
            snprintf(hist, sizeof(hist), "%s-%sp", doc_atual->nome, str);

            NoFilaHist *atual = P->dados->inicio;
            int encontrado = 0;
            while (atual != NULL) {
                if (strcmp(i_atual->nome, atual->nome_i) == 0) {
                    size_t hist_len = strlen(atual->historico);
                    size_t new_hist_len = hist_len + strlen(hist) + 3;
                    char *novo_hist = (char*) malloc(new_hist_len);
                    snprintf(novo_hist, new_hist_len, "%s, %s", hist, atual->historico);

                    //printf("[%s] %s\n", i_atual->nome, novo_hist);
                    fprintf(output, "[%s] %s\n", i_atual->nome, novo_hist);
                    //free(atual->historico);
                    atual->historico = novo_hist;
                    encontrado = 1;
                    break;
                }
                atual = atual->prox;
            }

            if (!encontrado) {
                //printf("[%s] %s\n", i_atual->nome, hist);
                fprintf(output, "[%s] %s\n", i_atual->nome, hist);
                adicionar_noFilaHist(P, i_atual->nome, doc_atual->nome, doc_atual->paginas, hist);
            }
            remove_doc(D, doc_atual);
            doc_atual = D->dados->inicio;
        }

        if (i_atual->paginas == 1){
            NoFilaHist *Nnova = F_doc->dados->inicio;
            while(Nnova != NULL){
                if(strcmp(Nnova->nome_i, i_atual->nome) == 0){
                    //printf("%s\n\n", Nnova->nome_d);
                    adicionar_noPilha(P_aux, Nnova->nome_d, Nnova->paginas);
                    remove_doc_p(F_doc, Nnova);
                    break;
                }else{
                    Nnova = Nnova->prox;
                }
            }

            i_atual->paginas = 0;
            doc_atual = D->dados->inicio;
        }

        if(i_atual->paginas != 0){
            i_atual->paginas = i_atual->paginas-1;
        }


        if(D->dados->tam == 0){
            break;
        }

        i_atual = i_atual->prox;
        controla_i++;
    }

    //printf("%dp\n\n", pag);
    fprintf(output, "%dp\n", pag);
    //imprimir_filaHist(P);
    imprimir_filaHist(F_doc);
    NoFilaHist *Nnova = F_doc->dados->inicio;
        while(Nnova != NULL){
            adicionar_noPilha(P_aux, Nnova->nome_d, Nnova->paginas);
            Nnova = Nnova->prox;
        }
    //puts("");
    imprimir_pilha(P_aux, output);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Uso: %s <arquivo_entrada> <arquivo_saida>\n", argv[0]);
        return 1;
    }

    FILE* input = fopen(argv[1], "r");
    if (input == NULL) {
        perror("Erro ao abrir arquivo de entrada");
        return 1;
    }

    FILE* output = fopen(argv[2], "w");
    if (output == NULL) {
        perror("Erro ao abrir arquivo de saída");
        fclose(input);
        return 1;
    }

    printf("Quantidade de argumentos (argc): %i\n", argc);
	// Iterando sobre o(s) argumento(s) do programa
	for(uint32_t i = 0; i < argc; i++) {
		// Mostrando o argumento i
		printf("Argumento %i (argv[%i]): %s\n", i, i, argv[i]);
	}

    char linha[100];
    int estado = 0;

    int num_impressoras = 0;
    int num_documentos = 0;
    int i = 0;
    int d = 0;

    Fila *doc = cria_estrutura();
    Fila *impressoras = cria_estrutura();

    while (fgets(linha, sizeof(linha), input) != NULL) {
        linha[strcspn(linha, "\n")] = 0;

        switch (estado) {
            case 0:
                num_impressoras = atoi(linha);
                //printf("Número de impressoras: %d\n", num_impressoras);
                estado = 1;
                break;
            case 1:
                if (i < num_impressoras) {
                    //char nome[50];
                    int st = 0;
                    char *nome = (char *)malloc(51*sizeof(char));
                    sscanf(linha, "%s %d", nome, &st);
                    //printf("%s %d\n", nome, st);
                    adicionar_noFila(impressoras, nome, st);
                    i++;
                }
                if (i == num_impressoras) {
                    estado = 2;
                }
                break;
            case 2:
                num_documentos = atoi(linha);
                //printf("Número de documentos: %d\n", num_documentos);
                estado = 3;
                break;
            case 3:
                if (d < num_documentos) {
                    //char nome[10];
                    char *nome = (char *)malloc(51*sizeof(char));
                    int pag;
                    sscanf(linha, "%s %d", nome, &pag);
                    //printf("%s %d\n", nome, pag);
                    adicionar_noFila(doc, nome, pag);
                    d++;
                }
                if (d == num_documentos) {
                    estado = 4;
                }
                break;
            default:
                printf("Linha não processada: %s\n", linha);
                break;
        }
    }

    impressao(impressoras, doc, num_impressoras, num_documentos, output);

    fclose(input);
    fclose(output);
    return 0;
}
