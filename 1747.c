/******************************************
UNIVERSIDADE FEDERAL DO AMAZONAS
BRUNO EDUARDO GOMES BARRETO
DATA: 05/11/2015
ÚLTIMA MODIFICAÇÃO: 05/11/2015
1747 - Distribuição Igual
solve - armazena os MDC's combinando os vertices {armazenadas na matriz diagonalsuperior}
e soma o total de MDC's diferentes obtidos com os da diagonal principal {equivalentes}
******************************************/

#include<stdio.h>
#include<stdlib.h>

struct vertice{
	int weight;
	struct vertice* prox;
};
typedef struct vertice* VERTICE ;

int qtdVertices, qtdArestas, qtdTotalCriancas;
int MAX_CITIES = 10002;		//   1 <= N <= 10.001 {incrementado 1 vértices para não efetuar a operação de subtração}
int MAX_STREETS = 10000;
int MAX_RESULT = 100;
int** grafo;	/**   0 - não adjacente	1 - adjacente	**/
int result[100];	//para armazenas os MDC's diferentes obtidos para posterior contagem

void grafo_inicializa(){
	int i, j, k;
	grafo = (int**)calloc(qtdVertices, (sizeof(int*)));
	for(i = 0; i < qtdVertices; i++){
		grafo[i] = (int*)calloc(qtdVertices, sizeof(int));
		for(j = 0; j < qtdVertices; j++){
			grafo[i][j] = 0;
		}
 	}
 	for(i = 0; i < MAX_RESULT; i++){
 		result[i] = -1;	//não é um valor válido para comparar
 	}
 	qtdTotalCriancas = 0;
}

void prepara_proxima_iteracao(){
	int i;
	for(i = 0; i < qtdVertices; i++){
		free(grafo[i]);
	}
	free(grafo);
	qtdVertices = 0;
	qtdArestas = 0;
}

void imprimeResultado(){
	printf("%d\n", qtdTotalCriancas);
}

void addResult(int x){
	int i, flag = 0;
	for(i = 0; i < qtdTotalCriancas; i++){
		if(result[i] == x){	//valor já existe no vetor de soluções
			flag = 1;
		}
	}
	if(flag == 0){		//nova solução
		result[qtdTotalCriancas] = x;
		qtdTotalCriancas++;
	}
}

int mdc(int u, int v){
	int t, k;
	u = u < 0 ? -u : u; /* abs(u) */
	v = v < 0 ? -v : v; 
	if (u < v) {
		t = u;
		u = v;
		v = t;
	}
	if (v == 0){
		return u;
	}
	k = 1;
	while (u & 1 == 0 && v & 1 == 0) { /* u, v - even */
		u >>= 1; v >>= 1;
		k <<= 1;
	}
	t = (u & 1) ? -v : u;
	while (t) {
		while (t & 1 == 0){
			 t >>= 1;
		}
		if (t > 0){
			u = t;
		}
		else{
			v = -t;
		}
		t = u - v;
	}
	return u * k;        
}

/*int mdc(int a, int b) {
    return b == 0 ? a : mdc(b, a % b);
}*/
/*
int mdc(int x, int y){
	if(y == 0){
		return x;
	}
	else{
		int resto;
		resto = x % y;
		while(resto != 0){
			x = y;
			y = resto;
			resto = x % y;
		}
		return y;
	}
}*/

void solve(){
	int i, j, temp;
	//percorre a diagonal superior somando a existência das arestas e multiplica por 2
	for(i = 0; i < qtdVertices; i++){
		for(j = (i+1); j < qtdVertices; j++){
			if(grafo[i][j] == 1){
				temp = mdc(grafo[i][i], grafo[j][j]);
				addResult(temp);
			}
		}
	}
}

int main(int argc, char *argv[]){
	int i, Va, Vb;
	while(scanf("%d %d", &qtdVertices, &qtdArestas) != EOF){
		//	grafo alocado dinâmicamente
		grafo_inicializa();
		for(i = 0; i < qtdVertices; i++){
			scanf("%d", &grafo[i][i]);
			addResult(grafo[i][i]);
		}
		//	leitura dos dados de entrada
		for(i = 0; i < qtdArestas; i++){
			scanf("%d %d", &Va, &Vb);
			--Va;
			--Vb;
			grafo[Va][Vb] = 1;
			grafo[Vb][Va] = 1;
		}
		solve();
		imprimeResultado();
		prepara_proxima_iteracao();
	}
	return 0;
}
