#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

//ingredientes
#define TABACO 0
#define PAPEL 1
#define FOSFORO 2

//estados
#define ESPERANDO 10
#define PREPARANDO1 11
#define PREPARANDO2 12
#define FUMANDO1 13
#define FUMANDO2 14

//sleeps
#define SLEEP_AGENTE 2
#define SLEEP_PREPARA 3
#define SLEEP_FUMA 3
#define SLEEP_ANIMA 2
#define SLEEP_FRASE 10

pthread_mutex_t global_mutex;

int n_fumantes = -1;
int n_ingr = -1;
int ingrediente = -1;
int** ingrFumante;
int* estadoFumante;
int* contaIngr;
char* frase;

// evolucao do programa em texto. apenas para debug
/*
void evolucao_texto() {
	int i, j;
		printf("Agente oferecendo: %d\n", ingrediente);
		for(j=0;j<n_fumantes;j++){
			printf("Fumante%d: %d\nTem: ", j, estadoFumante[j]);
			for(i=0;i<3;i++){
				if(ingrFumante[j][i] > 0){
					printf("%d ", i);
				}
			}
			printf("\n");
		}
}
*/

// animacao dos fumantes
void printFumante(int id) {
	if(estadoFumante[id] == FUMANDO1) {
		printf("---FUMANDO--------------------\n\n");
		printf("  <<>>>>>>>\n");
		printf("  |       |\n");
		printf(" <| -   - |>  s\n");
		printf("  |   ^   |  S\n");
		printf("  |       |   S\n");
		printf("  |   O ======\n");
		printf("  |_______|\n\n");
	}
	if(estadoFumante[id] == FUMANDO2) {
		printf("---FUMANDO--------------------\n\n");
		printf("  <<>>>>>>>\n");
		printf("  |       |\n");
		printf(" <| -   - |> s\n");
		printf("  |   ^   |   S\n");
		printf("  |       |  S\n");
		printf("  |   o ======\n");
		printf("  |_______|\n\n");
	}
	if(estadoFumante[id] == PREPARANDO1) {
		printf("--PREPARANDO------------------\n\n");
		printf("  <<>>>>>>>\n");
		printf("  |       |\n");
		printf(" <| @   @ |>    \"Quase...\"\n");
		printf("  |   ^   |\n");
		printf("  |       |\n");
		printf("  |   U   |\n");
		printf("  |_______|     ===\n\n");
	}
	if(estadoFumante[id] == PREPARANDO2) {
		printf("--PREPARANDO------------------\n\n");
		printf("  <<>>>>>>>\n");
		printf("  |       |\n");
		printf(" <| @   @ |>    \"...pronto!\"\n");
		printf("  |   ^   |\n");
		printf("  |       |\n");
		printf("  |   U   |\n");
		printf("  |_______|     =======\n\n");
	}
	if(estadoFumante[id] == ESPERANDO) {
		printf("--ESPERANDO-------------------\n\n");
		if(ingrFumante[id][TABACO] == 0) {
			printf("  <<>>>>>>>\n");
		}
		else {
			printf("  <<>>>>>>>       _\\|/_\n");
		}
		printf("  |       |\n");
		if(ingrFumante[id][PAPEL] == 0) {
			printf(" <| o   o |>\n");
			printf("  |   ^   |\n");
		}
		else {
			printf(" <| o   o |>       ____\n");
			printf("  |   ^   |       |____|\n");
		}
		printf("  |       |\n");
		printf("  |   U   |\n");
		if(ingrFumante[id][FOSFORO] == 0) {
			printf("  |_______|\n\n");
		}
		else {
			printf("  |_______|       ====o\n\n");
		}
	}

}

