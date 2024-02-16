/******************************************
UNIVERSIDADE FEDERAL DO AMAZONAS
BRUNO EDUARDO GOMES BARRETO
DATA: 15/01/2015
ÚLTIMA MODIFICAÇÃO: 15/01/2015
verifica os componentes conexos

g++ -c componentes_conexos.cpp
g++ -o conexos componentes_conexos.o
./conexos
******************************************/

#include<iostream>
#include<cstdio>
#include<cstdlib>

using namespace std;

int MAX_CITIES = 0, MAX_STREETS = 0, tempo = 0, componentes, posicao;
int** grafo;	/** -1 - nó original	0 - não adjacente	1 - adjacente	**/
char *cor;		/** b - branco, c - cinza , p - preto **/
int *pai;
int *distancia;
int vetor[26];

void preparacao(){			//alocação dinâmica do grafo
	//identifica a quantidade de vértices do grafo lendo o primeiro dado
	scanf("%d %d", &MAX_CITIES, &MAX_STREETS);
	componentes = 0;
	if(MAX_CITIES == 0){
		return;
	}
	int i, j;
	grafo = (int**)calloc(MAX_CITIES, (sizeof(int*)));
	for(i = 0; i < MAX_CITIES; i++){
		grafo[i] = (int*)calloc(MAX_CITIES, sizeof(int));
	}
	//	preparar a matriz de adjacência para receber dados
	for(i = 0; i < MAX_CITIES; i++){
		for(j = 0; j < MAX_CITIES; j++){
			grafo[i][j] = 0;
		}
	}	
	char V;
	char U;
	int posicaoV, contVertices;
	int posicaoU;
	contVertices = 0;
	//	leitura dos dados de entrada	
	while(contVertices < MAX_STREETS){
		scanf(" %c %c", &U, &V);
		posicaoU = ((int)U) - (int)'a';
		posicaoV = ((int)V) - (int)'a';
		grafo[posicaoU][posicaoV] = 1;
		grafo[posicaoV][posicaoU] = 1;
		contVertices++;
	}

	/*	inicialização do vetor*/	
	for(i = 0; i < 26; i++){
		vetor[i] = ' ';
	}
	
	//	int cor[MAX_CITIES];
	cor = (char*)calloc(MAX_CITIES, sizeof(char));
	for(i = 0; i < MAX_CITIES; i++){
		cor[i] = 'b';
	}
	
	//	int pai[MAX_CITIES];
	pai = (int*)calloc(MAX_CITIES, sizeof(int));
	for(i = 0; i < MAX_CITIES; i++){
		pai[i] = -1;
	}

	//	int distancia[MAX_CITIES];
	distancia = (int*)calloc(MAX_CITIES, sizeof(int));
	for(i = 0; i < MAX_CITIES; i++){
		distancia[i] = -1;
	}
}

void ordena(){
	int i, j;
	char temp;
	for(i = 0; i <= posicao; i++){
		for(j = i + 1; j <= posicao; j++){
			if(vetor[i] > vetor[j]){
				temp = vetor[i];
				vetor[i] = vetor[j];
				vetor[j] = temp;
			}
		}
	}	
}

void imprime(){
	int i = 0;
	while(i <= posicao){
		printf("%c,",vetor[i]);
		i++;
	}
	printf("\n");
}

void libera_vetor(){
	int i = 0;
	while(i <= posicao){
		vetor[i] = ' ';
		i++;
	}
}

void dfs_visita(int atual){
	cor[atual] = 'c';
	tempo += 1;
	int i;
	for(i = 0; i < MAX_CITIES; i++){
		if(grafo[atual][i] > 0){
			if((cor[i] == 'b')){
				pai[i] = atual;
				distancia[atual] = tempo;
				char temp;
				temp = (char)(i + 'a');
				//inserção no vetor
				posicao++;
				vetor[posicao] = temp;
				dfs_visita(i);
			}
		}
	}
	cor[atual] = 'p';
	tempo += 1;
}

void dfs(){
	int i;
	for(i = 0; i < MAX_CITIES; i++){
		cor[i] = 'b';
		pai[i] = -1;
		distancia[i] = 0;
	}
	tempo = 0;
	char temp;
	for(i = 0; i < MAX_CITIES; i++){
		posicao = -1;
		if(cor[i] == 'b'){
			componentes++;
			temp = (char)(i + 'a');
			//inserção no vetor
			posicao++;
			vetor[posicao] = temp;
			dfs_visita(i);
			//ordena vetor
			ordena();
			//imprime vetor
			imprime();
			//libera vetor
			libera_vetor();
		}
	}
}

void elimina_grafo(){
	int i;
	for(i = 0; i < MAX_CITIES; i++){
		free(grafo[i]);
	}
	free(grafo);
	free(cor);
	free(pai);
	free(distancia);
	distancia = NULL;
	pai = NULL;
	cor = NULL;
	grafo = NULL;
}

int main(){
	int i, j, casos;
	scanf("%d", &casos);
	int contador_casos = 0;
	while(contador_casos < casos){
		preparacao();
		contador_casos++;
		//printf("Case #%d:\n", contador_casos);
		dfs();
		//printf("%d connected components\n\n", componentes);
		elimina_grafo();
		if(componentes > 1){
			printf("SIM\n");
		}
		else{
			printf("NAO\n");
		}
	}
	return 0;
}
