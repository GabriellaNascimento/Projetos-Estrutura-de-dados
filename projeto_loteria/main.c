#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

typedef struct no{  
	char codigo[129];
    int valor;   
}no;

no criaNo(char codigo[129], int valor){

    no filho;
    filho.valor = valor;

    strcpy(filho.codigo, codigo);

    return filho;
}

void heapifyMax(no *raiz, int quant_apostas, int indice){
 
    no aux;
    
    int pai = indice;
	int direita = 2*indice + 2;
    int esquerda = 2*indice + 1;
    

    if(esquerda < quant_apostas && raiz[esquerda].valor > raiz[pai].valor){
        pai = esquerda; 
    }
    if(direita < quant_apostas && raiz[direita].valor > raiz[pai].valor ){
        pai = direita;
    }
    if(pai != indice){

        aux = raiz[indice];
        raiz[indice] = raiz[pai];
        raiz[pai] = aux;
        heapifyMax(raiz, quant_apostas, pai);
    }

}
    

void heapifyMin(no *raiz, int quant_apostas, int indice){
 
    no aux;
    
    int pai = indice;
	int direita = 2*indice + 2;
    int esquerda = 2*indice + 1;
    
    if(esquerda < quant_apostas && raiz[esquerda].valor < raiz[pai].valor){
        pai = esquerda; 
    }
    if(direita < quant_apostas && raiz[direita].valor < raiz[pai].valor ){
        pai = direita;
    }
    if(pai != indice){
        aux = raiz[indice];
        raiz[indice] = raiz[pai];
        raiz[pai] = aux;
        heapifyMin(raiz, quant_apostas, pai);
    }

}

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

    int premio;

    fscanf(input, "%d", &premio);
    //printf("%d\n", premio);

	int quant_apostas;
	fscanf(input, "%d", &quant_apostas);
    //printf("%d\n", quant_apostas);

	int resultado[10];
	for(int i = 0; i < 10; i++){
        fscanf(input,"%d", &resultado[i]);
    }	

	no* vetorMax = (no*)malloc(quant_apostas*sizeof(no));
    no* vetorMin = (no*)malloc(quant_apostas*sizeof(no));
	
	int apost;
	char codigo[129];
	for(int i = 0; i < quant_apostas; i++){
        int valor = 0;
        fscanf(input, "%s", codigo);
		//printf("%s\n", codigo);
        for(int j = 0; j < 15; j++){
            fscanf(input, "%d", &apost);
			//printf("%d\n", apost);
            for(int k = 0; k < 10 ; k++){
                if(apost == resultado[k]){
                    valor++;
                }
            }
        }

        vetorMax[i] = criaNo(codigo, valor);
        vetorMin[i] = criaNo(codigo, valor);
    }

	for (int i = quant_apostas - 1; i >= 0; i--)
    {
        heapifyMax(vetorMax, quant_apostas, i);
        heapifyMin(vetorMin, quant_apostas, i);
    }

	int vencedores = 0, perdedores = 0;

    for(int i = 0; i < quant_apostas; i++){
        if(vetorMax[i].valor == vetorMax[0].valor){
            vencedores++;
        }
        if(vetorMin[i].valor == vetorMin[0].valor){
            perdedores++;
        }

    }

	fprintf(output, "[%d:%d:%d]\n", vencedores, vetorMax[0].valor, (premio / 2) /vencedores);
    int max = vetorMax[0].valor;
    int aux = quant_apostas;

    for(int i = 0; i < quant_apostas; i++){
        if(vetorMax[0].valor == max){
            fprintf(output, "%s\n", vetorMax[0].codigo);
            vetorMax[0] = vetorMax[aux - 1];
            aux--;
            heapifyMax(vetorMax, aux, 0);
        }
    }

    fprintf(output, "[%d:%d:%d]\n", perdedores, vetorMin[0].valor, (premio / 2) / perdedores);
    int min = vetorMin[0].valor;
    aux = quant_apostas;

    for(int i = 0; i < quant_apostas; i++){
         if(vetorMin[0].valor == min){
            fprintf(output, "%s\n", vetorMin[0].codigo);
            vetorMin[0] = vetorMin[aux - 1];
            aux--;
            heapifyMin(vetorMin, aux, 0);
        }
    }
	
	// Fechando os arquivos
	fclose(input);
	fclose(output);

	// Finalizando programa
	return 0;
}