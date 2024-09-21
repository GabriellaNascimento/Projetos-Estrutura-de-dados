#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int ordem;

typedef struct arq
{
	char nome[31];
	int tamanho;
	char hash[33];
} arquivo;

typedef struct no
{
    // Vetor de chaves
    uint32_t *chaves;

    // Vetor de filhos
    struct no **filhos;

    // Quantidade utilizada
    uint32_t quant_utilizada;

	int ehFolha;
	arquivo *arquivos;
} no;

// Funcao principal
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

	fscanf(input, "%d", &ordem);
	printf("A ordem da arvore é: %d\n", ordem);

	int quant_arquivos;
	fscanf(input, "%d", &quant_arquivos);
	printf("A quant de arquivos é: %d\n", quant_arquivos);

	char nome[31];
	int tamanho;
	char hash[33];

    for(int i = 0; i < quant_arquivos; i++)
    {
        fscanf(input, " %s %d %s", nome, &tamanho, hash);
		printf("%s %d %s\n", nome, tamanho, hash);
    }

	int quant_operacoes;
	fscanf(input, "%d", &quant_operacoes);
	printf("A quant de operacoes é: %d\n", quant_operacoes);

	char nome_op[7];
	char nome_ar_op[31];
	int tamanho_ar_op;
	char hash_op[33];

	for(int i = 0; i < quant_operacoes; i++)
    {
        fscanf(input, " %s", nome_op);
		printf("%s", nome_op);
		if(strcmp(nome_op, "INSERT") == 0){
			fscanf(input, " %s %d %s", nome_ar_op, &tamanho_ar_op, hash_op);
			printf(" %s %d %s\n", nome_ar_op, tamanho_ar_op, hash_op);
		}else{
			fscanf(input, " %s", hash_op);
			printf(" %s \n", hash_op);
		}
		
    }

	// Fechando os arquivos
	fclose(input);
	fclose(output);

	// Finalizando programa
	return 0;
}