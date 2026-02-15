#ifndef GAMELIB_h
#define GAMELIB_h

//non uso typedef pk essendo agli inizi penso mi aiuti vedere per intero le nomenclature, mi riservo la possibilità di usarli poi.
enum Tipo_zona {
  bosco, scuola, laboratorio, caverna, strada, giardino, supermercato, centrale_elettrica, deposito_abbandonato, stazione_polizia
};
enum Tipo_nemico{
  nessun_nemico, billi, democane, demotorzone
};
enum Tipo_oggetto {
  nessun_oggetto, bicicletta, maglietta_fuocoinferno, bussola, schitarrata_metallica
};
struct Zona_mondoreale {
  int ord;
  enum Tipo_zona tipo;
  enum Tipo_nemico nemico;
  enum Tipo_oggetto oggetto;
  struct Zona_mondoreale *avanti;
  struct Zona_mondoreale *indietro;
  struct Zona_soprasotto *link_soprasotto;
};
struct Zona_soprasotto {
  int ord;
  enum Tipo_zona tipo;
  enum Tipo_nemico nemico;
  struct Zona_soprasotto *avanti;
  struct Zona_soprasotto *indietro;
  struct Zona_mondoreale *link_mondoreale;
};
struct Giocatore {
  char nome[25];
  int mondo; //0 reale, 1 sotto
  int win;
  struct Zona_mondoreale *pos_mondoreale;
  struct Zona_soprasotto *pos_soprasotto;
  int psyatk;
  int psydef;
  int fortuna;
  enum Tipo_oggetto zaino[3];
};
void imposta_gioco();
void gioca();
void termina_gioco();
void crediti();
int scancheck(int opt); //pk la uso anche in Exam.c

#endif
