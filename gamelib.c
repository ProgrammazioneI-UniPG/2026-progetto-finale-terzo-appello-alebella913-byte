#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "gamelib.h"

static int undi;
static int numzone;
static int numgio;
static int torzone_check;//per check impostazione mappa
static int mapgen=0;
static int map_impo;

static struct Zona_mondoreale *prima_mondoreale = NULL;
static struct Zona_soprasotto *prima_soprasotto = NULL;
static struct Zona_mondoreale* attualeM = NULL;
static struct Zona_soprasotto* attualeS = NULL;
struct Zona_mondoreale* freeptrM=NULL;
struct Zona_soprasotto* freeptrS=NULL;

struct Giocatore* giocatori[4];
static char vincitori[3][25];
static int round_record[3]={
  INT_MAX,INT_MAX,INT_MAX
};//li ho elencati ma per roba più lunga conviene un for che li inizializza tutti
static char* undiwin[1]={NULL};

static const char* tipo_oggetto_nome[5]={
  "Vuoto",
  "Bicicletta",
  "Maglietta Fuocoinferno",
  "Bussola",
  "Schitarrata Metallica"
};
static const char* tipo_nemico_nome[4]={
  "Nessuno",
  "Billi",
  "Democane",
  "Demotorzone"
};
static const char* tipo_zona_nome[10]={
  "Bosco",
  "Scuola",
  "Laboratorio",
  "Caverna",
  "Strada",
  "Giardino",
  "Supermercato",
  "Centrale Elettrica",
  "Deposito Abbandonato",
  "Stazione di Polizia"
};

int scancheck(int opt) {
int scan;
char check;
  while ((scanf("%d%c", &scan, &check)!= 2) || scan <1 || scan >opt || check != '\n') {
    printf("Input non valido, riprova con un intero da 1 a %d\n", opt);
    while (getchar() != '\n');
  }
return scan;} //non static perchè mi faceva comodo anche in Exam.c

static void cancella_mappa(){
  for(attualeM = prima_mondoreale; attualeM!=NULL; attualeM = freeptrM){
  freeptrM=attualeM->avanti;
  free(attualeM);
  }
  for(attualeS = prima_soprasotto; attualeS!=NULL; attualeS = freeptrS){
  freeptrS=attualeS->avanti;
  free(attualeS);
  }
  prima_mondoreale=NULL;
  prima_soprasotto=NULL;
  attualeM=prima_mondoreale;
  attualeS=prima_soprasotto;
  numzone=0;
}

static void reset(){
  if(mapgen==1){
    cancella_mappa();

    for(int i=0; i<4; i++){
      free(giocatori[i]);
      giocatori[i]=NULL;}

    mapgen=0;
    numgio=0;}
}

static void stampa_mappa(){
  printf("Quale mappa vuoi stampare?\n");
  printf("1)Mondo Reale\n");
  printf("2)Soprasotto\n");
  printf("3)Entrambe\n");
  int MapStamp = scancheck(3);
  switch (MapStamp){
       case 1 : for(attualeM=prima_mondoreale; attualeM!=NULL; attualeM=attualeM->avanti){printf("MONDOREALE ");
       printf("Zona %d:\n Tipo: %s, Nemico: %s, Oggetto: %s\n\n",
    attualeM->ord, tipo_zona_nome[attualeM->tipo], tipo_nemico_nome[attualeM->nemico], tipo_oggetto_nome[attualeM->oggetto]);
    }
    attualeM=prima_mondoreale;
    break;
       case 2 : for(attualeS=prima_soprasotto; attualeS!=NULL; attualeS=attualeS->avanti){printf("SOPRASOTTO ");
         printf("Zona %d:\n Tipo: %s, Nemico: %s\n\n",
    attualeS->ord, tipo_zona_nome[attualeS->tipo], tipo_nemico_nome[attualeS->nemico]);
    }
    attualeS=prima_soprasotto;
    break;
       case 3 : for(attualeM=prima_mondoreale; attualeM!=NULL; attualeM=attualeM->avanti){printf("MONDOREALE ");
       printf("Zona %d:\n Tipo: %s, Nemico: %s, Oggetto: %s\n\n",
    attualeM->ord, tipo_zona_nome[attualeM->tipo], tipo_nemico_nome[attualeM->nemico], tipo_oggetto_nome[attualeM->oggetto]);
    }
    attualeM=prima_mondoreale;
                for(attualeS=prima_soprasotto; attualeS!=NULL; attualeS=attualeS->avanti){printf("SOPRASOTTO ");
                  printf("Zona %d:\n Tipo: %s, Nemico: %s\n\n",
    attualeS->ord, tipo_zona_nome[attualeS->tipo], tipo_nemico_nome[attualeS->nemico]);
    }
    attualeS=prima_soprasotto;
    break;
}


}

