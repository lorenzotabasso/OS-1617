#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>



//IMPOSTAZIONI-------------------------------------------------
#define CONFIG_PATH  "/Users/lorenzotabasso/Dropbox/Progetto SO OFFLINE/BACKUP/FINALE/Progetto/config.txt" // Path del file config.txt
#define LOG_PATH "/Users/lorenzotabasso/Dropbox/Progetto SO OFFLINE/BACKUP/FINALE/Progetto/log.txt" // Path del file log.txt
#define mosseOgni 2
//-------------------------------------------------------------



#if defined(__linux__)
    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short  *array;
        #if defined(__linux__)
            struct seminfo* __buf;
        #endif
    };
#endif

#define MAX 100
#define LQ (sizeof(int)*5)

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

typedef struct _queue{
	long mtype;
	int mittente;
	int player;
	int team;
	int teampid;
	int mtext;
} message;

int i,j;
int status;
int codaMessaggi, semaforoPalla, setSemaforiSquadre, idshm;
int durataPartita, perc_Infortunio, perc_Tiro, perc_Dribbling;
int* punteggio;
char LINE[MAX];
FILE *fpL;
message mtmp;

pid_t arbitro, fato, giocatore[2][5], squadra[2];
pid_t infortunato;
pid_t ran;


//Arbitro
int lancioMonetina();
void segnatoGoal (int signal);

//Fato
void creazioneFato();
void letturaConfigurazione();
int probabilitaGiocata(int probabilita);

//Squadra
void creazioneSquadre(int num);
void creazioneUlterioreGiocatore(int player, int team);

//Giocatore
void creazioneGiocatori(int team, int num, int p);
void tentativo (int player, int team);
int sceltaGiocata ();
void giocata(int player, int team);

//Utilities
void creazioneCodaMessaggi();
void creazioneSemaforoPalla();
void creazioneSemaforiSquadre ();
void allocazioneMemoriaCondivisa();
int richiestaRisorse(int id, int num, int q);
void rilascioRisorse(int id, int num, int q);
void inizioPartita (int signal);
void avviaTimer(int signal);
void fineTimer(int signal);
void terminazioneProcessi (int signal);
void terminazioneGiocatori (int signal);
void deallocazioneIPC ();
void sveglia (int signal);
void svegliaGiocatori ();
void Error();
