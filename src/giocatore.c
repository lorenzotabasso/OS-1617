#include "libraries.h"

void creazioneGiocatori(int team, int num, int p){
    int player = 5-num;
    if (num > 0){
        pid_t child;
        switch(child = fork()){
            case -1:
                Error();
				
            case 0:
				printf ("%d° giocatore (%d) della %d° squadra creato!\n", p, getpid(), team+1);
                giocatore[team][player] = getpid();
				richiestaRisorse(setSemaforiSquadre, team, 1);
				// printf ("Il giocatore ha riservato una risorsa del %d° semaforo del set di semafori %d! Risorse rimanenti: %d!\n\n", team+1, setSemaforiSquadre, semctl(setSemaforiSquadre, team, GETVAL));
				ran = getpid();
				signal(SIGUSR1, sveglia);
                kill(getppid(), SIGUSR1); 
                pause();
                while(1){
					tentativo (player, team);
				} 
                break; 
				
            default:
                giocatore[team][player] = child;
                signal(SIGUSR1, sveglia);
                pause();
                creazioneGiocatori(team, num - 1, p + 1);
        }
    }
}

void tentativo (int player, int team) {
	while (richiestaRisorse(semaforoPalla, 0, 1) == -1) {
	}
	printf ("Il giocatore %d della squadra %d ha preso la palla:\n", player + 1, team + 1);
	srand(ran);
	giocata(player, team);
	ran = ran + 20;
	sleep(mosseOgni);
	rilascioRisorse(semaforoPalla, 0, 1);
	sleep(1);
}

int sceltaGiocata () {
	int x = rand() % 100;
	if (x < 33){
		return 1;
	} else {
		if (x > 33 && x < 66){
			return 2;
		} else {
			return 3;
		}	
	}
}

void giocata(int player, int team){
	int scelta = sceltaGiocata();
	mtmp.mtype = fato;
	mtmp.mittente = getpid();
	mtmp.player = player;
	mtmp.team = team;
	mtmp.teampid = getppid();
	mtmp.mtext = scelta;
	msgsnd (codaMessaggi, &mtmp, LQ, 0);
    switch (scelta) {
		case 1: 
			msgrcv (codaMessaggi, &mtmp, LQ, getpid(), 0);
			if (mtmp.mtext == 1) {
				printf (RED "	Ha subito un fallo e deve essere sostiuito!" RESET "\n");
				rilascioRisorse(setSemaforiSquadre, team, 1);
				exit(0);
			} else {
				printf (GRN "	Ha subito un fallo ma è in grado di continuare a giocare!" RESET "\n\n");
			}
			break;
						
		case 2: 
			msgrcv (codaMessaggi, &mtmp, LQ, getpid(), 0);
			if (mtmp.mtext == 1) {
				printf (GRN "	Esegue un tiro ed è GOAL!" RESET "\n\n");
				if (punteggio[0] > punteggio[1]) {
					printf ("--------- " CYN "Squadra A [%d]" RESET " - " "Squadra B [%d]" " ---------\n\n", punteggio[0], punteggio[1]);
				}
				if (punteggio[0] < punteggio[1]) {
					printf ("--------- " "Squadra A [%d]" " - " CYN "Squadra B [%d]" RESET " ---------\n\n", punteggio[0], punteggio[1]);
				}
				if (punteggio[0] == punteggio[1]) {
					printf ("--------- Squadra A [%d] - Squadra B [%d] ---------\n\n", punteggio[0], punteggio[1]);
				}
			} else {
				printf (RED "	Tenta un tiro ma sbaglia..." RESET "\n\n");
			}
			break;
		
		case 3:
			msgrcv (codaMessaggi, &mtmp, LQ, getpid(), 0);
			if (mtmp.mtext == 1) {
				printf (GRN "	Esegue un dribbling!" RESET "\n");
				giocata(player, team);
			} else {
				printf (RED "	Tenta un dribbling ma sbaglia e perde la palla..." RESET "\n\n");
			}
			break;
		
		default:
			break;
	} 
}