static void genera_mappa(){
if(mapgen==1){cancella_mappa();}

numzone = 15;
torzone_check = 0;

for(int i=0; i<numzone; i++){
struct Zona_mondoreale *mondoreale = (struct Zona_mondoreale*)malloc(sizeof(struct Zona_mondoreale));
struct Zona_soprasotto *soprasotto = (struct Zona_soprasotto*)malloc(sizeof(struct Zona_soprasotto));

  mondoreale->ord = i+1;
  mondoreale->tipo = (rand() % 10);
  if((rand() % 10) < 7){mondoreale->nemico = nessun_nemico;}
    else if((rand() % 2) == 0){mondoreale->nemico = billi;}
    else{mondoreale->nemico = democane;}
  if((rand() % (1+numgio)) == 0){mondoreale->oggetto = nessun_oggetto;}
    else{mondoreale->oggetto = (rand() % 4)+1;}

  soprasotto->ord = mondoreale->ord;
  soprasotto->tipo = mondoreale->tipo;

  if((rand() % 10) < 7){soprasotto->nemico = nessun_nemico;}
  else if(torzone_check == 0){soprasotto->nemico = (rand() % 2)+2;}
  else {soprasotto->nemico = democane;}
  if(soprasotto->nemico == demotorzone){torzone_check=1;}
  if(torzone_check == 0 && i == numzone-1){soprasotto->nemico = demotorzone;
  torzone_check=1;}


if (prima_mondoreale == NULL && prima_soprasotto == NULL){
    prima_mondoreale = mondoreale;
    mondoreale->indietro = NULL;
    mondoreale->avanti = NULL; //per evitare stranezze
    mondoreale->link_soprasotto = soprasotto;
    prima_soprasotto = soprasotto;
    soprasotto->indietro = NULL;
    soprasotto->avanti = NULL; //come sopra
    soprasotto->link_mondoreale = mondoreale;
    attualeM = prima_mondoreale;
    attualeS = prima_soprasotto;
  }

else {
  mondoreale->indietro=attualeM;
  mondoreale->avanti = NULL;
  attualeM->avanti=mondoreale;
  mondoreale->link_soprasotto = soprasotto;

  soprasotto->indietro=attualeS;
  soprasotto->avanti = NULL;
  attualeS->avanti=soprasotto;
  soprasotto->link_mondoreale = mondoreale;

  attualeM = mondoreale;
  attualeS = soprasotto;
}
}
  attualeS = prima_soprasotto;
  attualeM = prima_mondoreale;
printf("Nuova mappa generata\n");
}

static void inserisci_zona(){
 if(numzone>=20){
  printf("Max numero zone (20) raggiunto\n"); //aggiunto io per questioni di bilanciamento
   return;
 }
  struct Zona_mondoreale *mondoreale = (struct Zona_mondoreale*)malloc(sizeof(struct Zona_mondoreale));
  struct Zona_soprasotto *soprasotto = (struct Zona_soprasotto*)malloc(sizeof(struct Zona_soprasotto));
    printf("In quale posizione inserire la zona?\n(da 1 a %d)\n", numzone+1);
    int new_ord=scancheck(numzone+1);
     mondoreale->tipo = (rand() % 10);       //non capisco perchè da consegna dovrebbe generarla casualmente
     soprasotto->tipo = mondoreale->tipo;    //se poi ci fa inserire i campi più avanti
                                             //quindi non la genero casualmente

  printf("c'è un nemico nel Mondo Reale?\n");
  for(int i=0; i<3; i++){
   printf("%d) %s\n", i+1, tipo_nemico_nome[i]);}
   mondoreale->nemico=(scancheck(3)-1);
  printf("C'è un oggetto?\n");
  for (int i=0; i<5; i++) {
    printf("%d) %s\n", i+1, tipo_oggetto_nome[i]);}
   mondoreale->oggetto=(scancheck(5)-1);
  printf("C'è un nemico nel Sottosopra?\n");
   printf("1) Nessuno\n");
   printf("2) Democane\n");
   if(torzone_check==0){printf("3) Demotorzone\n");}
   int nemicoS;
   if((nemicoS = scancheck(3-torzone_check))== 1){
     soprasotto->nemico= nessun_nemico;
   }
   else {
     soprasotto->nemico=nemicoS; if(nemicoS==demotorzone){torzone_check=1;}
   }
     mondoreale->link_soprasotto=soprasotto;
     soprasotto->link_mondoreale=mondoreale;

   if(new_ord==1){//potevo usare uno switch effettivamente
     mondoreale->avanti=prima_mondoreale;
     mondoreale->indietro = NULL;
     prima_mondoreale->indietro=mondoreale;

     soprasotto->avanti=prima_soprasotto;
     soprasotto->indietro=NULL;
     prima_soprasotto->indietro=soprasotto;

     prima_mondoreale=mondoreale;
     prima_soprasotto=soprasotto;
     }
   else if (new_ord==(numzone+1)){
     for (attualeM=prima_mondoreale; attualeM!=NULL && attualeM->ord!=numzone; attualeM=attualeM->avanti){}
     //scorritore di lista mondoreale fino a ultima (potevo anche mettere ;attualeM->avanti!=NULL && attualeM->ord!=new_ord;)
       mondoreale->avanti=NULL;
       mondoreale->indietro = attualeM;
       attualeM->avanti = mondoreale;

     for (attualeS=prima_soprasotto; attualeS->avanti!=NULL && attualeS->ord!=new_ord; attualeS=attualeS->avanti){}
     //scorritore di lista soprasotto fino a ultima (come sopra)
       soprasotto->avanti= NULL;
       soprasotto->indietro = attualeS;
       attualeS->avanti = soprasotto;
     }
   else{
     for (attualeM=prima_mondoreale; attualeM!=NULL && attualeM->ord!=new_ord; attualeM=attualeM->avanti){}
         //scorritore di lista mondoreale
     mondoreale->avanti= attualeM;
     mondoreale->indietro = attualeM->indietro;
     attualeM->indietro->avanti = mondoreale;
     attualeM->indietro=mondoreale;

     for (attualeS=prima_soprasotto; attualeS!=NULL && attualeS->ord!=new_ord; attualeS=attualeS->avanti){}
         //scorritore di lista soprasotto
     soprasotto->avanti= attualeS;
     soprasotto->indietro = attualeS->indietro;
     attualeS->indietro->avanti = soprasotto;
     attualeS->indietro=soprasotto;
     }
   for(attualeM=mondoreale->avanti; attualeM!=NULL; attualeM=attualeM->avanti){ //aggiorna gli ord
     attualeM->ord++;
     attualeM->link_soprasotto->ord=attualeM->ord;
   }

  mondoreale->ord=new_ord;
  soprasotto->ord = mondoreale->ord;

  attualeM=prima_mondoreale;
  attualeS=prima_soprasotto;

numzone++;
}

