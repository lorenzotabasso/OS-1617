#include "libraries.h"

void creazioneFato(){
    pid_t child;
    switch(child = fork()){
        case -1:
            Error();
			
        case 0:
			printf ("Fato (%d) creato!\n", getpid());
			fato = getpid();
			letturaConfigurazione ();
			fpL = fopen(LOG_PATH, "w");
			if (fpL == NULL){
   			     Error();
			}
			signal (SIGUSR1, sveglia);
			signal(SIGUSR2, avviaTimer);	
			signal(SIGALRM, fineTimer);
			kill(getppid(), SIGUSR1);
			pause();
			while(1){
				msgrcv (codaMessaggi, &mtmp, LQ, getpid(), 0);
				switch (mtmp.mtext) {
					case 1: 
						mtmp.mtext = probabilitaGiocata(perc_Infortunio);
						if (mtmp.mtext == 1) {
							sprintf (LINE, "Giocatore %d (%d) della squadra %d (%d) subisce un infortunio.\n", mtmp.mittente, mtmp.player +1, mtmp.teampid, mtmp.team+1);
						} else {
							sprintf (LINE, "Giocatore %d (%d) della squadra %d (%d) evita un infortunio.\n", mtmp.mittente, mtmp.player+1, mtmp.teampid, mtmp.team+1);
						}
						fputs(LINE, fpL);
						mtmp.mtype = mtmp.mittente;
						mtmp.mittente = getpid();
						mtmp.player = -1;
						mtmp.team = -1;
						mtmp.teampid = -1;
						msgsnd (codaMessaggi, &mtmp, LQ, 0);
						break;
						
					case 2:
						mtmp.mtext = probabilitaGiocata(perc_Tiro);
						if (mtmp.mtext == 1) {
							if (mtmp.team == 0) {
								kill (arbitro, SIGUSR1);
								pause();
							} else {
								kill (arbitro, SIGUSR2);
								pause();
							}
						}
						if (mtmp.mtext == 1) {
							sprintf (LINE, "Giocatore %d (%d) della squadra %d (%d) esegue un tiro.\n\nSquadra A [%d] - Squadra B [%d]\n\n", mtmp.mittente, mtmp.player+1, mtmp.teampid, mtmp.team+1, punteggio[0], punteggio[1]);
						} else {
							sprintf (LINE, "Giocatore %d (%d) della squadra %d (%d) fallisce un tiro.\n", mtmp.mittente, mtmp.player+1, mtmp.teampid, mtmp.team+1);
						}
						fputs(LINE, fpL);
						mtmp.mtype = mtmp.mittente;
						mtmp.mittente = getpid();
						mtmp.player = -1;
						mtmp.team = -1;
						mtmp.teampid = -1;
						msgsnd (codaMessaggi, &mtmp, LQ, 0);
						break;
					
					case 3:
						mtmp.mtext = probabilitaGiocata(perc_Dribbling);
						if (mtmp.mtext == 1) {
							sprintf (LINE, "Giocatore %d (%d) della squadra %d (%d) esegue un dribbling.\n", mtmp.mittente, mtmp.player+1, mtmp.teampid, mtmp.team+1);
						} else {
							sprintf (LINE, "Giocatore %d (%d) della squadra %d (%d) fallisce un dribbling.\n", mtmp.mittente, mtmp.player+1, mtmp.teampid, mtmp.team+1);
						}
						fputs(LINE, fpL);
						mtmp.mtype = mtmp.mittente;
						mtmp.mittente = getpid();
						mtmp.player = -1;
						mtmp.team = -1;
						mtmp.teampid = -1;
						msgsnd (codaMessaggi, &mtmp, LQ, 0);
						break;
					
					default:
						break;
				}
			}
            break;
			
        default:
            fato = child;
			signal(SIGUSR1, sveglia);
            pause();
    }
}

void letturaConfigurazione(){
    FILE *fpC = fopen(CONFIG_PATH, "r");
    
    if (fpC == NULL){
        Error();
	}
		
    fgets(LINE, MAX, fpC);
    if (sscanf(LINE, "Durata partita: %d", &durataPartita) != 1) {
		Error();
	} else {
	    if (durataPartita < 0){
		   	Error();
		} else {
			printf ("Durata della partita impostata a: %d\n", durataPartita);
		}
	}
	
	fgets(LINE, MAX, fpC);
    if (sscanf(LINE, "Percentuale infortunio: %d", &perc_Infortunio) != 1) {
		Error();
	} else {
	    if (perc_Infortunio < 0 || perc_Infortunio > 100){
		   	Error();
		} else {
			printf ("Percentuale dell'infortunio impostata a: %d\n", perc_Infortunio);
		}
	}
	
	fgets(LINE, MAX, fpC);
    if (sscanf(LINE, "Percentuale tiro: %d", &perc_Tiro) != 1) {
		Error();
	} else {
	    if (perc_Tiro < 0 || perc_Tiro > 100){
		   	Error();
		} else {
			printf ("Percentuale del tiro impostata a: %d\n", perc_Tiro);
		}
	}

    fgets(LINE, MAX, fpC);
    if (sscanf(LINE, "Percentuale dribbling: %d", &perc_Dribbling) != 1) {
		Error();
	} else {
	    if (perc_Dribbling < 0 || perc_Dribbling > 100){
		   	Error();
		} else {
			printf ("Percentuale del dribbling impostata a: %d\n", perc_Dribbling);
		}
	}
}



int probabilitaGiocata(int probabilita) {
	int x = rand() % 100;
	if (x <= probabilita){
		return 1;
	} else { 
		return 0;
	}
}
