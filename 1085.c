/******************************************
UNIVERSIDADE FEDERAL DO AMAZONAS
BRUNO EDUARDO GOMES BARRETO
DATA: 11/11/2015
ÚLTIMA MODIFICAÇÃO: 26/11/2015
1085 - Babel
******************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct entrada{
	char origem[51];
	char destino[51];
	char palavra[51];
};
typedef struct entrada *ENTRADA;

struct nodo{
	int position;	
	char idioma[51];
	struct nodo* prox;
};
typedef struct nodo *NO;

struct word{
	char letra;
	int tam;
	struct word* prox;
};
typedef struct word* WORD;

struct graph{
	WORD words;
};
typedef struct graph* GRAPH;

ENTRADA lista = NULL;
NO conjVertices = NULL;
NO listaVertices = NULL;
int verticeOrigem, verticeDestino;
int qtdVertices, qtdArestas;
int MAX_CITIES = 2000;		//   1 <= N <= 10.001 {incrementado 1 vértices para não efetuar a operação de subtração}
int MAX_STREETS = 2000;
int distanciaMaxima;

GRAPH* grafo;	/**   NULL - não adjacente	com ponteiro - adjacente	**/ 

int* pai;					//PÍ   identifica o vertice de onde se origina uma conexão até v de maneira a formar um caminho mínimo
int* distancia;			//d[v] vetor de distancia
NO Q = NULL;				//conjunto cujos vertices ainda não contém o custo do menor caminho d[v] determinado
char *charSequence;

void idioma_inicializa();
void lista_inicializa();
void add_idioma(char x[]);
void verifica_quantidade_vertices();
void aloca_matrizes_vetores();
void determina_origem_e_cria_lista(char *origem, char *destino);
int find_position(char *v);
int insere_grafo(WORD aresta, int a, int b);
void insere_na_matriz();
void inicializa_vetor_de_caracteres();
WORD extract_min_edge(int a, int b);
NO extract_min();
void print_matriz();
void dijkstra();
void preparProximaIteracao();
void imprime_resultado(char *str1, char* str2);
void print_chars();
void show_vetor_de_caracteres();
void print_distance();

int main(int argc, char *argv[]){
	int i;
	char entradaOrigemDestino[105];
	char ORIGEM[50], DESTINO[50];
	char Va[50], Vb[50], Wab[50];
	char entradaDados[154];	//talvez 154
	while(scanf("%d", &qtdArestas) && (qtdArestas != 0)){
		//	grafo alocado dinâmicamente
		scanf(" %[^\n]s", entradaOrigemDestino);
		sscanf(entradaOrigemDestino, "%s %s", ORIGEM , DESTINO);
		lista_inicializa();
		//	leitura dos dados de entrada
		for(i = 0; i < qtdArestas; i++){
			scanf(" %[^\n]s", entradaDados);
			sscanf(entradaDados, "%s %s %s", lista[i].origem , lista[i].destino, lista[i].palavra);
 //			printf("origem-%s\tdestino-%s\tword-%s\n", lista[i].origem , lista[i].destino, lista[i].palavra);
			add_idioma(lista[i].origem);
			add_idioma(lista[i].destino);
		}
		verifica_quantidade_vertices();
	//	printf("vertices - %d\n", qtdVertices);
		aloca_matrizes_vetores();
		determina_origem_e_cria_lista(ORIGEM, DESTINO);
	//	print_matriz();
		insere_na_matriz();
	//	print_chars();
	//	print_matriz();
		inicializa_vetor_de_caracteres();
		dijkstra();
	//	print_matriz();
		imprime_resultado(ORIGEM, DESTINO);
		preparProximaIteracao();
	}
	return 0;
}

void idioma_inicializa(){
	conjVertices = NULL;
}

void print_chars(){
	int i;
	for(i = 0; i < qtdVertices; i++){
		printf("%s ", Q[i].idioma);
	}
	printf("\n");
}

void lista_inicializa(){
	lista = NULL;
	int i = 0;
	lista = (ENTRADA) calloc (qtdArestas, (sizeof(struct entrada)));
	for(i; i < qtdArestas; i++){
		memset(lista[i].origem, '\0', sizeof(lista[i].origem));
	}
	idioma_inicializa();
}