static void cancella_zona(){
  printf("Quale zona vuoi eliminare? (da 1 a %d)\n", numzone);
  int canc_ord=scancheck(numzone);
  if(canc_ord==1){
            if(prima_soprasotto->nemico==demotorzone){torzone_check=0;}
            prima_mondoreale=prima_mondoreale->avanti;
            prima_soprasotto=prima_soprasotto->avanti;
            attualeM=prima_mondoreale->indietro;
            attualeS=prima_soprasotto->indietro;
            prima_mondoreale->indietro=NULL;
            prima_soprasotto->indietro=NULL;
            free(attualeM);
            free(attualeS);
          }
  else if(canc_ord==numzone){
    for(attualeM=prima_mondoreale; attualeM->ord<canc_ord; attualeM=attualeM->avanti){}
              if(attualeM->link_soprasotto->nemico==demotorzone){torzone_check=0;}
              attualeM->indietro->avanti=NULL;
              attualeM->link_soprasotto->indietro->avanti=NULL;
              free(attualeM->link_soprasotto);
              free(attualeM);
  }

  else{ for(attualeM=prima_mondoreale; attualeM->ord<canc_ord; attualeM=attualeM->avanti){}
            if(attualeM->link_soprasotto->nemico==demotorzone){torzone_check=0;}
            attualeM->avanti->indietro=attualeM->indietro;
            attualeM->indietro->avanti=attualeM->avanti;
            attualeM->link_soprasotto->avanti->indietro=attualeM->link_soprasotto->indietro;
            attualeM->link_soprasotto->indietro->avanti=attualeM->link_soprasotto->avanti;
            free(attualeM->link_soprasotto);
            free(attualeM);
  }
            for(attualeM=prima_mondoreale; attualeM!=NULL; attualeM=attualeM->avanti){
              if(attualeM->ord>canc_ord){
                attualeM->ord--;
                attualeM->link_soprasotto->ord--;
              }
            }
attualeM=prima_mondoreale;
attualeS=prima_soprasotto;
numzone--;
}

static void stampa_zona(){
printf("Quale zona vuoi vedere? (da 1 a %d)\n", numzone);
int print_ord=scancheck(numzone);
for(attualeM=prima_mondoreale; attualeM->ord!=print_ord; attualeM=attualeM->avanti){}
printf("MONDOREALE ");
printf("Zona %d:\n Tipo: %s, Nemico: %s, Oggetto: %s\n\n",
attualeM->ord, tipo_zona_nome[attualeM->tipo], tipo_nemico_nome[attualeM->nemico], tipo_oggetto_nome[attualeM->oggetto]);
attualeM=prima_mondoreale;
for(attualeS=prima_soprasotto; attualeS->ord!=print_ord; attualeS=attualeS->avanti){}
printf("SOPRASOTTO ");
printf("Zona %d:\n Tipo: %s, Nemico: %s\n\n",
attualeS->ord, tipo_zona_nome[attualeS->tipo], tipo_nemico_nome[attualeS->nemico]);
attualeS=prima_soprasotto;
}

static void chiudi_mapmenu(){
  if(torzone_check!=1){printf("Deve essere presente 1 Demotorzone\n"); map_impo=67;}
  if(numzone<15 || numzone>20){printf("Il numero di zone deve essere compreso tra 15 e 20\n"); map_impo=67;}
}

