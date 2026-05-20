#include "melody_player.h"

/*
   ============================================================
   cancion3.c

   Canción 3:
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
TONE_T melody3[] = {
  {NOTE_G4, 250},
  {NOTE_E4, 250},
  {NOTE_E4, 500},

  {NOTE_F4, 250},
  {NOTE_D4, 250},
  {NOTE_D4, 500},

  {NOTE_C4, 250},
  {NOTE_D4, 250},
  {NOTE_E4, 250},
  {NOTE_F4, 250},

  {NOTE_G4, 250},
  {NOTE_G4, 250},
  {NOTE_G4, 500}
};

const int melody3_size = sizeof(melody3) / sizeof(melody3[0]);