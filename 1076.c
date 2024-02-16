#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int qtdVertices, qtdArestas, qtdTotalArestas;
int MAX_CITIES = 0;
int** grafo;	/**   -1 - nó original	0 - não adjacente	1 - adjacente	**/

void grafo_inicializa(){
	int i, j, k;
	grafo = (int**)calloc(qtdVertices, (sizeof(int*)));
	for(i = 0; i < qtdVertices; i++){
		grafo[i] = (int*)calloc(qtdVertices, sizeof(int));
		for(j = 0; j < qtdVertices; j++){
			grafo[i][j] = 0;
		}
 	}
}

void prepara_proxima_iteracao(){
	int i;
	for(i = 0; i < qtdVertices; i++){
		free(grafo[i]);
	}
	free(grafo);
	qtdVertices = 0;
}

void imprimeResultado(){
	printf("%d\n", qtdTotalArestas);
}

void solve(){
	int i, j;
	qtdTotalArestas = 0;
	//percorre a diagonal superior somando a existência das arestas e multiplica por 2
	for(i = 0; i < qtdVertices; i++){
		for(j = 0; j < qtdVertices; j++){
			if(grafo[i][j] == 1){
				qtdTotalArestas++;
			}
		}
	}
	qtdTotalArestas = qtdTotalArestas;
}

int main(int argc, char *argv[]){
	int i, casos, Va, Vb;
	int inicioPercurso = 0; //desnecessário para a solução proposta!!
	MAX_CITIES = 8;
	scanf("%d", &casos);
	for(; casos > 0; casos--){
		scanf("%d", &inicioPercurso);
		scanf("%d %d", &qtdVertices, &qtdArestas);
		//	grafo alocado dinâmicamente
		grafo_inicializa();
		//	leitura dos dados de entrada
		for(i = 0; i < qtdArestas; i++){
			scanf("%d %d", &Va, &Vb);
			grafo[Va][Vb] = 1;
			grafo[Vb][Va] = 1;
		}
		solve();
		imprimeResultado();
		prepara_proxima_iteracao();
	}
	return 0;
}