void imposta_gioco(){
reset();
mapgen=1;
printf("Inserire numero giocatori (1,2,3,4):\n");
numgio = scancheck(4);
 for (int i = 0; i < numgio; i++) {
  giocatori[i] = malloc(sizeof(struct Giocatore));
   printf("Inserire nome Giocatore %d:\n(Max 24 caratteri o il nome verrà accorciato)\n", i+1);
    while(1){fgets(giocatori[i]->nome, sizeof(giocatori[i]->nome), stdin); //così prende anche spazi
    giocatori[i]->nome[strcspn(giocatori[i]->nome, "\n")] = '\0';                //toglie il newline
    while (getchar() != '\n');    //pulisce eventuali rimasugli che vanno sullo scancheck successivo
    if((strlen(giocatori[i]->nome) == 0) || strspn(giocatori[i]->nome, " ") == strlen(giocatori[i]->nome)){
      printf("Non solo spazi e almeno 1 carattere\n");}
    else{break;}
    }
  giocatori[i]->win = 0;
  giocatori[i]->mondo = 0;
  giocatori[i]->psyatk = ((rand() % 20)+1);
  giocatori[i]->psydef = ((rand() % 20)+1);
  giocatori[i]->fortuna = (rand() % 20)+1;
  giocatori[i]->pos_mondoreale = NULL;
  giocatori[i]->pos_soprasotto = NULL;
   printf("Attacco psichico: %d\n", giocatori[i]->psyatk);
   printf("Difesa psichica: %d\n", giocatori[i]->psydef);
   printf("Fortuna: %d\n", giocatori[i]->fortuna);
    printf("1)+3atk -3dif\n2)-3atk +3dif\n3)Skip\n(Le statistiche non vanno oltre 20 e sotto 1):\n");
    int statup = scancheck(3);
    switch (statup) {
      case 1: giocatori[i]->psyatk += 3; giocatori[i]->psydef -= 3;
      break;
      case 2: giocatori[i]->psyatk -= 3; giocatori[i]->psydef += 3;
      break;
      case 3: break;
      default: break; //sempre in caso di emergenza
    }
    if (giocatori[i]->psyatk > 20){giocatori[i]->psyatk = 20;} //per evitare statistiche negative
    if (giocatori[i]->psyatk < 1){giocatori[i]->psyatk = 1;} //e per bilanciare di conseguenza
    if (giocatori[i]->psydef > 20){giocatori[i]->psydef = 20;}
    if (giocatori[i]->psydef < 1){giocatori[i]->psydef = 1;}
   printf("Nuove statistiche:\n");
    printf("Attacco psichico: %d\n", giocatori[i]->psyatk);
    printf("Difesa psichica: %d\n", giocatori[i]->psydef);
    printf("Fortuna: %d\n\n", giocatori[i]->fortuna);
 for(int a=0; a<3; a++){
   giocatori[i]->zaino[a]= nessun_oggetto;
 }

 printf("Lo zaino contiene %s, %s, %s.\n\n",
 tipo_oggetto_nome[giocatori[i]->zaino[0]], tipo_oggetto_nome[giocatori[i]->zaino[1]], tipo_oggetto_nome[giocatori[i]->zaino[2]]);
}

for(int i=numgio; i<4; i++){giocatori[i]=NULL;} //numgio va da 1 a 4 mentrei gli slot di giocatori[4] da 0 a 3 quindi funziona

  printf("Chi vuole diventare UndiciVirgolaCinque (+4atk +4dif -7for)\n");
               printf("1)%s\n", giocatori[0]->nome);
  if(numgio>1){printf("2)%s\n", giocatori[1]->nome);}
  if(numgio>2){printf("3)%s\n", giocatori[2]->nome);}
  if(numgio>3){printf("4)%s\n", giocatori[3]->nome);}
               printf("%d)Nessuno\n", numgio+1);
   undi = scancheck(numgio+1)-1;
   if(undi >= 0 && undi < numgio){
   giocatori[undi]->psyatk += 4;
   giocatori[undi]->psydef += 4;
   giocatori[undi]->fortuna -= 7;
   if (giocatori[undi]->psyatk > 20){giocatori[undi]->psyatk = 20;}
   if (giocatori[undi]->psydef > 20){giocatori[undi]->psydef = 20;}
   if (giocatori[undi]->fortuna < 1){giocatori[undi]->fortuna = 1;}
   printf("Il nome di %s diventa UndiciVirgolaCinque\n", giocatori[undi]->nome);
   printf("Nuove statistiche di %s:\n", giocatori[undi]->nome);

   undiwin[0] = malloc(strlen(giocatori[undi]->nome)+1);
   strcpy(undiwin[0], giocatori[undi]->nome);

   strcpy(giocatori[undi]->nome, "UndiciVirgolaCinque");
    printf("Attacco psichico: %d\n", giocatori[undi]->psyatk);
    printf("Difesa psichica: %d\n", giocatori[undi]->psydef);
    printf("Fortuna: %d\n\n", giocatori[undi]->fortuna);
}

genera_mappa();
stampa_mappa();//visualizzazione dopo la prima mappa

do {
  printf("========MENÙ IMPOSTAZIONI========\n1) Genera Mappa\n2) Inserisci Zona\n3) Cancella Zona\n");
  printf("4) Visualizza Mappe\n5) Visualizza Zona\n6) Chiudi Menù Mappa\n");
   map_impo = scancheck(6);
  switch (map_impo) {
    case 1: genera_mappa();
    break;
    case 2: inserisci_zona();
    break;
    case 3: cancella_zona();
    break;
    case 4: stampa_mappa();
    break;
    case 5: stampa_zona();
    break;
    case 6: chiudi_mapmenu();
    break;
    default: break;
    }
   }
  while(map_impo != 6);
}

