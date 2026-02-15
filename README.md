[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/5fsIc7xe)
# Progetto-finale-2025-Cosestrane
Progetto finale Programmazione Procedurale UniPG Informatica

## Nome: Alessandro

## Cognome: Bellani

## Matricola: 396528

## Commenti/modifiche al progetto:
Campo aggiuntivo in struct giocatore (win) per determinare se sì è battuto un nemico nel turno precedente e quindi permettere il movimento anche in caso di respawn del nemico post sconfitta.
Azzerata col movimento

funzione personalizzata int scancheck(int opt) che prende input da 1 a opt e stampa errore in caso contrario lascia \n nel buffer quindi se si immette un input sbagliato poi bisogna inviare 2 volte).
Non è static perchè la uso anche in main

cancella mappa(), non richesta ma utile

reset(), per deallocare tutto prima di riselezionare imposta gioco o quando si vince/si fa gameover

statistiche limitate all intervallo 1-20 anche con i buff in character creation ma non con gli oggetti

memoria allocata per ricordare vero nome di undicivirgolacinque in caso di inserimento nel registro vittorie

funzione turn_ord() decide l ordine dello svolgimento dei turni

movimenti stampano automaticamente la zona attuale

raccogli_oggetto() non manda messaggi d errore se si ha l inventario pieno ma sostituisce il campo oggetto della zona col proprio e viceversa (mi pareva più carino così ci si possono condividere gli oggetti a vicenda se si volesse collaborare)(manda comunque errore se la zona non ha oggetti)

combatti()lancia d20 in attacco e in difesa e aggiunge psyatk all offensiva e fortuna alla difensiva, difesa sono i colpi incassabili