void add_idioma(char x[]){	//lista encadeada dos idiomas
	if(conjVertices == NULL){
		conjVertices = (NO) malloc(sizeof(struct nodo));
		strcpy(conjVertices->idioma, x);
		conjVertices->prox = NULL;
		return;
	}
	else{
		NO p1 = conjVertices;
		while(p1 != NULL){
			if(!(strcmp(p1->idioma , x))){
				return;
			}
			p1 = p1->prox;

		}
		if(p1 == NULL){
			p1 = (NO) malloc(sizeof(struct nodo));
			strcpy(p1->idioma, x);
			p1->prox = conjVertices;
			conjVertices = p1;
			return;
		}
	}
}

void verifica_quantidade_vertices(){
	qtdVertices = 0;
	NO p1 = conjVertices;
	while(p1 != NULL){
		qtdVertices++;
		p1 = p1->prox;
	}
}

void aloca_matrizes_vetores(){
	int i, j;
	//ALOCA GRAFO (MATRIZ DE PONTEIROS)
	grafo = (GRAPH*) calloc(qtdVertices, (sizeof(GRAPH)));
	for(i = 0; i < qtdVertices; i++){
		grafo[i] = (GRAPH) calloc (qtdVertices, sizeof(struct graph));
		for(j = 0; j < qtdVertices; j++){
			grafo[i][j].words = NULL;
		}
	}	
	//ALOCA VETOR DE DISTÂNCIAS
	distancia = (int*)calloc(qtdVertices, (sizeof(int)));
	j = qtdVertices * 50;	//100.000	é o maximo
	distanciaMaxima = j;
	for(i = 0; i < qtdVertices; i++){
		distancia[i] = j;
	}
	//ALOCA VETOR DOS VERTICES DE ORIGEM
	pai = (int*)calloc(qtdVertices, (sizeof(int)));
	for(i = 0; i < qtdVertices; i++){
		pai[i] = -1;
	}
	//ALOCA CONJUNTO DE VERTICES Q;
	Q = conjVertices;
	charSequence = (char*)calloc(qtdVertices, sizeof(char));
}

void inicializa_vetor_de_caracteres(){
	int i = 0;
	for(i; i < qtdVertices; i++){
		charSequence[i] = 'A';
	}
	charSequence[verticeOrigem] = '#';
}

void swap(){
	int temp = verticeOrigem;
	verticeOrigem = verticeDestino;
	verticeDestino = temp;
}

void recria_charSequence_distancia_pai(){
	int i = 0;
	swap();
	for(i; i < qtdVertices; i++){
		distancia[i] = distanciaMaxima;
		pai[i] = -1;
		charSequence[i] = 'A';
	}
	distancia[verticeOrigem] = 0;
	charSequence[verticeOrigem] = '#';
}

void recria_lista(){
	//conjVertice 	== VETOR
	//Q			== lista dos vértices
	//alteração de lista para vetor com as strings de vertices
	NO p1 = listaVertices;	//segunda lista
	//novo vetor das strings dos países
	NO p2 = conjVertices;
	NO p3 = NULL;
	NO p4 = NULL;
	int i = 0;
	// pega os idiomas da lista conjVertices e insere estes idiomas no novo conjVertices chamado p2
	if(!(strcmp(p2[i].idioma, p1->idioma))){		//se a os dados são iguais....   
		p4 = &p2[i];					//refaço a o vetor e lista para preparar uma nova iteração do dijkstra
		p3 = p1;
		p1 = p1->prox;
		free(p3);
		for(i = 1; i < qtdVertices; i++){
			if(!(strcmp(p2[i].idioma, p1->idioma))){
				p4->prox = &p2[i];
				p4 = &p2[i];
				p3 = p1;
				p1 = p1->prox;
				free(p3);
			}
			
		}
		p4->prox = NULL;
	}
	recria_charSequence_distancia_pai();
	// Vetor P2 ESTÁ COM TODOS OS DADOS
	// A SEQUENCIA DA LISTA ENCADEADA ESTÁ PRONTA
	//Q 			- fila de prioridade dos vertices
	//conjVertices	- vetor contendo os vertices sequencialmente para eventuais comparações com a string da posição 
	Q = &p2[0];
	conjVertices = p2;
}

