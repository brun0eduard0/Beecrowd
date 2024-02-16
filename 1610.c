/******************************************
UNIVERSIDADE FEDERAL DO AMAZONAS
BRUNO EDUARDO GOMES BARRETO
DATA: 12/11/2013
ÚLTIMA MODIFICAÇÃO: 15/11/2013
Implementação da busca em profundidade
******************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int qtdvertices, qtdarestas, status;
int MAX_CITIES = 0, tempo = 0, MAX_STREETS = 0;
int** grafo;	/** -1 - nó original	0 - não adjacente	1 - adjacente	**/
char *cor;		/** b - branco, c - cinza , p - preto **/
int *pai;
int *distancia;
int** arestas;

void grafo_inicializa(){
	int i;
	grafo = (int**)calloc(MAX_CITIES, (sizeof(int*)));
	for(i = 0; i < MAX_CITIES; i++){
		grafo[i] = (int*)calloc(MAX_CITIES, sizeof(int));
	}
	//	remoção de autoloop em um mesmo vértice
	for(i = 0; i < MAX_CITIES; i++){
		grafo[i][i] = -2;
	}
	arestas = (int**)calloc(MAX_STREETS, (sizeof(int*)));
	for(i = 0; i < MAX_STREETS; i++){
		arestas[i] = (int*)calloc(2, sizeof(int));
		arestas[i][0] = -1;
		arestas[i][1] = -1;
	}
	//	int cor[MAX_CITIES];
	cor = (char*)calloc(MAX_CITIES, sizeof(char));
	//	int pai[MAX_CITIES];
	pai = (int*)calloc(MAX_CITIES, sizeof(int));
	//	int distancia[MAX_CITIES];
	distancia = (int*)calloc(MAX_CITIES, sizeof(int));
	for(i = 0; i < MAX_CITIES; i++){
		cor[i] = 'b';
		pai[i] = -1;
		distancia[i] = 0;
	}	
}

void dfs_visita(int atual){
	cor[atual] = 'c';
	tempo += 1;
	int i;
	for(i = 0; i < qtdvertices; i++){
		if(grafo[atual][i] == 1){
			if(cor[i] == 'b'){
				pai[i] = atual;
				distancia[atual] = tempo;
				dfs_visita(i);
			}
			else if(cor[i] == 'c'){
				status = 1;
			}
		}
	}
	cor[atual] = 'p';
	tempo += 1;
}

void dfs(){
	int i;
	status = 0;
	tempo = 0;
	for(i = 0; i < qtdvertices; i++){
		if(cor[i] == 'b'){
			dfs_visita(i);	
 		}
 		else if(cor[i] == 'c'){
			status = 1;
 		}
	}
}

void prepara_proxima_iteracao(){
	int i;
	for(i = 0; i < qtdvertices; i++){
		cor[i] = 'b';
		pai[i] = -1;
		distancia[i] = 0;
	}
	for(i = 0; i < qtdarestas; i++){
		grafo[ arestas[i][0] ][ arestas[i][1] ] = -1;
	}
}

int main(int argc, char *argv[]){
	int i, Va, Vb, casos;
	//identifica a quantidade de vértices do grafo lendo o primeiro dado
	MAX_CITIES = 10000;
	MAX_STREETS = 30000;
	//alocação dinâmica do grafo
	grafo_inicializa();
	scanf("%d", &casos);
	for(; casos > 0; casos--){
		scanf("%d %d", &qtdvertices, &qtdarestas);		
		//	leitura dos dados de entrada
		for(i = 0; i < qtdarestas; i++){
			scanf("%d %d", &Va, &Vb);
			Va--;
			Vb--;
			arestas[i][0] = Va;
			arestas[i][1] = Vb;
			grafo[Va][Vb] = 1;
		}
		dfs();
		prepara_proxima_iteracao();	
		if(status == 0){
			printf("NAO\n");
		}
		else{
			printf("SIM\n");
		}
	}
	return 0;
}
