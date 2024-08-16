#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct livros{
    char isbn[14];
    char nome_autor[51];
    char nome_livro[101];
}Livros;

int32_t bbi(int32_t* count, char pesquisa[14], Livros* livros, uint32_t n) {
    // Índices de partições
    int32_t i = 0, j = n - 1;
    int32_t p = (i + j) / 2;

    // Iterações de 1 -> k
    while (j >= i && strcmp(livros[p].isbn, pesquisa) != 0) {
        (*count)++;
        if (strcmp(livros[p].isbn, pesquisa) > 0)
            j = p - 1;
        else
            i = p + 1;
        p = (i + j) / 2;
    }
    (*count)++;

    return (strcmp(livros[p].isbn, pesquisa) == 0) ? p : -1;
}


int32_t interpolada(int32_t* count, char pesquisa[14], Livros* livros, uint32_t n) {
    int32_t i = 0, j = n - 1;

    //&& strcmp(pesquisa, livros[i].isbn) >= 0 && strcmp(pesquisa, livros[j].isbn) <= 0
    while (i <= j) {
        (*count)++;

        // Converter ISBN para inteiro para cálculo
        long long isbn_i = strtoll(livros[i].isbn, NULL, 10);
        //printf("ISBN: %s\n", livros[i].isbn);
        long long isbn_j = strtoll(livros[j].isbn, NULL, 10);
        //printf("ISBN: %lld\n", isbn_i);
        //printf("ISBN F: %lld\n\n", isbn_j);

        // Evitar divisão por zero
        if (isbn_j == isbn_i) {
            return strcmp(livros[i].isbn, pesquisa) == 0 ? i : -1;
        }

        // Calcular o índice provável usando a heurística
        int32_t range = j - i + 1;
        int32_t p = i + ((isbn_j - isbn_i) % range);

        // Verificar se o índice calculado está dentro do intervalo
        if (p < i || p > j) break;

        int cmp = strcmp(livros[p].isbn, pesquisa);
        if (cmp == 0) return p;
        if (cmp < 0) i = p + 1;
        else j = p - 1;
    }
    return -1;
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

	int quant_livros;
	fscanf(input, "%d", &quant_livros);
	//printf("Quantidade de livros: %d\n", quant_livros);

	Livros* livros = (Livros*)malloc(quant_livros * sizeof(Livros));

	int i;
	for(i = 0; i < quant_livros; i++){
        fscanf(input, "%s %50[^&]&%100[^\n]", livros[i].isbn, livros[i].nome_autor, livros[i].nome_livro);
        /*printf("Nome do livro: %s\n", livros[i].nome_livro);
        printf("Nome do autor: %s\n", livros[i].nome_autor);
        printf("ISBN: %s\n\n", livros[i].isbn);*/
	}

	int quant_consultas;
	fscanf(input, "%d", &quant_consultas);
	//printf("Quantidade de consultas: %d\n", quant_consultas);

    char pesquisa[quant_consultas][14];
    for (int i = 0; i < quant_consultas; i++) {
        fscanf(input, "%s", pesquisa[i]);
        //printf("Consula %d: %s\n", i, pesquisa[i]);
    }

    int consulta_binaria, consulta_interpolada;
    int vitorias_binaria = 0, vitorias_interpolada = 0;
    int total_b = 0 , total_i = 0;

    for (i = 0; i < quant_consultas; i++) {
        consulta_binaria = 0;
        consulta_interpolada = 0;

        //chamar as funcoes
        int32_t resultado_binario = bbi(&consulta_binaria, pesquisa[i], livros, quant_livros);
        int32_t resultado_interpolado = interpolada(&consulta_interpolada, pesquisa[i], livros, quant_livros);

        fprintf(output, "[%s]", pesquisa[i]);
        fprintf(output, "B=%d,", consulta_binaria);
        fprintf(output, "I=%d", consulta_interpolada);

        total_b += consulta_binaria;
        total_i += consulta_interpolada;

        if(consulta_binaria >= consulta_interpolada){
            vitorias_interpolada++;
        }else{
            vitorias_binaria++;
        }


        if(resultado_binario != -1 || resultado_interpolado != -1){
            fprintf(output, ":Author:%s,Title:%s\n", livros[resultado_binario].nome_autor, livros[resultado_binario].nome_livro);
        }else{
            fprintf(output, ":ISBN_NOT_FOUND\n");
        }
    }

    fprintf(output, "BINARY=%d:%d\n", vitorias_binaria, total_b/quant_consultas);
    fprintf(output, "INTERPOLATION=%d:%d\n", vitorias_interpolada, total_i/quant_consultas);


	// Fechando os arquivos
	fclose(input);
	fclose(output);
	// Finalizando programa
	return 0;
}
