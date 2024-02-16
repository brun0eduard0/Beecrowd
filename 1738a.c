/******************************************
UNIVERSIDADE FEDERAL DO AMAZONAS
BRUNO EDUARDO GOMES BARRETO
DATA: 12/11/2015
ÚLTIMA MODIFICAÇÃO: 12/11/2015
1738 - Rede de Fibra
******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX_STRING 27
#define MAX_MATRIX 200


void inicializa_matriz(int matriz[MAX_MATRIX] [MAX_MATRIX], int tamanho){
	int i =0;
	int j =0;
	for(i=0; i < tamanho; i++){
		for(j=0; j < tamanho; j++){
			matriz[i][j] = 0;
		}
	}
}


void mapea_string(int matriz [MAX_MATRIX][MAX_MATRIX], char *palavra, int linha, int coluna){
	int i = 0;
	int resultado = 0;
	for (i = (strlen(palavra)-1); i >= 0; --i){
		resultado |= (1<<(palavra[i]-'a'));
		matriz[linha][coluna] = resultado;
	}	
}

void floyd_warshall(int matriz[MAX_MATRIX][MAX_MATRIX], int tamanho){
	int i =0;
	int j =0;
	int k =0;
	for(k=0; k < tamanho; k++){
		for(i=0; i < tamanho; i++){
			for(j=0; j < tamanho; j++){
				matriz[i][j] |= (matriz[i][k] & matriz[k][j]);
			}
		}
	}
}

int main (){
	char word[27];
	int i =0;
	int j =0;
	int k =0;
	int linha = 0;
	int coluna = 0;
	int qtd_nos = 0;
	int resultado = 0;
	int matriz[MAX_MATRIX][MAX_MATRIX];
	while (true){
		scanf("%d", &qtd_nos);
		if (qtd_nos == 0){
			break;
		}
		inicializa_matriz(matriz, qtd_nos);
		while (true){
			scanf("%d %d", &linha, &coluna);
			if(linha == 0){
				break;
			}
			--linha;
			--coluna;
			scanf("%s", word);	
			resultado = 0;
			mapea_string(matriz, word, linha, coluna);
		}
		floyd_warshall(matriz, qtd_nos);
		while(true){
			scanf("%d %d", &linha, &coluna);
			if (linha == 0){
				break;
			}
			--linha;
			--coluna;
			if(matriz[linha][coluna] == 0){
				printf("-\n");
			}
			else{
				for(i =0; i < (MAX_STRING-1); i++){
					if(matriz[linha][coluna] & (1<<i)){
						printf("%c",'a'+i);
					}
				}
				printf("\n");
			}
		}
		printf("\n");
	}
	return 0;
}
