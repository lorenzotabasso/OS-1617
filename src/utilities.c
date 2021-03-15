#include "libraries.h"

void creazioneCodaMessaggi(){
    codaMessaggi = msgget(IPC_PRIVATE, 0400 | 0200);
	if (codaMessaggi == -1) {
		Error();
	}
	printf ("Coda di messaggi creata!\nID della coda di messaggi: %d\n\n", codaMessaggi);
}

void creazioneSemaforoPalla (){
	semaforoPalla = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);
	if (semaforoPalla == -1) {
		Error();
	}
	if (semctl(semaforoPalla, 0, SETVAL, 1) == -1) {
		Error();
	}
	printf("Semaforo per la palla creato e inizializzato a 1!\nID del semaforo: %d\n\n", semaforoPalla);
}

void creazioneSemaforiSquadre (){
	setSemaforiSquadre = semget(IPC_PRIVATE, 2, 0666 | IPC_CREAT);
	if (setSemaforiSquadre == -1) {
		Error();
	}
	if (semctl(setSemaforiSquadre, 0, SETVAL, 5) == -1) {
		Error();
	}
	if (semctl(setSemaforiSquadre, 1, SETVAL, 5) == -1) {
		Error();
	}
	printf("Semafori per le squadre creati e inizializzati a 5!\nID del gruppo di semafori: %d\n\n", setSemaforiSquadre);
}

void allocazioneMemoriaCondivisa(){
	idshm = shmget(IPC_PRIVATE, sizeof(int)*2, 0777 | IPC_CREAT);
	printf("Memoria condivisa per il punteggio creata!\nID della memoria condivisa: %d\n\n", idshm);
	punteggio = (int*) shmat (idshm, 0, 0);
	punteggio[0] = 0;
	punteggio[1] = 0;
}

int richiestaRisorse(int id, int num, int q) {
    struct sembuf lock = {num, -q, IPC_NOWAIT};
    return semop(id, &lock, 1);
}

void rilascioRisorse(int id, int num, int q) {
    struct sembuf unlock = {num, q, 0};
    if (semop(id, &unlock, 1) == -1) {
		Error();
    }
}

void inizioPartita (int signal){
	if (signal == -1) {
		Error();
	} else {
		printf("Lanciamo la monetina per scegliere chi dovrà iniziare la partita!\n\n");
		if (lancioMonetina() == 0) {
			printf("Inizierà la squadra A!\n\n");
			kill(squadra[0], SIGUSR1);
			kill (squadra[1], SIGUSR1);
		} else {
			printf("Inizierà la squadra B!\n\n");
			kill(squadra[1], SIGUSR1);
			kill (squadra[0], SIGUSR1);
		}
	}
}

void avviaTimer(int signal){
	if (signal == -1) {
		Error();
	} else {
		alarm(durataPartita);
		kill(getppid(), SIGUSR1);
	}
}

void fineTimer(int signal) {
	if (signal == -1) {
		Error();
	} else {
		fclose(fpL);
		sleep(1);
		printf("Tempo scaduto!\n\n");
		kill (arbitro, SIGALRM);
		pause();
	}
}

void terminazioneProcessi (int signal) {
	if (signal != SIGALRM){
		Error();
	}
	
	printf ("Risultato finale: Squadra A %d - Squadra B %d\n", punteggio[0], punteggio[1]);
	if (punteggio[0] > punteggio[1]) {
		printf ("Vince la squadra A!\n\n");
	}
	if (punteggio[0] < punteggio[1]) {
		printf ("Vince la squadra B!\n\n");
	}
	if (punteggio[0] == punteggio[1]) {
		printf ("Pareggio!\n\n");
	}
	
	printf ("-----------------------------------------------\n\n");
	
	kill(fato, SIGKILL);
	printf ("Fato (%d) terminato!\n\n", fato);
	for (i = 0; i < 2; i++) {
		kill (squadra[i], SIGALRM);
		waitpid(squadra[i], &status, 0);
		printf ("\n%d° squadra (%d) terminata!\n\n", i+1, squadra[i]);
	}
	deallocazioneIPC();
	exit(0);
}

void terminazioneGiocatori (int signal) {
	if (signal != SIGALRM) {
		Error();
	}
	if (getpid() == squadra[0]) {
		for (i = 0; i < 5; i++) {	
			kill(giocatore[0][i], SIGKILL);
			printf ("%d° giocatore (%d) della 1° squadra terminato!\n", i+1, giocatore[0][i]);
		}
	} else {
		for (i = 0; i < 5; i++) {
			kill(giocatore[1][i], SIGKILL);
			printf ("%d° giocatore (%d) della 2° squadra terminato!\n", i+1, giocatore[1][i]);
		}
	}
	exit(0);
}

void deallocazioneIPC (){
	if (msgctl (codaMessaggi, IPC_RMID, NULL) == -1){
		Error();
	}
	if (semctl(semaforoPalla, 0, IPC_RMID) == -1){
		Error();
	}
	if (semctl(setSemaforiSquadre, 1, IPC_RMID) == -1){
		Error();
	}
	if (shmctl(idshm, IPC_RMID, NULL) == -1) {
		Error();
	}
	printf ("IPC deallocate!\n\n");
}

void sveglia (int signal) {
	if (signal == -1){
		Error();
	}
}

void svegliaGiocatori () {
	if (getpid() == squadra[0]){
		for (i = 0; i < 5; i++)
			kill(giocatore[0][i], SIGUSR1);
	} else {
		for (i = 0; i < 5; i++)
			kill(giocatore[1][i], SIGUSR1);
	}
}

void Error(){
    perror("---> ERRORE");
    exit(EXIT_FAILURE);
}