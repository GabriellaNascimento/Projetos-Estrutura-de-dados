#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct arvore
{
    int ordem;
    char nome[51];
    char tipo[3];
    int tamanho;

    // Filho da direita
    struct arvore *Direita ;
    // Filho da esquerda
    struct arvore *Esquerda ;
} Arvore;

Arvore* criar_arvore(){
    return NULL;    
}

int controla = 0;
void insere_no(Arvore** A, char nome[], char tipo[], int tamanho)
{
    if (*A == NULL)
    {
        *A = (Arvore*) calloc(1,sizeof(Arvore));
        (*A)->Direita = NULL;
        (*A)->Esquerda = NULL;
        strcpy((*A)->nome, nome);
        strcpy((*A)->tipo, tipo);
        (*A)->tamanho = tamanho;
        (*A)->ordem = controla;
        controla++;
    }else
    {
        //strcmo = 1 -> o no que desejo colocar é maior
        int cmp = strcmp(nome, (*A)->nome);

        if(cmp > 0)
        {
            insere_no(&(*A)->Direita, nome, tipo, tamanho);
        }
        if(cmp < 0)
        {
            insere_no(&(*A)->Esquerda, nome, tipo, tamanho);
        }
        if(cmp == 0)
        {
            if(strcmp((*A)->tipo, "rw") == 0)
            {
                strcpy((*A)->tipo, tipo);
                (*A)->tamanho = tamanho;
                (*A)->ordem = controla;
            }
        }
    }
}

void verifica(int tamanho, FILE *output)
{
    if(tamanho == 1)
    {
        fprintf(output, " byte\n");
    }else
    {
        fprintf(output, " bytes\n");
    }
}


void mostrar_PED(Arvore* A, FILE *output)
{
    if (A == NULL) return;

    fprintf(output, "%d %s %s %d", A->ordem, A->nome, A->tipo, A->tamanho);
    verifica(A->tamanho, output);
    /*printf("%s ", A->nome);
    printf("%s ", A->tipo);
    printf("%d ", A->tamanho);
    printf("Ordem: %d ", A->ordem);*/
    mostrar_PED(A->Esquerda, output);
    mostrar_PED(A->Direita, output);
}

void mostrar_EPD(Arvore* A, FILE *output)
{
    if (A == NULL) return;

    mostrar_EPD(A->Esquerda, output);
    fprintf(output, "%d %s %s %d", A->ordem, A->nome, A->tipo, A->tamanho);
    verifica(A->tamanho, output);
    mostrar_EPD(A->Direita, output);
}

void mostrar_EDP(Arvore* A, FILE *output)
{
    if (A == NULL) return;

    mostrar_EDP(A->Esquerda, output);
    mostrar_EDP(A->Direita, output);
    fprintf(output, "%d %s %s %d", A->ordem, A->nome, A->tipo, A->tamanho);
    verifica(A->tamanho, output);

}

int main(int argc, char* argv[]) {
	// Exibindo a quantidade de argumentos
	printf("Quantidade de argumentos (argc): %i\n", argc);
	// Iterando sobre o(s) argumento(s) do programa
	for(uint32_t i = 0; i < argc; i++) {
		// Mostrando o argumento i
		printf("Argumento %i (argv[%i]): %s\n", i, i, argv[i]);
	}

	// Abrindo os arquivos com as permissoes corretas
	FILE* input = fopen(argv[1], "r");
	FILE* output = fopen(argv[2], "w");

    Arvore* A = criar_arvore();

    int quant_arquivos;
	fscanf(input, "%d", &quant_arquivos);
    //printf("%d\n", quant_arquivos);

    char nome[51];
    char tipo[3]; 
    int tamanho; 

    int i;
	for(i = 0; i < quant_arquivos; i++){
        fscanf(input, " %[^ ] %[^ ] %d", nome, tipo, &tamanho);
        /*printf("Nome do livro: %s\n", nome);
        printf("Nome do autor: %s\n", tipo);
        printf("ISBN: %d\n\n", tamanho);*/
        insere_no(&A, nome, tipo, tamanho);
	}

    fprintf(output, "EPD:\n");
    mostrar_EPD(A, output);
    fprintf(output, "PED:\n");
    mostrar_PED(A, output);
    fprintf(output, "EDP:\n");
    mostrar_EDP(A, output);
	
	// Fechando os arquivos
	fclose(input);
	fclose(output);
	// Finalizando programa
	return 0;
}