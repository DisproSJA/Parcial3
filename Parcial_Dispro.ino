#include "melody_player.h"
/*
   ============================================================
   Parcial 3 DISPRO
   Reproductor concurrente de 3 melodías usando FSM

   Archivo principal:
   - Inicializa los 3 reproductores.
   - Cada reproductor tiene su propia canción, pin y estado.
   - En el loop se actualizan las 3 FSM una instancia por cada reproductor.
   ============================================================
*/

// Declaración de canciones externas.
// Cada canción está definida en su propio archivo .c.
extern TONE_T melody1[];
extern const int melody1_size;

extern TONE_T melody2[];
extern const int melody2_size;

extern TONE_T melody3[];
extern const int melody3_size;

// Pines donde se conectan los buzzers pasivos o piezos.
#define PIN_MELODY_1 8
#define PIN_MELODY_2 9
#define PIN_MELODY_3 10

// Se crean tres reproductores independientes.
MELODY_PLAYER_T player1;
MELODY_PLAYER_T player2;
MELODY_PLAYER_T player3;

void setup() {
  /*
     Inicialización de cada reproductor.

     Cada player tiene:
     - Una canción diferente.
     - Un tamaño diferente.
     - Un pin diferente.
     - Su propia FSM interna.
  */

  Melody_Player_Init(&player1, melody1, melody1_size, PIN_MELODY_1);
  Melody_Player_Init(&player2, melody2, melody2_size, PIN_MELODY_2);
  Melody_Player_Init(&player3, melody3, melody3_size, PIN_MELODY_3);
}

void loop() {
  Melody_Player_Update(&player1);
  Melody_Player_Update(&player2);
  Melody_Player_Update(&player3);
}