// animacao do agente
void printAgente() {
	int altura = (n_fumantes/2) * 11 + 3;
	printf("\033[%d;42H-----TABACARIA-----\n", altura++);
	printf("\033[%d;42H\n", altura++);
	printf("\033[%d;42H           ssSsSsSss\n", altura++);
	printf("\033[%d;42HOferta:    |       |\n", altura++);
	if(ingrediente == -1) {
		printf("\033[%d;42H          <| G   0 |>\n", altura++);
		printf("\033[%d;42H           |   c   |\n", altura++);
	}
	if(ingrediente == TABACO) {
		printf("\033[%d;42H          <| G   0 |>\n", altura++);
		printf("\033[%d;42H _\\|/_     |   c   |\n", altura++);
	}
	if(ingrediente == PAPEL) {
		printf("\033[%d;42H  ____    <| G   0 |>\n", altura++);
		printf("\033[%d;42H |____|    |   c   |\n", altura++);
	}
	if(ingrediente == FOSFORO) {
		printf("\033[%d;42H          <| G   0 |>\n", altura++);
		printf("\033[%d;42H ====o     |   c   |\n", altura++);
	}
	printf("\033[%d;42H           |  ~~~  |\n", altura++);
	printf("\033[%d;42H           |   U   |\n", altura++);
	printf("\033[%d;42H           |,,,,,,,|\n", altura++);
	printf("\033[%d;42H\n", altura++);
	printf("\033[%d;42H-------------------\n", altura++);
}

// funcao que administra a animacao
void *animacao(void *id) {
	int i;
	while(1) {
		printf("\e[1;1H\e[2J");
		printAgente();
		printf("\033[0;0H");
		printf("Fumantes: %2d | Numero de ingr. iniciais: %2d\n", n_fumantes, n_ingr);
		printf("______________________________________________________________\n\n");
		for(i = 0; i < n_fumantes; i++)
			printFumante(i);
		printf("______________________________________________________________\n");
		printf("O MINISTERIO DA SAUDE ADVERTE:\n%s\n", frase);
		//evolucao_texto();

		sleep(1);
	}
	return NULL;
}

// (re)inicia o fumante com os parametros escolhidos
void iniciaFumante(id) {
	int i, aux;

	estadoFumante[id] = ESPERANDO;
	contaIngr[id] = n_ingr;

	// randomiza ingredientes iniciais
	aux = rand();
	// atribui ingredientes
	for(i=0; i<3; i++){
		if(i < n_ingr) {
			ingrFumante[id][(id+i+aux)%3] = 1;
		}
		else {
			ingrFumante[id][(id+i+aux)%3] = 0;
		}
	}
}

// funcao para o comportamento do agente
void *agente(void *id) {
	while(1) {
		sleep(SLEEP_AGENTE);
		ingrediente = -1;
		sleep(SLEEP_ANIMA);
		pthread_mutex_lock(&global_mutex);
		// escolhe ingrediente
		ingrediente = rand()%3;
		sleep(SLEEP_ANIMA);
		pthread_mutex_unlock(&global_mutex);
	}

	return NULL;
}

// funcao para o comportamento dos fumantes
void *fumante (void *v) {
	int id = (int) v;

	iniciaFumante(id);

	while(1) {
		//tenta pegar ingrediente
		pthread_mutex_lock(&global_mutex);
		if(ingrediente != -1 && ingrFumante[id][ingrediente] == 0) {
			contaIngr[id]++;
			ingrFumante[id][ingrediente] = 1;
			ingrediente = -1;
			sleep(SLEEP_ANIMA);
		}
		pthread_mutex_unlock(&global_mutex);

		// pronto?
		if(contaIngr[id] == 3) {
			//prepara
			estadoFumante[id] = PREPARANDO1;
			sleep(SLEEP_PREPARA);
			estadoFumante[id] = PREPARANDO2;
			sleep(SLEEP_PREPARA);
			//fuma
			estadoFumante[id] = FUMANDO1;
			sleep(SLEEP_FUMA);
			estadoFumante[id] = FUMANDO2;
			sleep(SLEEP_FUMA);
			estadoFumante[id] = FUMANDO1;
			sleep(SLEEP_FUMA);
			estadoFumante[id] = FUMANDO2;
			sleep(SLEEP_FUMA);
			estadoFumante[id] = FUMANDO1;
			sleep(SLEEP_FUMA);
			//reset
			iniciaFumante(id);
			sleep(SLEEP_ANIMA);
		}
	}

	return NULL;
}