static int primo, secondo, terzo, quarto;
static int *round[4]={&primo, &secondo, &terzo, &quarto};
static int movimento;
static int game_menù;
static int gameopt;
static int fighting=0;
static int empty_check;
static int defbuff=0;
static int atkbuff=0;
static int numround;

static void turn_ord(){
primo = rand() % numgio;
if (numgio>1){while (primo==secondo) {secondo = rand() % numgio;}}
if (numgio>2){while(terzo==primo || terzo==secondo){terzo = rand() % numgio;}}
if (numgio>3){while(quarto == terzo || quarto==secondo || quarto==primo){quarto = rand() % numgio;}}
}

static void stampa_zona_ingame(int ordgio){
if(giocatori[ordgio]->mondo==0){
printf("MONDOREALE ");
printf("Zona %d:\n Tipo: %s, Nemico: %s, Oggetto: %s\n\n", giocatori[ordgio]->pos_mondoreale->ord,
tipo_zona_nome[giocatori[ordgio]->pos_mondoreale->tipo], tipo_nemico_nome[giocatori[ordgio]->pos_mondoreale->nemico],
tipo_oggetto_nome[giocatori[ordgio]->pos_mondoreale->oggetto]);
}
else{
printf("SOPRASOTTO ");
printf("Zona %d:\n Tipo: %s, Nemico: %s\n\n",
giocatori[ordgio]->pos_soprasotto->ord, tipo_zona_nome[giocatori[ordgio]->pos_soprasotto->tipo],
tipo_nemico_nome[giocatori[ordgio]->pos_soprasotto->nemico]);
}
}

static void avanza(int ordgio){
if (movimento>0){printf("Ti sei già mosso questo turno\n");}
else if  (giocatori[ordgio]->pos_mondoreale->avanti==NULL){printf("Sei ai confini della mappa, torna indietro\n");}
else if(((giocatori[ordgio]->mondo==0 && giocatori[ordgio]->pos_mondoreale->nemico!=nessun_nemico)
     ||  (giocatori[ordgio]->mondo==1 && giocatori[ordgio]->pos_soprasotto->nemico!=nessun_nemico)) && giocatori[ordgio]->win==0){
          printf("Un Nemico ti sbarra la strada\n");}
else{
  giocatori[ordgio]->pos_mondoreale=giocatori[ordgio]->pos_mondoreale->avanti;
  giocatori[ordgio]->pos_soprasotto=giocatori[ordgio]->pos_soprasotto->avanti;
  movimento++; giocatori[ordgio]->win=0; stampa_zona_ingame(ordgio);
}
}
static void indietreggia(int ordgio){
if (movimento>0){printf("Ti sei già mosso questo turno\n");}
else if  (giocatori[ordgio]->pos_mondoreale->indietro==NULL){printf("Sei ai confini della mappa, avanza\n");}
else if(((giocatori[ordgio]->mondo==0 && giocatori[ordgio]->pos_mondoreale->nemico!=nessun_nemico)
     ||  (giocatori[ordgio]->mondo==1 && giocatori[ordgio]->pos_soprasotto->nemico!=nessun_nemico)) && giocatori[ordgio]->win==0){
          printf("Un Nemico ti sbarra la strada\n");}
else{
  giocatori[ordgio]->pos_mondoreale=giocatori[ordgio]->pos_mondoreale->indietro;
  giocatori[ordgio]->pos_soprasotto=giocatori[ordgio]->pos_soprasotto->indietro;
  movimento++; giocatori[ordgio]->win=0; stampa_zona_ingame(ordgio);
}
}
static void cambia_mondo(int ordgio){
int dado;
if((giocatori[ordgio]->mondo==0 && movimento<=0 && giocatori[ordgio]->pos_mondoreale->nemico==nessun_nemico)
 ||(giocatori[ordgio]->mondo==0 && movimento<=0 && giocatori[ordgio]->win==1)){
    giocatori[ordgio]->mondo=1; movimento++; stampa_zona_ingame(ordgio); giocatori[ordgio]->win=0;}

else if (giocatori[ordgio]->mondo==0 && giocatori[ordgio]->pos_mondoreale->nemico!=nessun_nemico){
         printf("\nUn Nemico ti sbarra la strada\n");}

else if((giocatori[ordgio]->mondo==1 && fighting==0 && movimento<=0 && giocatori[ordgio]->pos_soprasotto->nemico==nessun_nemico)
     || (giocatori[ordgio]->mondo==1 && fighting==0 && movimento<=0 && giocatori[ordgio]->win==1)){
         giocatori[ordgio]->mondo=0; stampa_zona_ingame(ordgio); giocatori[ordgio]->win=0; movimento++;}

else if (giocatori[ordgio]->mondo==1 && movimento<=0 && giocatori[ordgio]->pos_soprasotto->nemico!=nessun_nemico){
         dado=((rand() % 20)+1); printf("\nÈ uscito %d\n\n", dado);
         if   (dado<=giocatori[ordgio]->fortuna){
               giocatori[ordgio]->mondo=0; stampa_zona_ingame(ordgio); giocatori[ordgio]->win=0; movimento++;}
         else {printf("Fuga fallita \n");}
}
else if (giocatori[ordgio]->mondo==1 && fighting==1){
         dado=((rand() % 20)+1); printf("\nÈ uscito %d\n\n", dado);
         if   (dado<=giocatori[ordgio]->fortuna){
               giocatori[ordgio]->mondo=0; stampa_zona_ingame(ordgio); giocatori[ordgio]->win=0;
               giocatori[ordgio]->psyatk-=atkbuff;
               giocatori[ordgio]->psydef-=defbuff;
               atkbuff=0;
               defbuff=0;
               fighting=0;
               movimento++;}
         else {printf("Fuga fallita\n");}}

else if (movimento>0){printf("\nTi sei già mosso questo turno\n");}
}

