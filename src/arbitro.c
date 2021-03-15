#include "libraries.h"

int main (){
	printf ("\nPreparazione della partita...\n\n");
	srand(time(NULL));
	arbitro = getpid();
	creazioneCodaMessaggi ();
	creazioneSemaforoPalla ();
	creazioneSemaforiSquadre ();
	allocazioneMemoriaCondivisa();
	creazioneFato();
	if (getpid() == arbitro){
		creazioneSquadre(2); 
		if (getpid() == arbitro){
			printf ("\n-----------------------------------------------\n");
			printf ("\nPossiamo iniziare la partita!\n\n");
			signal(SIGUSR1, inizioPartita);
			signal (SIGALRM, terminazioneProcessi);
			kill (fato, SIGUSR2);
			pause();
			signal(SIGUSR1, segnatoGoal);
			signal(SIGUSR2, segnatoGoal);
			while (1){
				
			}
		}
	}
}

int lancioMonetina () {
	return (rand()%2);
}

void segnatoGoal (int signal) {
	if (signal == -1) {
		Error();
	}
	if (signal == SIGUSR1) {
		punteggio[0]++;
	}
	if (signal == SIGUSR2) {
		punteggio[1]++;
	}
	kill (fato, SIGUSR1);
}