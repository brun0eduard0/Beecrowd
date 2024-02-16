/******************************************
UNIVERSIDADE FEDERAL DO AMAZONAS
BRUNO EDUARDO GOMES BARRETO
DATA: 05/11/2015
ÚLTIMA MODIFICAÇÃO: 05/11/2015
1747 - Distribuição Igual
solve - armazena os MDC's combinando os NOs {armazenadas na matriz diagonalsuperior}
e soma o total de MDC's diferentes obtidos com os da diagonal principal {equivalentes}
g++ -c 1747.cpp
g++ -o 1747 1747.o
./1747

******************************************/

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

#include <cstdio>
#include <cstdlib>
//#include <bits/stdc++.h>
//#include <set>

struct nodo{
	int weight;
	struct nodo* prox;
};
typedef struct nodo *NO ;

//set<int> s;

NO lista;

int qtdTotal = 0;

int qtdVertices, qtdArestas;
int MAX_CITIES = 10002;		//   1 <= N <= 10.001 {incrementado 1 vértices para não efetuar a operação de subtração}
int MAX_STREETS = 10000;
int MAX_RESULT = 100;
int** grafo;	/**   0 - não adjacente	1 - adjacente	**/
int result[100];	//para armazenas os MDC's diferentes obtidos para posterior contagem

void grafo_inicializa(){
	lista = (NO) calloc((++qtdVertices), (sizeof(struct nodo)));
	qtdTotal = 0;
}

void prepara_proxima_iteracao(){
	int i;
	NO p;
	for(i = 0; i < qtdVertices; i++){
		p = lista[i].prox;
		while(p != NULL){
			lista[i].prox = p->prox;
			free(p);
			p = lista[i].prox;
		}
	}
	free(lista);
	qtdVertices = 0;
	qtdArestas = 0;
}

void imprimeResultado(){
	printf("%d\n", (qtdTotal));
}

void addResult(int x){
	int i;
	for(i = 0; i < qtdTotal; i++){
		if(result[i] == x){	//valor já existe no vetor de soluções
			return;
		}
	}
	result[qtdTotal] = x;
	qtdTotal++;
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

void solve(){
	int i, temp;
	NO p;
	
	//percorre a diagonal superior somando a existência das arestas e multiplica por 2
	for(i = 1; i < qtdVertices; i++){
		p = lista[i].prox;
		while(p != NULL){
			temp = mdc(lista[i].weight, p->weight);
			addResult(temp);
			p = p->prox;
		}
	}
}

int main(int argc, char *argv[]){
	int i, Va, Vb;
	while(scanf("%d %d", &qtdVertices, &qtdArestas)){
		//	grafo alocado dinâmicamente
		grafo_inicializa();
		for(i = 1; i < qtdVertices; i++){
			scanf("%d", &(lista[i].weight));
			addResult(lista[i].weight);
			lista[i].prox = NULL;
		}
		//	leitura dos dados de entrada
		for(i = 0; i < qtdArestas; i++){
			scanf("%d %d", &Va, &Vb);
			NO p = (NO) malloc(sizeof(struct nodo));
			if(lista[Va].weight < lista[Vb].weight){
				p->weight = (lista[Va].weight);
				p->prox = lista[Vb].prox;
				lista[Vb].prox = p;
			}
			else{
				p->weight = (lista[Vb].weight);
				p->prox = lista[Va].prox;
				lista[Va].prox = p;}
			
		}
		solve();
		imprimeResultado();
		prepara_proxima_iteracao();
	}
	return 0;
}