static void stampa_stats(int ordgio){
  printf("      Nome:        %s\n", giocatori[ordgio]->nome);
  printf("\n Attacco psichico: %d\n", giocatori[ordgio]->psyatk);
  printf("\n Difesa psichica:  %d\n", giocatori[ordgio]->psydef);
  printf("\n     Fortuna:      %d\n", giocatori[ordgio]->fortuna);
  printf("\nLo zaino contiene: %s, %s, %s.\n\n",
  tipo_oggetto_nome[giocatori[ordgio]->zaino[0]], tipo_oggetto_nome[giocatori[ordgio]->zaino[1]],
  tipo_oggetto_nome[giocatori[ordgio]->zaino[2]]);
}

static void raccogli_oggetto(int ordgio){
  if(giocatori[ordgio]->pos_mondoreale->oggetto==nessun_oggetto){
    printf("Non c è niente da raccogliere\n");}

  else if(giocatori[ordgio]->pos_mondoreale->nemico!=nessun_nemico && giocatori[ordgio]->win==0){
    printf("Un nemico ti impedisce di raccogliere l oggetto\n");}

  else{
    printf("In quale tasca metti l'oggetto?\n");

    for(int i=0; i<3; i++){
      printf("%d) %s\n", i+1, tipo_oggetto_nome[giocatori[ordgio]->zaino[i]]);}

    int tasca=(scancheck(3)-1);
    int temp_poket=giocatori[ordgio]->zaino[tasca];
    giocatori[ordgio]->zaino[tasca]=giocatori[ordgio]->pos_mondoreale->oggetto;
    giocatori[ordgio]->pos_mondoreale->oggetto=temp_poket;
    temp_poket=0;
}
}

static void utilizza_oggetto(int ordgio){
empty_check=0;
 for(int i=0; i<3; i++){
  if(giocatori[ordgio]->zaino[i]==nessun_oggetto){
     empty_check++;}}

   if(empty_check>2){
   printf("Lo zaino è vuoto\n");}

   else{
    printf("Quale oggetto vuoi usare?\n");
    int j=0;
    for(int i=0; i<3; i++){
      if(giocatori[ordgio]->zaino[i]!=nessun_oggetto){
         j++;
         printf("%d) %s\n", j, tipo_oggetto_nome[giocatori[ordgio]->zaino[i]]);}}

   int tasca=scancheck(j);
   int valid_tasca=0;
   int oggetto_usato;
   int k;
   for(k=0; k<3; k++){
     if(giocatori[ordgio]->zaino[k]!=nessun_oggetto){
        valid_tasca++;
        if(valid_tasca==tasca){
          oggetto_usato=giocatori[ordgio]->zaino[k]; break;}}}

   printf("\nHai scelto %s: ", tipo_oggetto_nome[oggetto_usato]);
   switch (oggetto_usato) {
     case 1: printf("Aumenta la tua capacità di movimento per questo turno\n"); movimento--;
     break;
     case 2: if(fighting==1){
             printf("Distrae il nemico con colori sgargianti\n(+5def per il resto del combattimento)\n");
             defbuff+=5; //in caso di doppioni
             giocatori[ordgio]->psydef+=5;}

             else{printf("Da usare in combattimento, oggetto non consumato\n");return;}
     break;
     case 3: printf("Ti orienti leggermente meglio\n");
     printf("MONDOREALE ");
     printf("Zona %d:\n Tipo: %s, Nemico: %s, Oggetto: %s\n\n", giocatori[ordgio]->pos_mondoreale->indietro->ord,
     tipo_zona_nome[giocatori[ordgio]->pos_mondoreale->indietro->tipo], tipo_nemico_nome[giocatori[ordgio]->pos_mondoreale->indietro->nemico],
     tipo_oggetto_nome[giocatori[ordgio]->pos_mondoreale->indietro->oggetto]);
     printf("SOPRASOTTO ");
     printf("Zona %d:\n Tipo: %s, Nemico: %s\n\n",
     giocatori[ordgio]->pos_soprasotto->indietro->ord, tipo_zona_nome[giocatori[ordgio]->pos_soprasotto->indietro->tipo],
     tipo_nemico_nome[giocatori[ordgio]->pos_soprasotto->indietro->nemico]);
     printf("MONDOREALE ");
     printf("Zona %d:\n Tipo: %s, Nemico: %s, Oggetto: %s\n\n", giocatori[ordgio]->pos_mondoreale->ord,
     tipo_zona_nome[giocatori[ordgio]->pos_mondoreale->tipo], tipo_nemico_nome[giocatori[ordgio]->pos_mondoreale->nemico],
     tipo_oggetto_nome[giocatori[ordgio]->pos_mondoreale->oggetto]);
     printf("SOPRASOTTO ");
     printf("Zona %d:\n Tipo: %s, Nemico: %s\n\n",
     giocatori[ordgio]->pos_soprasotto->ord, tipo_zona_nome[giocatori[ordgio]->pos_soprasotto->tipo],
     tipo_nemico_nome[giocatori[ordgio]->pos_soprasotto->nemico]);
     printf("MONDOREALE ");
     printf("Zona %d:\n Tipo: %s, Nemico: %s, Oggetto: %s\n\n", giocatori[ordgio]->pos_mondoreale->avanti->ord,
     tipo_zona_nome[giocatori[ordgio]->pos_mondoreale->avanti->tipo], tipo_nemico_nome[giocatori[ordgio]->pos_mondoreale->avanti->nemico],
     tipo_oggetto_nome[giocatori[ordgio]->pos_mondoreale->avanti->oggetto]);
     printf("SOPRASOTTO ");
     printf("Zona %d:\n Tipo: %s, Nemico: %s\n\n",
     giocatori[ordgio]->pos_soprasotto->avanti->ord, tipo_zona_nome[giocatori[ordgio]->pos_soprasotto->avanti->tipo],
     tipo_nemico_nome[giocatori[ordgio]->pos_soprasotto->avanti->nemico]);
     break;
     case 4: if(fighting==1){
             printf("Assorda il nemico limitandone i movimenti\n(+5atk per il resto del combattimento)\n");
             atkbuff+=5; //per eventuali doppioni
             giocatori[ordgio]->psyatk+=5;}

             else{printf("Da usare in combattimento, oggetto non consumato\n");return;}
     break;
  }
     giocatori[ordgio]->zaino[k]=nessun_oggetto;
 }
}

