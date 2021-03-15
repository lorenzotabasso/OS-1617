#include "libraries.h"

void creazioneSquadre(int num){
    int team = 2-num;
    if (num > 0){
        pid_t child;
        switch(child = fork()){
            case -1:
                Error();
				
            case 0:
				printf ("\n%d° squadra (%d) creata!\n\n", team+1, getpid());
                squadra[team] = getpid();
                creazioneGiocatori(team, 5, 1);
                if (getpid() == squadra[team]){
					signal(SIGALRM, terminazioneGiocatori);
					signal(SIGUSR1, sveglia);
					kill(getppid(), SIGUSR1);
					pause();
					svegliaGiocatori();
					while (1) {
						infortunato = wait(&status);
						int p;
						for (i = 0; i < 5; i++) {
							if (giocatore[team][i] == infortunato) {
								p = i;
							}
						}
						creazioneUlterioreGiocatore(p, team);
					}
                }
				break;
				
            default:
                squadra[team] = child;
                signal(SIGUSR1, sveglia);
                pause();
                if (num > 1) {
					creazioneSquadre(num - 1);
				}
        }
	}
}

void creazioneUlterioreGiocatore(int player, int team){
	pid_t nuovo;
	switch(nuovo = fork()){
		case -1:
			Error();
			
		case 0:
			richiestaRisorse(setSemaforiSquadre, team, 1);
			ran = getpid();
			while(1){
				tentativo (player, team);
			} 
			break;
			
		default:
			giocatore[team][player] = nuovo;
			printf ("	Il giocatore %d esce ed entra %d\n\n", infortunato, nuovo);
			sleep(mosseOgni);
			rilascioRisorse(semaforoPalla, 0, 1);
			break;
	}
}