void determina_origem_e_cria_lista(char *origem, char *destino){
	verticeOrigem = 0;
	verticeDestino = 0;
	NO p1 = conjVertices;
	while(p1 != NULL){
		if(!strcmp(p1->idioma, origem)){
			distancia[verticeOrigem] = 0;	//determina o vertice de origem
			break;
		}
		verticeOrigem++;
		p1 = p1->prox;
	}
	p1 = conjVertices;
	while(p1 != NULL){
		if(!strcmp(p1->idioma, destino)){
			break;
		}
		verticeDestino++;
		p1 = p1->prox;
	}
	// ORIGEM E DESTINO DEFINIDOS - verticeOrigem   &&     verticeDestino
	if(distancia[verticeOrigem] != 0){
		printf("\nOCORREU O IMPOSSIVEL, tem algo errado!\n");
	}
	//alteração de lista para vetor com as strings de vertices
	p1 = conjVertices;
	listaVertices = conjVertices;	//segunda lista
	//novo vetor das strings dos países
	NO p2 = (NO) calloc(qtdVertices, sizeof(struct nodo));
	NO p3 = NULL;
	NO p4 = NULL;
	int i = 0;
	strcpy(p2[i].idioma, p1->idioma);
	p2[i].position = i;
	p4 = &p2[i];
	p3 = p1;
	p1 = p1->prox;
	for(i = 1; i < qtdVertices; i++){
		strcpy(p2[i].idioma, p1->idioma);
		p2[i].position = i;
		p4->prox = &p2[i];
		p4 = &p2[i];
		p3 = p1;
		p1 = p1->prox;
	}
	p4->prox = NULL;
	//Q 			- fila de prioridade dos vertices
	//conjVertices	- vetor contendo os vertices sequencialmente para eventuais comparações com a string da posição 
	Q = &p2[0];
	conjVertices = p2;
	//atualiza a fila de prioridade colocando o vértice de origem no início com
}

//tenho o vetor dos dados de entrada
//e tenho a matriz de ponteiros...
//e o vetor de strings
//como inserir?

int find_position(char *v){
	int i = 0;
	while((strcmp(v, conjVertices[i].idioma)) != 0){
		i++;
	}
	//é igual a algum
	return conjVertices[i].position;
}

int insere_grafo(WORD aresta, int a, int b){
	if(grafo[a][b].words == NULL){
		grafo[a][b].words = aresta;
		grafo[a][b].words->prox = NULL;
		return 1;
	}
	else{	//já há outra aresta nesta posiçao... então eu adiciono como fila de prioridade... :)
		if(aresta->tam < grafo[a][b].words->tam){
			aresta->prox = grafo[a][b].words;
			grafo[a][b].words = aresta;
			//printf("inseriu :)\tini->%c   next->%c\n", aresta->letra, aresta->prox->letra);
			return 1;
		}
		else{
			WORD p1 = grafo[a][b].words;
			WORD p2 = NULL;
			while(p1->tam < aresta->tam){
				p2 = p1;
				p1 = p1->prox;
				if(p1 == NULL){
					aresta->prox = NULL;
					p2->prox = aresta;
					return 1;
				}
			}
			//inserção no inicio    com tamanho igual
			if(p1->tam == aresta->tam){
				if(aresta->letra == p1->letra){
					return 0;
				}
				else{
					if(p2 == NULL){
						aresta->prox = grafo[a][b].words;
						grafo[a][b].words = aresta;
						return 1;
					}
					else{
						p2->prox = aresta;
						aresta->prox = grafo[a][b].words->prox;
						return 1;
					}
				}
			}
			else{
				if(p1->tam > aresta->tam){
					aresta->prox = p1;
					p2->prox = aresta;
					return 1;
				}
			}
		}
	}
}

void insere_na_matriz(){
	int i, a, b, flag;
	for(i = 0; i < qtdArestas; i++){
		a = find_position(lista[i].origem);
		b = find_position(lista[i].destino);
		WORD arestaAB = (WORD) malloc(sizeof(struct word));		
		arestaAB->letra = lista[i].palavra[0];
		arestaAB->tam = strlen(lista[i].palavra);
		//printf("aresta->%c   tamanho->%d\n", arestaAB->letra, arestaAB->tam);
		flag = insere_grafo(arestaAB, a, b);
		if(flag){
			WORD arestaBA = (WORD) malloc(sizeof(struct word));
			arestaBA->letra = lista[i].palavra[0];
			arestaBA->tam = arestaAB->tam;
			flag = insere_grafo(arestaBA, b, a);
		}
		flag = 0;
	}
//	exit(1);
}

void show_vetor_de_caracteres(){
	int i = 0;
	for(i; i < qtdVertices; i++){
		printf("%c, ", charSequence[i]);
	}
	printf("\n\n");
}