//funcao que escolhe uma frase aleatoria
void *frases(void *id) {
	int i;
	while(1) {
		i = rand()%8;
		switch(i){
		case 0:
			frase = "FUMAR CAUSA MAU HALITO, PERDA DE DENTES E CANCER DE BOCA";
			break;
		case 1:
			frase = "FUMAR CAUSA CANCER DE PULMAO";
			break;
		case 2:
			frase = "FUMAR CAUSA INFARTO DO CORACAO";
			break;
		case 3:
			frase = "QUEM FUMA NAO TEM FOLEGO PARA NADA";
			break;
		case 4:
			frase = "FUMAR NA GRAVIDEZ PREJUDICA O BEBE";
			break;
		case 5:
			frase = "FUMAR CAUSA IMPOTENCIA SEXUAL";
			break;
		case 6:
			frase = "CRIANCAS COMECAM A FUMAR AO VEREM OS ADULTOS FUMANDO";
			break;
		case 7:
			frase = "WINNERS DON'T USE DRUGS";
			break;
		}
		sleep(SLEEP_FRASE);
	}

	return NULL;
}


int main() {
	pthread_t t_agente, *t_fumante, t_animacao, t_frases;
	int i = 0;

	//iniciando parametros
	printf("\e[1;1H\e[2J");
	while(n_fumantes < 1 || n_fumantes > 5) {
		printf("Digite o numero de fumantes (min. 1  max. 5):\n");
		scanf("%d", &n_fumantes);
		if(n_fumantes < 1 || n_fumantes > 5) {
			printf("O numero minimo de fumantes eh 1 e o maximo eh 5.\n");
		}
	}
	while(n_ingr < 0 || n_ingr > 2) {
		printf("Digite quantos ingredientes cada fumante possui inicialmente (0, 1 ou 2):\n");
		scanf("%d", &n_ingr);
		if(n_ingr < 0 || n_ingr > 2) {
			printf("O numero deve ser 0, 1 ou 2.\n");
		}
	}
	printf("\nIniciando...\n\n Legenda:\n\n");
	printf(" _\\|/_ = TABACO\n");
	printf("  ___\n |___| = PAPEL\n\n");
	printf(" ====o = FOSFORO\n");
	sleep(5);

	//alocacoes de memoria
	t_fumante = (pthread_t*)malloc(n_fumantes*sizeof(pthread_t));
	estadoFumante = (int*)malloc(n_fumantes*sizeof(int));
	contaIngr = (int*)malloc(n_fumantes*sizeof(int));
	ingrFumante = (int**)malloc(n_fumantes*sizeof(int*));
	for(i = 0; i < n_fumantes; i++) {
		ingrFumante[i] = (int*)malloc(n_fumantes*sizeof(int));
	}

	//seed frases
	srand (time(NULL));

	//cria mutexes
	pthread_mutex_init(&global_mutex, NULL);

	//criando threads
	//agente
	pthread_create(&t_agente, NULL, agente, NULL);
	//fumantes
	for(i = 0; i < n_fumantes; i++) {
		pthread_create(&t_fumante[i], NULL, fumante, (void*) i);
	}
	//animcao
	pthread_create(&t_animacao, NULL, animacao, NULL);
	//gerador de frases
	pthread_create(&t_frases, NULL, frases, NULL);

	//joins
	pthread_join(t_agente, NULL);
	for(i = 0; i < 3; i++) {
		pthread_join(t_fumante[i], NULL);
	}
	pthread_join(t_animacao, NULL);
	pthread_join(t_frases, NULL);

	return 0;
}