static void combatti(int ordgio){
if ((giocatori[ordgio]->mondo==0 && giocatori[ordgio]->pos_mondoreale->nemico==nessun_nemico)
  ||(giocatori[ordgio]->mondo==1 && giocatori[ordgio]->pos_soprasotto->nemico==nessun_nemico)){
      printf("Non ci sono nemici\n");}
else if(giocatori[ordgio]->win==1){printf("Hai già battuto il nemico di questa zona\n");}
else{
  int vittorie_giocatore=0;
  int vittorie_nemico=0;
  int enemy, enemyatk, enemydef, enemyfor;
      fighting=1;

      if (giocatori[ordgio]->mondo==0){
    enemy=giocatori[ordgio]->pos_mondoreale->nemico;}
  else if(giocatori[ordgio]->mondo==1){
          enemy=giocatori[ordgio]->pos_soprasotto->nemico;}
      if (enemy==billi)      {enemyatk=8; enemydef=7; enemyfor=6;}
 else if (enemy==democane)   {enemyatk=11;enemydef=9; enemyfor=10;}
 else if (enemy==demotorzone){enemyatk=13;enemydef=13;enemyfor=13;}

printf("\nÈ apparso un %s selvatico! *musichetta dei selvatici di 5°gen*\n\n", tipo_nemico_nome[enemy]);

while(1){
int fight_opt;
  printf("\nAttacchi subiti: %d/%d                            Attacchi andati a segno: %d/%d\n\n",
  vittorie_nemico, giocatori[ordgio]->psydef, vittorie_giocatore, enemydef);
  printf("=========COMBATTI=========\n");
  printf("1)Attacca\n");
  printf("2)Utilizza Oggetto\n");
  if(giocatori[ordgio]->mondo==1){printf("3)Scappa nel Mondo Reale\n");fight_opt=3;}
  else{fight_opt=2;}

  int fight=scancheck(fight_opt);
  switch (fight) {
    case 1: if((rand() % 20)+1+giocatori[ordgio]->psyatk>=(rand() % 20)+1+enemyfor){
           printf("\nAttacco andato a segno\n"); vittorie_giocatore++;}
      else{printf("\nAttacco fallito\n");}
    break;
    case 2: utilizza_oggetto(ordgio);if(empty_check>2){continue;}
    break;
    case 3: cambia_mondo(ordgio); if(giocatori[ordgio]->mondo==0){
      printf("Fuga riuscita\n");
      return;}
    break;
  }

if(vittorie_giocatore>=enemydef){
  if(enemy==demotorzone){
    printf("Congratulazioni %s!\nHai sconfitto il Demotorzone e Vinto la partita.\n", giocatori[ordgio]->nome);

    if(ordgio==undi){strcpy(giocatori[ordgio]->nome, undiwin[0]);
      free(undiwin[0]); undiwin[0]=NULL;
      printf("o forse dovrei dire %s\n", giocatori[ordgio]->nome);}

    if(numround<=round_record[2]){
      int i;
      for(i=2; i>0 && numround<=round_record[i-1]; i--){
        round_record[i]=round_record[i-1];
        strcpy(vincitori[i], vincitori[i-1]);}

      round_record[i]=numround;
      strcpy(vincitori[i], giocatori[ordgio]->nome);}

    torzone_check=0; game_menù=gameopt;
    break;}

  printf("Hai sconfitto %s!\n", tipo_nemico_nome[enemy]);
  giocatori[ordgio]->win=1;
  if(giocatori[ordgio]->mondo==0){if((rand() % 2)==0){giocatori[ordgio]->pos_mondoreale->nemico=nessun_nemico;}}
  else                           {if((rand() % 2)==0){giocatori[ordgio]->pos_soprasotto->nemico=nessun_nemico;}}
  break;}

if((rand() % 20)+1+enemyatk>=(rand() % 20)+1+giocatori[ordgio]->fortuna){
       printf("\nAttacco nemico subito\n");vittorie_nemico++;}
  else{printf("\nAttacco nemico evitato\n");}

if (vittorie_nemico>=giocatori[ordgio]->psydef){
    numgio--;
    if(numgio==0){printf("GAMEOVER\n"); torzone_check=0;} //torzone torzone_check=0 solo per uscire dal while in gioca
    else         {printf("GAMEOVER per %s\n", giocatori[ordgio]->nome);}

    free(giocatori[ordgio]); giocatori[ordgio]=NULL; game_menù=gameopt;
    break;}
}
  if(vittorie_giocatore>=enemydef){
    giocatori[ordgio]->psyatk-=atkbuff;
    giocatori[ordgio]->psydef-=defbuff;}
  atkbuff=0;
  defbuff=0;
  fighting=0;
}
}

