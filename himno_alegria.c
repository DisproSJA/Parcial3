#include "melody_player.h"

/*
   ============================================================
   himno_alegria.c

   Canción 1:
   Himno de la Alegría

   Formato de cada nota:
   { frecuencia_en_Hz, duracion_en_ms }
   ============================================================
*/

/*
   Definición de algunas notas musicales.

   C = Do
   D = Re
   E = Mi
   F = Fa
   G = Sol
   A = La
   B = Si
*/
#define NOTE_C4 262
#define NOTE_D4 294
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_G4 392
#define NOTE_A4 440
#define NOTE_B4 494
#define NOTE_C5 523

/*
   Himno de la Alegría.

   Melodía principal:
   E E F G | G F E D | C C D E | E D D
*/
TONE_T melody1[] = {
  {NOTE_E4, 400},
  {NOTE_E4, 400},
  {NOTE_F4, 400},
  {NOTE_G4, 400},

  {NOTE_G4, 400},
  {NOTE_F4, 400},
  {NOTE_E4, 400},
  {NOTE_D4, 400},

  {NOTE_C4, 400},
  {NOTE_C4, 400},
  {NOTE_D4, 400},
  {NOTE_E4, 400},

  {NOTE_E4, 600},
  {NOTE_D4, 200},
  {NOTE_D4, 800},

  {NOTE_E4, 400},
  {NOTE_E4, 400},
  {NOTE_F4, 400},
  {NOTE_G4, 400},

  {NOTE_G4, 400},
  {NOTE_F4, 400},
  {NOTE_E4, 400},
  {NOTE_D4, 400},

  {NOTE_C4, 400},
  {NOTE_C4, 400},
  {NOTE_D4, 400},
  {NOTE_E4, 400},

  {NOTE_D4, 600},
  {NOTE_C4, 200},
  {NOTE_C4, 800}
};

/*
   Tamaño de la canción.
   Esto calcula automáticamente cuántas notas tiene melody1.
*/
const int melody1_size = sizeof(melody1) / sizeof(melody1[0]);