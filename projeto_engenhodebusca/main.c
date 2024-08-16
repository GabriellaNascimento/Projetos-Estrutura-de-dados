#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Req{
    int id;
    char *descricao;
} Requisicao;

typedef struct Serv{
    int id;
    Requisicao *requisicoes;
    int quant_req;
} Servidor;

int calcula_checksum(const char *text) {
    int checksum = 0;
    while (*text){
        if (*text == ' '){
            text++;
            continue;
        }
        checksum ^= *text;
        text++;
    }
    return checksum;
}

int H1(const char *text){
    int checksum = calcula_checksum(text);
    return (7919 * checksum);
}

int H2(const char *text){
    int checksum = calcula_checksum(text);
    return (104729 * checksum + 123);
}

int insere_req(int indice, char *nome_requis, int primeiro_serv, Servidor *servidores, int max_req_serv, FILE *output){
    int quant_req = servidores[indice].quant_req;
    if(servidores[indice].quant_req < max_req_serv){
        servidores[indice].requisicoes[quant_req].descricao = malloc(strlen(nome_requis) + 1);
        strcpy(servidores[indice].requisicoes[quant_req].descricao, nome_requis);
        servidores[indice].quant_req++;
        //printf("%s\n\n", servidores[indice].requisicoes[quant_req].descricao);
        int i;
        if(indice != primeiro_serv){
            fprintf(output, "S%d->S%d\n", primeiro_serv, indice);
        }

        fprintf(output, "[S%d]", indice);
        for(i = 0; i <= quant_req; i++){
            if(i == quant_req){
                fprintf(output, "%s\n", servidores[indice].requisicoes[i].descricao);
                break;
            }
            fprintf(output, "%s,", servidores[indice].requisicoes[i].descricao);
        }

        return 1;
    }else{
        return 0;
    }
}

int main(int argc, char* argv[]) {
    /*const char *str = "ab";
    char *str1 = "a b";
    int checksum = calcula_checksum(str);
    int checksum1 = calcula_checksum(str1);
    printf("Checksum of \"%s\" is %d\n\n", str, checksum); // Sa�da: 96
    printf("Checksum of \"%s\" is %d\n\n", str1, checksum1); // Sa�da: 96*/


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

	int T, max_req_serv, quant_req;

	fscanf(input, "%d %d", &T, &max_req_serv);
    fscanf(input, "%d", &quant_req);

    //gambiarra
    char bf[100];
    fgets(bf, sizeof(bf), input);

    //printf("\nServ: %d Max req por serv: %d\n", T, max_req_serv);
    //printf("Quant de req: %d\n\n", quant_req);

    Servidor *servidores = (Servidor *) malloc(T * sizeof(Servidor));

    int i;
    for (i = 0; i < T; i++){
        servidores[i].id = i;
        servidores[i].quant_req = 0;
        servidores[i].requisicoes = (Requisicao *)malloc(max_req_serv * sizeof(Requisicao));
    }

    int numero;
    /*int val = numero*100;
    char linha[val];*/
    for (i = 0; i < quant_req; i++){
        fscanf(input, "%d", &numero);
        int val = numero * 100;

        // Alocar dinamicamente mem�ria para 'linha'
        char *linha = (char *)malloc(val * sizeof(char));

        // Ler a linha do arquivo
        fgets(linha, val, input);
        linha[strcspn(linha, "\n")] = '\0';  // Remover a nova linha

        /*printf("N�mero: %d\n", numero);
        printf("val: %d\n", val);
        printf("Resto da linha: %s\n", linha);*/


        /*fscanf(input, "%d", &numero);
        fgets(linha, sizeof(linha), input);
        linha[strcspn(linha, "\n")] = '\0';
        //char *nome_req = strtok(linha, " "); // Ignora o n�mero
        //nome_req = strtok(NULL, "");
        printf("Resto da linha: %s\n", linha);*/

        int hash1 = H1(linha);
        //printf("hash 1 = %d\n", hash1);
        //printf("Indice hash 1 = %d\n", hash1 % T);

        int hash2 = H2(linha);
        //printf("hash 2 = %d\n\n", hash2);

        int primeiro_serv = hash1 % T;
        //printf("%d\n", primeiro_serv);

        int b;
        for(b = 0; b < T; b++){
            int indice;
            indice = (hash1 + b*hash2) % T;
            //printf("\n%d\n", indice);
            if(insere_req(indice, linha, primeiro_serv, servidores, max_req_serv, output))
                break;
            else{
                if(b == T-1){
                    indice++;
                    if(insere_req(indice, linha, primeiro_serv, servidores, max_req_serv, output))
                        break;
                }
            }
            /*else{
                //indice++;
                indice = (hash1 + (b+1)*hash2) % T;
                //printf("\n%d\n", indice);
                if(insere_req(indice, linha, primeiro_serv, servidores, max_req_serv, output)){
                    //printf("S%d -> S%d\n", primiro_serv, indice);
                    //fprintf(output, "S%d -> S%d\n", primiro_serv, indice);
                    break;
                }
            }*/
        }
        free(linha);
    }

    //servidores[0].id = 3;
    //servidores[1].id = 3;
    //strcpy(servidores[0].requisicoes[0].descricao, "Requisicao 1 para Servidor 1");
    //printf("\n%d\n%d", servidores[0].id, servidores[1].id);
    //printf("\n%s\n", servidores[0].requisicoes[0].descricao);

	// Fechando os arquivos
	fclose(input);
	fclose(output);
	// Finalizando programa
    return 0;
}
