#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct no
{
    int ordem;
    char nome[51];
    char tipo[3];
    int tamanho;

    // Filho da direita
    struct no *Direita ;
    // Filho da esquerda
    struct no *Esquerda ;
} No;

No* criar_arvore()
{
    return NULL;    
}

int contador = 0; //Controle da ordem de inserção na árvore
void insere_no(No** raiz, char nome[], char tipo[], int tamanho)
{
    if (*raiz == NULL)
    {
        *raiz = (No*) calloc(1,sizeof(No));
        (*raiz)->Direita = NULL;
        (*raiz)->Esquerda = NULL;
        strcpy((*raiz)->nome, nome);
        strcpy((*raiz)->tipo, tipo);
        (*raiz)->tamanho = tamanho;
        (*raiz)->ordem = contador;
        contador++;
    }else
    {
        // 1 = primeiro arg é maior
        // 0 = os dois arg são iguais
        // -1 = primeiro arg é menor
        int cmp = strcmp(nome, (*raiz)->nome);

        if(cmp > 0)
        {
            insere_no(&(*raiz)->Direita, nome, tipo, tamanho);
        }
        if(cmp < 0)
        {
            insere_no(&(*raiz)->Esquerda, nome, tipo, tamanho);
        }
        if(cmp == 0)
        {
            if(strcmp((*raiz)->tipo, "rw") == 0)
            {
                strcpy((*raiz)->tipo, tipo);
                (*raiz)->tamanho = tamanho;
                (*raiz)->ordem = contador;
                contador++;
            }
        }
    }
}

//Inserir no final "byte ou bytes de acordo com o tamanho"
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

//Percurso pré-ordem - pai, esquerda, direira
void mostrar_PED(No* A, FILE *output)
{
    if (A == NULL) return;

    fprintf(output, "%d %s %s %d", A->ordem, A->nome, A->tipo, A->tamanho);
    verifica(A->tamanho, output);
    mostrar_PED(A->Esquerda, output);
    mostrar_PED(A->Direita, output);
}

//Percurso em ordem - esquerda, pai, direita
void mostrar_EPD(No* A, FILE *output)
{
    if (A == NULL) return;

    mostrar_EPD(A->Esquerda, output);
    fprintf(output, "%d %s %s %d", A->ordem, A->nome, A->tipo, A->tamanho);
    verifica(A->tamanho, output);
    mostrar_EPD(A->Direita, output);
}

//Percurso pós-ordem - esquerda, direita, pai
void mostrar_EDP(No* A, FILE *output)
{
    if (A == NULL) return;

    mostrar_EDP(A->Esquerda, output);
    mostrar_EDP(A->Direita, output);
    fprintf(output, "%d %s %s %d", A->ordem, A->nome, A->tipo, A->tamanho);
    verifica(A->tamanho, output);

}

int main(int argc, char* argv[]) 
{
	// Exibindo a quantidade de argumentos
	printf("Quantidade de argumentos (argc): %i\n", argc);
	// Iterando sobre o(s) argumento(s) do programa
	for(uint32_t i = 0; i < argc; i++) 
    {
		// Mostrando o argumento i
		printf("Argumento %i (argv[%i]): %s\n", i, i, argv[i]);
	}

	// Abrindo os arquivos com as permissoes corretas
	FILE* input = fopen(argv[1], "r");
	FILE* output = fopen(argv[2], "w");

    // Inicializando a árvore
    No* A = criar_arvore();

    // Quantidade de arquivos que serão lidos
    int quant_arquivos;
	fscanf(input, "%d", &quant_arquivos);

    char nome[51];
    char tipo[3]; 
    int tamanho; 

    // Inserção das informações de cada arquivo na árvore
	for(int i = 0; i < quant_arquivos; i++)
    {
        fscanf(input, " %[^ ] %[^ ] %d", nome, tipo, &tamanho);
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