void gioca(){
if(mapgen!=1){printf("Impostare il gioco prima di giocare\n");
return;
}
        printf("Nella tranquilla cittadina di Occhinz, famosa per i Waffle Undici\n");
        printf("e per il numero inspiegabilmente alto di biciclette scomparse,\n");
        printf("cominciano ad aprirsi strani portali dimensionali, collegando il\n");
        printf("Mondo Reale a una versione oscura, polverosa, appiccicosa e decisamente\n");
        printf("poco accogliente: il temuto Soprasotto.\n");
        printf("Per sventare la minaccia, dei ragazzi hanno deciso di prendere in mano\n");
        printf("la situazione e andare a fare una visita ai nuovi vicini.\n\n");
for(int i = 0; i<numgio; i++){
  giocatori[i]->pos_mondoreale=prima_mondoreale;
  giocatori[i]->pos_soprasotto=prima_soprasotto;
}

numround=0;
while(torzone_check==1 && numgio!=0){  //i round cominciano da qui
numround++;

turn_ord();
for(int j=0; j<numgio; j++){           //i singoli turni invece da qui

if(torzone_check==0){break;}

int i= *round[j];
while(j<numgio && giocatori[i]==NULL){
  if(numgio!=1){continue;}}

movimento=0;
printf("ROUND %d\n", numround);
printf("Tocca a: %s\n\n", giocatori[i]->nome);

stampa_zona_ingame(i);
do{
gameopt=9;
printf("==========%s==========\n", giocatori[i]->nome);
printf("1)Avanza\n");
printf("2)Indietreggia\n");
printf("3)Cambia Mondo\n");
printf("4)Vedi Statistiche\n");
printf("5)Vedi Zona Attuale\n");
printf("6)Utilizza Oggetto\n");
if (giocatori[i]->mondo==0){printf("%d)Raccogli Oggetto\n", gameopt-2);}
else{gameopt--;}
printf("%d)Combatti\n", gameopt-1);
printf("%d)Passa\n", gameopt);

game_menù=scancheck(gameopt);
switch (game_menù) {
  case 1: avanza(i);
  break;
  case 2: indietreggia(i);
  break;
  case 3: cambia_mondo(i);
  break;
  case 4: stampa_stats(i);
  break;
  case 5: stampa_zona_ingame(i);
  break;
  case 6: utilizza_oggetto(i);
  break;
  case 7: if(giocatori[i]->mondo==1){combatti(i);break;}
  else{raccogli_oggetto(i);
    break;}
  case 8: if(giocatori[i]->mondo==1){}//auto break
  else{combatti(i);
  break;}
  default: break;
}
} while(game_menù!=(gameopt));
 }
}
reset();
}

void termina_gioco(){
  printf("Alla prossima avventurieri!\n"); //bastava lasciare il printf di là ¯\_(ツ)_/¯

  /*se si intendeva salutarli per nome bastava mettere %s e giocatori[i]->nome nel printf con un ciclo for,
    se si vogliono anche includere i morti si crea un array di stringhe durante la character creation
    così si hanno tutti i nomi di chi ha partecipato*/
}

void crediti(){
printf("\nProduced by Alessandro Bellani\n");
printf("\nWritten by Francesco Santini\n\n");

printf("============Highscore Tables============\n");
for(int i=0; i<3; i++){
  if(strlen(vincitori[i])!=0){printf("\n%s:\n%d Rounds\n", vincitori[i],round_record[i]);}

  else{printf("\nDati Insufficienti\n");}
}
}