WORD extract_min_edge(int a, int b){
	WORD nova;
	WORD p1 = grafo[a][b].words;
	WORD p2 = NULL;
	while(p1->letra == charSequence[a]){
		p2 = p1;
		p1 = p1->prox;
		if(p1 == NULL){	//só há aresta cuja sequencia de caracteres não é interessante
			// o que fazer quando as arestas não batem?
			nova = NULL;
			return nova;
		}
	}
	if(p1 != NULL){
		nova = grafo[a][b].words;
		return nova;
	}
	/*
	while(p1->letra == charSequence[a]){
		p2 = p1;
		p1 = p1->prox;
		if(p1 == NULL){
			nova = NULL;
			return nova;
		}
	}
	if(p1 != NULL){
		nova = grafo[a][b].words;
		return nova;
	}*/
}

void remove_edge(){
	
}

NO extract_min(){
	NO min = Q;
	NO p1 = Q;
	NO p2 = NULL;
	NO predecessor = NULL;
	while(p1 != NULL){
		if(distancia[min->position] > distancia[p1->position]){
			predecessor = p2;
			min = p1;
		}
		p2 = p1;
		p1 = p1->prox;
	}
	//primeiro
	if(predecessor == NULL){
		Q = Q->prox;
		min->prox = NULL;
		return min;
	}
	else{
		//meio
		predecessor->prox = min->prox;
		min->prox = NULL;
		return min;
	}
}

void print_matriz(){
	int i, j, cont;
	WORD p;
	for(i = 0; i < qtdVertices; i++){
		for(j = 0; j < qtdVertices; j++){
			if(grafo[i][j].words == NULL){
				printf("0 ");
			}
			else{
				p = grafo[i][j].words;
				cont = 0;
				while(p != NULL){
					p = p->prox;
					cont++;
				}
				printf("%d ", cont);
			}
		}
		printf("\n");
	}
	printf("\n");
}

void print_distance(){
	int i;
	printf("Distancias\n");
	for(i = 0; i < qtdVertices; i++){
		printf("%d ", distancia[i]);
	}
	printf("\n");
}

struct oldEdge{
	int origem;
	int destino;
	WORD edge;
};
typedef struct oldEdge OLDEDGE;

void dijkstra(){
	int i = 0, oldOrigin = -1;
	WORD aresta = NULL;
	NO u = NULL;
	OLDEDGE antigaAresta = NULL;
	while(Q != NULL){
		u = extract_min();
		//printf("minimo->%s\n",u->idioma);
		//print_matriz();
		for(i = 0; i < qtdVertices; i++){
			if(grafo[u->position][i].words != NULL){
				//para cada vértice V adjacente a U
				aresta = extract_min_edge(u->position, i);
				if((aresta != NULL) && (distancia[i] > (distancia[u->position] + aresta->tam))){
					//faço o relaxamento entre U e V
					charSequence[i] = aresta->letra;
					//show_vetor_de_caracteres();
					distancia[i] = distancia[u->position] + aresta->tam;
					pai[i] = u->position;
				}
				//ainda há arestas ligando os dois vértices, portanto o vértice anterior deve voltar ao final da fila para uma eventual nova tentativa
			}
		}
	}
}

void preparProximaIteracao(){
	lista = NULL;
	int i, j;
	free(lista);
	WORD p1;
	WORD p2;
	for(i = 0; i < qtdVertices; i++){
		for(j = 0; j < qtdVertices; j++){
			if(grafo[i][j].words != NULL){
				p1 = grafo[i][j].words;
				p2 = NULL;
				while(p1 != NULL){
					p2 = p1;
					p1 = p1->prox;
					free(p2);
				}
			}
		}
		free(grafo[i]);
	}
	free(distancia);
	free(pai);
	free(grafo);
	free(charSequence);
	Q = NULL;
	qtdVertices = 0;
	qtdArestas = 0;
	verticeOrigem = 0;
	verticeDestino = 0;
}

void imprime_resultado(char *str1, char* str2){
	/*int i;
	for(i = 0; i < qtdVertices; i++){
		printf("distancia até %d - %d\n", i, distancia[i]);
	}
	int i;
	printf("imprimindo sequencia de caracterres\n");
	for(i = 0; i < qtdVertices; i++){
		printf("%c ", charSequence[i]);
	}
	printf("\n");*/
	if((distancia[verticeDestino] == 0) || (distancia[verticeDestino] == distanciaMaxima)){
		recria_charSequence_distancia_pai();
		dijkstra();
		if((distancia[verticeDestino] == 0) || (distancia[verticeDestino] == distanciaMaxima)){
			printf("distancia-> %d\t\timpossivel\n", distancia[verticeOrigem]);
		}
		else{
		printf("%d\n",distancia[verticeDestino]);
	}
	}
	else{
		printf("%d\n",distancia[verticeDestino]);
	}
}

