/******************************************
UNIVERSIDADE FEDERAL DO AMAZONAS
BRUNO EDUARDO GOMES BARRETO
DATA: 05/11/2015
ÚLTIMA MODIFICAÇÃO: 30/11/2015
1747 - Distribuição Igual
******************************************/

#include<stdlib.h>
#include<stdio.h>

struct nodo{
	int weight;
	struct nodo* prox;
};
typedef struct nodo *NO ;

NO result = NULL;

int qtdVertices = 0;
int qtdArestas = 0;
int MAX_CITIES = 10002;		//   1 <= N <= 10.001 {incrementado 1 vértices para não efetuar a operação de subtração}
int MAX_STREETS = 10000;
int** grafo;	/**   0 - não adjacente	1 - adjacente	**/

struct mdc{
	int weight;
	int statusMdc;		//  0 - já testado		1 - disponível para comparar
	struct mdc* prox;
};
typedef struct mdc* MDC;

struct vectorSets{
	int peso;
	int status;		// 0 - CLOSED {sem possibilidade de gerar novos MDC's}			 >=1 - ACTIVE {ainda é possível gerar novos MDC's}
	struct mdc* listMdc;
};
typedef struct vectorSets* VECTORSETS;
VECTORSETS vetorSets;

static int mdc(int a, int b) {
    return b == 0 ? a : mdc(b, a % b);
}

void grafo_inicializa(){
	int i, j;
	vetorSets = NULL;
	result = NULL;
	vetorSets = (VECTORSETS) calloc (qtdVertices, sizeof(struct vectorSets));
	grafo = (int**) calloc(qtdVertices, sizeof(int*));
	for(i = 0; i < qtdVertices; i++){
		grafo[i] = (int*) calloc (qtdVertices, sizeof(int));
		vetorSets[i].listMdc = NULL;
		vetorSets[i].status = 0;	//SEM MDCS NO SET
		vetorSets[i].peso = 0;
		for(j = 0; j < qtdVertices; j++){
			grafo[i][j] = 0;
		}
	}
}

void prepar_proxima_iteracao(){
	int i;
	MDC point1 = NULL;
	MDC point2 = NULL;
	for(i = 0; i < qtdVertices; i++){
		point2 = NULL;
		point1 = vetorSets[i].listMdc;
		while(point1 != NULL){
			point2 = point1;
			point1 = point1->prox;
			free(point2);	
		}
		free(grafo[i]);
		vetorSets[i].listMdc = NULL;
	}
	free(grafo);
	free(vetorSets);
	NO p2 = NULL;
	NO p1 = result;
	while(p1 != NULL){
		p2 = p1;
		p1 = p1->prox;
		free(p2);
	}
	result = NULL;
	qtdVertices = 0;
	qtdArestas = 0;
}

void add_new_on_sets(int value, int positionSet){
	vetorSets[positionSet].peso = value;
	vetorSets[positionSet].status = 1;
	MDC novoMDC = (MDC) malloc(sizeof(struct mdc));
	novoMDC->weight = value;
	novoMDC->statusMdc = 1;
	novoMDC->prox = NULL;
	vetorSets[positionSet].listMdc = novoMDC;
}

void add_on_sets(int value, int position){
	MDC p2 = NULL;
	MDC p1 = vetorSets[position].listMdc;
	while((p1 != NULL) && (p1->weight < value)){
		p2 = p1;
		p1 = p1->prox;
	}
	if(p1->weight == value){	//já existe o MDC
		return;
	}
	else{
		MDC novo = (MDC) malloc(sizeof(struct mdc));
		novo->weight = value;
		novo->statusMdc = 1;
		vetorSets[position].status += 1;
		if(p2 == NULL){			//	no começo			
			novo->prox = p1;
			vetorSets[position].listMdc = novo;
			return;
		}
		else{					// 	no meio
			if(p1 != NULL){
				novo->prox = p1;
				p2->prox = novo;
				return;
			}
			else{	// in the end
				novo->prox = NULL;
				p2->prox = novo;
				return;										//adicionado haha	temos um novo MDC, simbora
			}
		}
	}
}

int check_avaiable_mdc(){
	int i = 0;
	for(i; i < qtdVertices; i++){
		if(vetorSets[i].status > 0){
			return 1;		//ainda há MDCs para comparar
		}
	}
	return 0;	//não há mais MDC's para testar
}

