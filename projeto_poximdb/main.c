#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct no
{
    // Vetor de chaves
    uint32_t *chaves;

    // Vetor de filhos
    struct no **filhos;

    // Quantidade utilizada
    uint32_t quant_utilizada;

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
	
	// Fechando os arquivos
	fclose(input);
	fclose(output);

	// Finalizando programa
	return 0;
}