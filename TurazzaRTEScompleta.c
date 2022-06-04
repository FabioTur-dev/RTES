#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define N 5

//--------------Questa versione "completa" contempla la presenza di N corridori (oltre all'arbitro)---------------//
//-----rappresenta una soluzione ulteriore che ho voluto implementare per rendere la correzione più completa------//
// la soluzione finale risulta essere pressochè identica alla soluzione precedente ad eccezione dei cicli for che // 
// sono stati aggiunti alle funzioni "arbitro_attendicorridori", "arbitro_via", "arbitro_risultato" e disporre di //
// sufficienti semafori generici in base al numero di corridori; è stato aggiunto, inoltre, un controllo if nella //
// funzione "corridore_arrivo" per il cont "ultimo", condizione necessaria per il funzionamento con più corridori //
//----------------------------------------------------------------------------------------------------------------//  

struct corsa_t{
	
	sem_t mutex;
	sem_t preparazione_corridore, attesa_via, attesa_fine;

	int primo, ultimo, giocatore_ultimo; // ultimo è un generico contatore

}corsa;

void init_corsa(struct corsa_t* s){

	sem_init(&s->mutex,0,1);
	sem_init(&s->preparazione_corridore,0,0);	
	sem_init(&s->attesa_via,0,0);	
	sem_init(&s->attesa_fine,0,0);
	
	s->primo = 0;										
	s->giocatore_ultimo = 0;							
	s->ultimo = N;											
}

void arbitro_attendicorridori(struct corsa_t* s){
    for(int i=0; i<N; i++)   // il ciclo for effettua una wait al semaforo di preparazione per ogni corridore (bloccandolo)
	    sem_wait(&s->preparazione_corridore);		
}

void corridore_attendivia(struct corsa_t* s, int numerocorridore){
	sem_post(&s->preparazione_corridore);
	sem_wait(&s->attesa_via);	
}

void arbitro_via(struct corsa_t* s){
	for(int i=0; i<N; i++)   // il ciclo for effettua una post al semaforo di attesa del via, sbloccando ogni corridore
	    sem_post(&s->attesa_via);
}

void corridore_arrivo(struct corsa_t* s, int numerocorridore){
	sem_wait(&s->mutex);
	
	if (s->primo == 0){
		s->primo = numerocorridore;
    }
    if (s->ultimo > 1){   // ho aggiunto un ulteriore controllo if per decrementare il contatore generico
		s->ultimo = s->ultimo - 1;
		sem_post(&s->mutex);
	}
	else if (s->ultimo == 1){ 
		s->giocatore_ultimo = numerocorridore;
		sem_post(&s->mutex);
	}
	sem_post(&s->attesa_fine);
}

void arbitro_risultato(struct corsa_t* s, int *giocatore_primo, int *giocatore_ultimo){

	for(int i=0; i<N; i++){   // il ciclo for effettua una wait al semaforo di attesa_fine per ogni corridore
	    sem_wait(&s->attesa_fine);
    }
	printf("Il primo classificato è: %d\n", s->primo);
	printf("Ultimo classificato è: %d\n", s->giocatore_ultimo);
}

void *corridore(void *arg){
	int numerocorridore=(int)arg;
	sleep(3);
	corridore_attendivia(&corsa, numerocorridore);
	sleep(3);
	corridore_arrivo(&corsa, numerocorridore);
	sleep(3);
}

void *arbitro(void *arg){
	int primo, ultimo;
	sleep(3);												
	arbitro_attendicorridori(&corsa);
	sleep(3);
	arbitro_via(&corsa);
	sleep(3);
	arbitro_risultato(&corsa, &primo, &ultimo);
}

int main(){	
	int i, num_gioc[N];
	pthread_attr_t a;
  	pthread_t thread_corridore[N], thread_arbitro;
	
	//inizializzo i numeri casuali in maniera pseudo-randomica
	srand(100);
	
	pthread_attr_init(&a);
  	
  	for (i=0; i<100; i++){
		init_corsa(&corsa);
		
        for(int i=0; i<N; i++){
		    num_gioc[i] = rand()%100+1;
            printf ("ID corridore %d: %d\n", i+1, num_gioc[i]);
        }
		
        for(int i=0; i<N; i++){
  		    pthread_create(&thread_corridore[i], &a, corridore, (void *) num_gioc[i]);
        }

  		pthread_create(&thread_arbitro, &a, arbitro, NULL);
  		
        for(int i=0; i<N; i++){
  		    pthread_join(thread_corridore[i], NULL);
        }

  		pthread_join(thread_arbitro, NULL);
  		printf("\n");
	}
	
	pthread_attr_destroy(&a);
    sleep(3);			
	
	return 0;
}