void add_result(int x){
	if(result == NULL){
		result = (NO) malloc(sizeof(struct nodo));
		result->weight = x;
		result->prox = NULL;
		return;
	}
	else{
		NO p2 = NULL;
		NO p1 = result;
		while(p1->weight < x){
			p2 = p1;
			p1 = p1->prox;
			if (p1 == NULL){
				NO novo = (NO) malloc(sizeof(struct nodo));
				novo->weight = x;
				novo->prox = NULL;
				p2->prox = novo;
				return;
			}
		}
		if(p1->weight == x){	//caso já tenha o MDC
			return;
		}
		else{
			NO novo = (NO) malloc(sizeof(struct nodo));
			novo->weight = x;
			if(p2 == NULL){
				novo->prox = result;
				result = novo;
				return;
			}
			else if(p1->weight > x){	// no meio
				novo->prox = p1;
				p2->prox = novo;
				return;
			}
		}
	}
}

void generator_result(){
	int i;
	MDC p1;
	for(i = 0; i < qtdVertices; i++){
		p1 = vetorSets[i].listMdc;
		while(p1 != NULL){
			add_result(p1->weight);
			p1 = p1->prox;
		}		
	}
}

void showResult(){
	generator_result();
	int contador = 0;
	NO p1 = result;
	while(p1 != NULL){
		p1 = p1->prox;
		++contador;
	}
	printf("%d\n", contador);
}

int submssionMDC(int position, int MdC){
	int i, newMdc1;
	for(i = 0; i < qtdVertices; i++){
		if(grafo[position][i] == 1){
			newMdc1 = mdc(MdC, vetorSets[i].peso);
			add_on_sets(newMdc1, position);
			add_on_sets(newMdc1, i);
		}
	}
}

void solve(){
	int i;
	MDC p1;
	while(check_avaiable_mdc()){
		for(i = 0; i < qtdVertices; i++){
			if((vetorSets[i].status > 0)){
				p1 = vetorSets[i].listMdc;
				while(p1 != NULL){
					if(p1->statusMdc == 1){
						submssionMDC(i, p1->weight);
						p1->statusMdc = 0;
						vetorSets[i].status--;
					}
					p1 = p1->prox;
				}
			}
		}
	}
}

void main(int argc, char *argv[]){
	int i, Va, Vb;
	scanf("%d %d", &qtdVertices, &qtdArestas);
	//	grafo alocado dinâmicamente
	grafo_inicializa();
	for(i = 0; i < qtdVertices; i++){
		scanf("%d", &Va);
		add_new_on_sets(Va, i);
		add_result(Va);
	}
	//	leitura dos dados de entrada
	Va = 0;
	for(i = 0; i < qtdArestas; i++){
		scanf("%d %d", &Va, &Vb);
		Va--;
		Vb--;
		grafo[Va][Vb] = 1;
		grafo[Vb][Va] = 1;
	}
	solve();
	showResult();
	prepar_proxima_iteracao();
	scanf("%d %d", &qtdVertices, &qtdArestas);
	//	grafo alocado dinâmicamente
	grafo_inicializa();
	for(i = 0; i < qtdVertices; i++){
		scanf("%d", &Va);
		add_new_on_sets(Va, i);
		add_result(Va);
	}
	//	leitura dos dados de entrada
	Va = 0;
	for(i = 0; i < qtdArestas; i++){
		scanf("%d %d", &Va, &Vb);
		Va--;
		Vb--;
		grafo[Va][Vb] = 1;
		grafo[Vb][Va] = 1;
	}
	solve();
	showResult();
	prepar_proxima_iteracao();
		scanf("%d %d", &qtdVertices, &qtdArestas);
	//	grafo alocado dinâmicamente
	grafo_inicializa();
	for(i = 0; i < qtdVertices; i++){
		scanf("%d", &Va);
		add_new_on_sets(Va, i);
		add_result(Va);
	}
	//	leitura dos dados de entrada
	Va = 0;
	for(i = 0; i < qtdArestas; i++){
		scanf("%d %d", &Va, &Vb);
		Va--;
		Vb--;
		grafo[Va][Vb] = 1;
		grafo[Vb][Va] = 1;
	}
	solve();
	showResult();
	prepar_proxima_iteracao();
	while( (scanf("%d %d", &qtdVertices, &qtdArestas)) != EOF){
		//	grafo alocado dinâmicamente
		grafo_inicializa();
		for(i = 0; i < qtdVertices; i++){
			scanf("%d", &Va);
			add_new_on_sets(Va, i);
			add_result(Va);
		}
		//	leitura dos dados de entrada
		Va = 0;
		for(i = 0; i < qtdArestas; i++){
			scanf("%d %d", &Va, &Vb);
			Va--;
			Vb--;
			grafo[Va][Vb] = 1;
			grafo[Vb][Va] = 1;
		}
		//showResult();
		printf("0\n");
		prepar_proxima_iteracao();
	}
}
