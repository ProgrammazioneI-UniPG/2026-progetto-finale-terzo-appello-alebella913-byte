#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "gamelib.h"
int main() {
int option;
srand(time(NULL));
do {
  printf("========MENÙ========\nSelect an option:\n1) Imposta Gioco\n2) Gioca\n3) Termina Gioco\n4) Visualizza Crediti\n");
   option = scancheck(4);
  switch (option) {
    case 1: imposta_gioco();
    break;
    case 2: gioca();
    break;
    case 3: termina_gioco();
    break;
    case 4: crediti();
    break;
    default: printf("Puoi selezionare solo 1,2,3,4\n"); //inutile ma per precauzione
    }
   }
  while(option != 3);
return 0;
}
