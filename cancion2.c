#include "melody_player.h"

/*
   ============================================================
   cancion2.c

   Canción 2:
   ...

   Formato de cada nota:
   { frecuencia_en_Hz, duracion_en_ms }
   ============================================================
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
   Canción de ejemplo.
*/
TONE_T melody2[] = {
  {NOTE_C4, 300},
  {NOTE_D4, 300},
  {NOTE_E4, 300},
  {NOTE_C4, 300},

  {NOTE_C4, 300},
  {NOTE_D4, 300},
  {NOTE_E4, 300},
  {NOTE_C4, 300},

  {NOTE_E4, 300},
  {NOTE_F4, 300},
  {NOTE_G4, 600},

  {NOTE_E4, 300},
  {NOTE_F4, 300},
  {NOTE_G4, 600}
};

const int melody2_size = sizeof(melody2) / sizeof(melody2[0]);