#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct no {
    // Valor de filhos
    struct no** filho;

    // Armazernar o caractere
    char valor; 

    // A palavra completa quando o no representa o final de uma palavra
    char palavra[21]; 

    // Marca se o no contem o final de uma palavra
    uint32_t ordem;

    // Verificar se o no já foi visitado
    int verificador;
} no;

// Inicializar um novo nó
no* criar_no(char caracter){
    no* prefixo = calloc(1, sizeof(no));

    prefixo->filho = (no**)calloc(26, sizeof(no*));
    prefixo->ordem = -1;
    prefixo->valor = caracter;
    prefixo->verificador = 0;

    for(int i = 0; i<26; i++){
        prefixo->filho[i] = (no*)calloc(26, sizeof(no));
        prefixo->filho[i] = NULL;
    }

    return prefixo;
}

no* insere_no(no* raiz, char palavra[21], int controlador, int ordem){
    if(raiz == NULL){
        raiz = criar_no(palavra[controlador-2]);
    }
    if(controlador - 1 == strlen(palavra)){
        raiz->ordem = ordem;
        strcpy(raiz->palavra, palavra);
    }else{
        int pos = palavra[controlador -1] - 'a';
        raiz->filho[pos] = insere_no(raiz->filho[pos], palavra, controlador+1, ordem);
    }

    return raiz;
}

int vir = 0;
void busca_apos(no* raiz, int max, int atual, FILE *output){
    for(int i = 0; i<26; i++){
        if(raiz->filho[i] != NULL){
            if(raiz->filho[i]->ordem != -1  && raiz->filho[i]->verificador == 0 && atual < max){
                if(vir == 1){
                    fprintf(output, ",");   
                    fprintf(output, "%s", raiz->filho[i]->palavra);

                }
                else{

                    fprintf(output,"%s", raiz->filho[i]->palavra);
                    vir = 1;

                }

                raiz->filho[i]->verificador = 1;
                
            }
            if(atual < max ){

                busca_apos(raiz->filho[i], max, atual+1, output);
               
            }
            
            else{

                return;

            }
        }
    }
}

void limpar(no* raiz){
    for(int i = 0; i < 26; i++){
        if(raiz->filho[i] != NULL){
            if(raiz->filho[i]->verificador == 1){
                raiz->filho[i]->verificador = 0;
            }
        }
    }
}

void buscar(no* raiz, char palavra[21], FILE* output){
    no* aux = raiz;

    fprintf(output, "%s:", palavra);
    int verif = 0;
    for(int i = 1; i<=strlen(palavra); i++){
        for(int y = 0; y<26; y++){
            if(aux->filho[y] != NULL){
                if(aux->filho[y]->valor == palavra[verif]){
                    aux = aux->filho[y];
                    busca_apos(aux, i*2, i, output);
                    limpar(aux);
                    verif++;
                    break;
                }
            }
        }
        if(verif==0){
            fprintf(output, "-");
            break;
        }
    }
    vir = 0;
    fprintf(output, "\n");
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

    no* raiz = criar_no('R');

    int quant_palavras;
    fscanf(input, "%d", &quant_palavras);
    // printf("%d\n", quant_palavras);

    char palavra[21];
    for (int i = 0; i < quant_palavras; i++){
        fscanf(input, "%s", palavra);
        // printf("%s\n", palavra);
        raiz = insere_no(raiz, palavra, 1, i);
    }

    int quant_req;
    fscanf(input, "%d", &quant_req);
    // printf("%d\n", quant_req);

    char requisicao[21];
    for (int i = 0; i < quant_req; i++){
        fscanf(input, "%s", requisicao);
        buscar(raiz, requisicao, output);
        // printf("%s\n", requisicao);
    }
	
	// Fechando os arquivos
	fclose(input);
	fclose(output);

	// Finalizando programa
	return 0;
}