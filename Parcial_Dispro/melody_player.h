#ifndef MELODY_PLAYER_H
#define MELODY_PLAYER_H

#include <Arduino.h>

/*
   ============================================================
   melody_player.h

   Este archivo contiene:
   - La estructura de una nota.
   - La estructura completa de un reproductor.
   - Los estados de la FSM.
   - Los prototipos de las funciones.
   ============================================================
*/

/*
   PAUSE_FACTOR define la pausa entre una nota y la siguiente.

   Ejemplo:
   Si una nota dura 400 ms y PAUSE_FACTOR = 0.20,
   la pausa entre notas será:

   400 * 0.20 = 80 ms
*/
#define PAUSE_FACTOR 0.20

/*
   Estructura de una nota musical.

   tone:
   - Frecuencia en Hz.
   - Ejemplo: 440 Hz corresponde a LA4.

   length:
   - Duración de la nota en milisegundos.
*/
typedef struct {
  unsigned int tone;
  unsigned int length;
} TONE_T;

/*
   Estados de la FSM.

   Estos estados son los estados de la FSM:
   1. Seleccionar nota.
   2. Generar ciclos de la onda.
   3. Mantener nivel HIGH.
   4. Mantener nivel LOW.
   5. Pausar entre notas.
*/
typedef enum {
  PLAYING_NOTES = 0,
  GENERATING_CYCLE,
  KEEPING_HIGH_LEVEL,
  KEEPING_LOW_LEVEL,
  PAUSING_BETWEEN_TONES
} MELODY_STATE_T;

/*
   Estructura del reproductor.
   Cada canción tiene su propio reproductor, propia memoria y propia FSM.
*/
typedef struct {
  TONE_T *melody;                  // Arreglo de notas de la canción.
  int melody_size;                 // Cantidad de notas de la canción.
  int melody_pin;                  // Pin de salida.

  MELODY_STATE_T state;            // Estado actual de la FSM.

  unsigned int note;               // Nota actual.
  unsigned long halfcycle_in_micros; // Medio periodo de la onda.
  unsigned long max_cycles;        // Cantidad máxima de ciclos para la nota.
  unsigned long cycles;            // Ciclos generados hasta el momento.

  unsigned long previous;          // Tiempo de referencia.
  unsigned long current;           // Tiempo actual.
} MELODY_PLAYER_T;

/*
   Inicializa un reproductor de melodía.
*/
void Melody_Player_Init(
  MELODY_PLAYER_T *player,
  TONE_T *melody,
  int melody_size,
  int melody_pin
);

/*
   Actualiza la FSM de un reproductor.
*/
void Melody_Player_Update(MELODY_PLAYER_T *player);

#endif