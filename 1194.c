/******************************************
UNIVERSIDADE FEDERAL DO AMAZONAS
BRUNO EDUARDO GOMES BARRETO
DATA: 24/09/2015
ÚLTIMA MODIFICAÇÃO: 29/09/2015
Uso do caminhamento em preordem e emordem para
obter a arvore e gerar o caminhamento posordem
******************************************/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct nodo{
	char dado;
	int posicao;
	struct nodo* sae;
	struct nodo* sad;
};
typedef struct nodo* NO ;

struct nodo2{
	char dado;
	int posicaoEmOrdem;
};
typedef struct nodo2* NO2;

struct Raiz{
	int qtd_nos;
	NO raiz;
};
typedef struct Raiz *RAIZ;

RAIZ Arvore_cria(){
	RAIZ novo = (RAIZ) malloc(sizeof(struct Raiz));
	novo->qtd_nos = 0;
	novo->raiz = NULL;
	return novo;
}

void arv_raiz(RAIZ arv, NO2 preordem, NO lista, int *cont, int tam){
	arv->raiz = &lista[preordem[*cont].posicaoEmOrdem];
	(*cont)++;
}

NO2 preOrdem_cria(){
	NO2 preordem;
	preordem = (NO2) calloc (52, sizeof(struct nodo2));	//vetor com os nós EM ORDEM
	int i = 0;
	while(i < 52){
		preordem[i].dado = ' ';			// caracter vazio...
		preordem[i].posicaoEmOrdem = -1;	// -1 indica que essa posição ainda não foi utilizada....
		i++;
	}
	return preordem;
}

int findPositionInOrder(char pre, char emordem[], int tam){
	int i = 0;
	if(pre == emordem[i]){
		return i;
	}
	else{
		for(i; (pre != emordem[i]) && (i < tam); i++){
		}
		return i;
	}
}

void preOrdem_add(NO2 preordem, char pre[], char in[], int tam){
	int i;
	char preor[52], emor[52];
	for(i = 0; i < tam; i++){
		preor[i] = pre[i];
		emor[i] = in[i];
	}
	i = 0;
	while(i < tam){
		preordem[i].dado = pre[i];
		preordem[i].posicaoEmOrdem = findPositionInOrder(preor[i], emor, tam);
		i++;
	}
}

void preOrdem_esvaziar(NO2 preordem, int tam){
	int i = 0;
	while(i < tam){
		preordem[i].dado = ' ';			//caracter vazio...
		preordem[i].posicaoEmOrdem = -1;	// 0 indica que essa posição ainda não foi utilizada....
		i++;
	}
}

NO emOrdem_cria(){
	int cont;
	NO lista;
	lista = (NO)calloc(52, sizeof(struct nodo));	//vetor com os nós EM ORDEM
	for(cont = 0; cont < 52; cont++){	
		lista[cont].dado = ' ';
		lista[cont].posicao = -1;
		lista[cont].sae = NULL;
		lista[cont].sad = NULL;
	}
	return lista;
}


void emOrdem_add(NO lista, char b[], int tam){
	int cont;
	for(cont = 0; cont < tam; cont++){	
		lista[cont].dado = b[cont];
		lista[cont].posicao = cont;
		lista[cont].sae = NULL;
		lista[cont].sad = NULL;
	}
}

void emOrdem_esvaziar(NO lista, int tam){
	int cont = 0;
	for(cont = 0; cont < tam; cont++){	
		lista[cont].dado = ' ';
		lista[cont].posicao = -1;
		lista[cont].sae = NULL;
		lista[cont].sad = NULL;
	}
}

void arv_visita(struct nodo** galho, NO lista, NO2 preordem, int tam){
	if(*galho == NULL){
		*galho = &(lista[preordem->posicaoEmOrdem]);
	}
	else{
		if(preordem->posicaoEmOrdem < (*galho)->posicao){
			arv_visita(&(*galho)->sae, lista, preordem, tam);
			return;
		}
		else{
			arv_visita(&(*galho)->sad, lista, preordem, tam);
			return;
		}
	}
}

void arv_add(RAIZ raiz, NO lista, NO2 preordem, int *posicao, int tam){
	//começa a partir do segundo valor do preordem
	for(; (*posicao) < tam; (*posicao)++){
		arv_visita(&(raiz)->raiz, lista, &(preordem[*posicao]), tam);
	}
}

void posOrdem_imprime(NO arv){
	if(arv == NULL){
		return;
	}
	else{
		posOrdem_imprime(arv->sae);
		posOrdem_imprime(arv->sad);
		printf("%c", arv->dado);
	}
}

int main(){
	int n, tam, cont;
	scanf("%d", &n);
	char str[100], a[51], b[51];
	RAIZ arv = Arvore_cria();
	NO lista = emOrdem_cria();
	NO2 preordem = preOrdem_cria();
	while(n > 0){
		cont = 0;
		fscanf(stdin,"%d %s %s", &tam, a, b);
		preOrdem_add(preordem, a, b, tam);
		emOrdem_add(lista, b, tam);
		arv_raiz(arv, preordem, lista, &cont, tam);
		arv_add(arv, lista, preordem, &cont, tam);	//talvez gere problema com a raiz
		//imprime pos ordem
		posOrdem_imprime(arv->raiz);
		emOrdem_esvaziar(lista, tam);
		printf("\n");
		n--;
	}
	return 0;
}
