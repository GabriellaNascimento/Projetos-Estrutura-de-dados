#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct sinonimo
{
    char palavra[31];
} Sinonimo;

typedef struct no
{
	int fator_balanceamento; 

    char nome[31];
	int quant_sinonimos; 
	char *sinonimos;

    // Filho da direita
    struct no *Direita ;
    // Filho da esquerda
    struct no *Esquerda ;
} No;

No* criar_arvore()
{
    return NULL;    
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

// Função para obter a altura de um nó
int altura(No* N) {
    if (N == NULL)
        return 0;
    return 1 + max(altura(N->Esquerda), altura(N->Direita));
}

int fator_balanceamento(No* N) {
    if (N == NULL)
        return 0;
    return altura(N->Direita) - altura(N->Esquerda);
}

// Função para rotação à direita
void rotacao_direita(No** raiz) {
    No* eixo = (*raiz)->Esquerda;
    (*raiz)->Esquerda = eixo->Direita;
	eixo->Direita = (*raiz);
	(*raiz) = eixo;

    (*raiz)->Direita->fator_balanceamento = 0;
    (*raiz)->fator_balanceamento = 0;
}

// Função para rotação à esquerda
void rotacao_esquerda(No** raiz) {
    No* eixo = (*raiz)->Direita;
    (*raiz)->Direita = eixo->Esquerda;
	eixo->Esquerda = (*raiz);
    (*raiz) = eixo;

    (*raiz)->Esquerda->fator_balanceamento = 0;
    (*raiz)->fator_balanceamento = 0;
}

void insere_no(No** raiz, char nome[], char sinonimos[], int quant_sinonimos)
{
    if (*raiz == NULL)
    {
        *raiz = (No*) calloc(1,sizeof(No));
        (*raiz)->Direita = NULL;
        (*raiz)->Esquerda = NULL;
        strcpy((*raiz)->nome, nome);
		(*raiz)->quant_sinonimos = quant_sinonimos;
		(*raiz)->fator_balanceamento = 0;
		// Alocar memória para a string de sinônimos
        (*raiz)->sinonimos = (char*) calloc(strlen(sinonimos) + 1, sizeof(char));
        strcpy((*raiz)->sinonimos, sinonimos);

		//printf("No: %s\n", (*raiz)->nome);
    }else{
		int cmp = strcmp(nome, (*raiz)->nome);

		if(cmp > 0)
		{
			insere_no(&(*raiz)->Direita, nome, sinonimos, quant_sinonimos);
		}
		if(cmp < 0)
		{
			insere_no(&(*raiz)->Esquerda, nome, sinonimos, quant_sinonimos);
		}

		// Atualizar fator de balanceamento
		(*raiz)->fator_balanceamento = fator_balanceamento(*raiz);
		// printf("No: %s\n", (*raiz)->nome);
		// printf("Nome: %s\n", nome);
		// printf("FDB: %d\n", (*raiz)->fator_balanceamento);

		int valor_balanceamento = (*raiz)->fator_balanceamento;
		
		// Caso de rotação à esquerda
        if ((*raiz)->fator_balanceamento > 1)
        {
            if (strcmp(nome, (*raiz)->Direita->nome) > 0) {
                rotacao_esquerda(raiz);
            }
            else {
                rotacao_direita(&(*raiz)->Direita);
                rotacao_esquerda(raiz);
            }
        }

		if ((*raiz)->fator_balanceamento < -1)
        {
            if (strcmp(nome, (*raiz)->Esquerda->nome) < 0) {
                rotacao_direita(raiz);
            }
            else {
                rotacao_esquerda(&(*raiz)->Esquerda);
                rotacao_direita(raiz);
            }
        }
	}
}

void buscar_palavra(No* A, char palavra[], FILE *output)
{
	// 1 = primeiro arg é maior
    // 0 = os dois arg são iguais
    // -1 = primeiro arg é menor
	if (A == NULL) {
        fprintf(output, "?]\n-");
        return;
    }

	int cmp = strcmp(palavra, A->nome);
    if (cmp > 0) {
        fprintf(output, "%s->", A->nome);
        buscar_palavra(A->Direita, palavra, output);
    }
    else if (cmp < 0) {
        fprintf(output, "%s->", A->nome);
        buscar_palavra(A->Esquerda, palavra, output);
    }
    else {
        fprintf(output, "%s]\n", A->nome);
        // for(int i = 0; i < A->quant_sinonimos; i++)
        // {
        //     fprintf(output, "%s", A->sinonimos[i].palavra);
        //     if(i+1 < A->quant_sinonimos){
        //         fprintf(output, ", ");
        //     }
        // }
		// Imprimir os sinônimos separados por vírgula e espaço
    }
}

// Funcao principal
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

	// Quantidade de palavras que serão lidas
    int num_palavras;
	fscanf(input, "%d", &num_palavras);
	//printf("Num de palavras: %d\n", num_palavras);

	char palavra[31];
	int quant_sin;
	char sinonimos[300];

	// Inserção de cada palavra na árvore
	for(int i = 0; i < num_palavras; i++)
    {
        fscanf(input, " %[^ ] %d", palavra, &quant_sin);
		fgets(sinonimos, sizeof(sinonimos), input);
		insere_no(&A, palavra, sinonimos, quant_sin);
	}

	// Quantidade de buscas 
    int num_buscas;
	fscanf(input, "%d", &num_buscas);
	//printf("Num de buscas: %d\n", num_buscas);

	char palavra_busca[31];

	for(int i = 0; i < num_buscas; i++)
    {
		fscanf(input, " %s", palavra_busca);
		fprintf(output,"[");
		buscar_palavra(A, palavra_busca, output);
		fprintf(output,"\n");
	}

	
	// Fechando os arquivos
	fclose(input);
	fclose(output);

	// Finalizando programa
	return 0;
}