#include "melody_player.h"

/*
   ============================================================
   melody_player.c

   Este archivo contiene la FSM.
   - Cada llamada a Melody_Player_Update() avanza un paso.
   ============================================================
*/

void Melody_Player_Init(MELODY_PLAYER_T *player, TONE_T *melody, int melody_size, int melody_pin) {
   
   //  Se guardan los datos principales del reproductor.
  player->melody = melody;
  player->melody_size = melody_size;
  player->melody_pin = melody_pin;

  //  El reproductor inicia desde la primera nota.
  player->note = 0;

  //  Al inicio no se ha generado ningún ciclo.
  player->cycles = 0;
  player->max_cycles = 0;
  player->halfcycle_in_micros = 0;

  //  Se inicializan las variables de tiempo.
  player->previous = 0;
  player->current = 0;

  //  Estado inicial de la FSM.
  player->state = PLAYING_NOTES;

  //  Se configura el pin como salida y se deja inicialmente en LOW.
  pinMode(player->melody_pin, OUTPUT);
  digitalWrite(player->melody_pin, LOW);
}

void Melody_Player_Update(MELODY_PLAYER_T *player) {
  //  Esta función funciona como es la FSM.
  switch (player->state) {
    case PLAYING_NOTES: // Estado 0: Seleccionar nota actual
      //  Si ya se llegó al final de la canción, se reinicia desde la nota inicial.
      if (player->note >= player->melody_size) {
        player->note = 0;
      }

      //  Si por error una nota tiene frecuencia 0, se salta.
      //  Esto evita división por cero.
      if (player->melody[player->note].tone == 0) {
        player->current = player->previous = millis();
        player->state = PAUSING_BETWEEN_TONES;
        break;
      }

      /*
         Se calcula el medio ciclo de la onda cuadrada.

         Fórmula:
         periodo = 1 / frecuencia

         En microsegundos:
         periodo_us = 1000000 / frecuencia

         Como se necesita medio periodo en HIGH y medio periodo en LOW:
         halfcycle = periodo_us / 2
      */
      player->halfcycle_in_micros =
        1000000UL / player->melody[player->note].tone / 2;

      /*
         Se calcula cuántos ciclos completos se deben generar
         para que la nota dure el tiempo indicado.

         max_cycles = frecuencia * duración_ms / 1000
      */
      player->max_cycles =
        ((unsigned long)player->melody[player->note].tone *
         (unsigned long)player->melody[player->note].length) / 1000UL;

      
      //Se reinicia el contador de ciclos para la nueva nota.
      player->cycles = 0;

      // Luego se pasa al estado que genera los ciclos de la nota.
      player->state = GENERATING_CYCLE;
      break;

    case GENERATING_CYCLE: // Estado 1: Decide si hay que generar más ciclos de la nota.

      //  Si faltan ciclos, se pone el pin en HIGH y se empieza a contar el tiempo del medio ciclo.
      if (player->cycles < player->max_cycles) {
        digitalWrite(player->melody_pin, HIGH);
        player->current = micros();
        player->previous = player->current;
        player->state = KEEPING_HIGH_LEVEL;
      } else {
        //  Si ya se completaron los ciclos de la nota, se apaga el pin y se pasa a la pausa entre tonos.
        digitalWrite(player->melody_pin, LOW);

        player->current = millis();
        player->previous = player->current;

        player->state = PAUSING_BETWEEN_TONES;
      }
      break;

    case KEEPING_HIGH_LEVEL: // Estado 2: Mantiene el pin en HIGH durante medio ciclo.

      //  Se revisa si ya pasó el tiempo del medio ciclo.
      player->current = micros();

      if (player->current - player->previous >= player->halfcycle_in_micros) {
        //  Cuando ya pasó medio ciclo, se baja el pin a LOW.
        digitalWrite(player->melody_pin, LOW);

        player->current = micros();
        player->previous = player->current;

        player->state = KEEPING_LOW_LEVEL;
      }
      break;

    case KEEPING_LOW_LEVEL: // Estado 3: Mantiene el pin en LOW durante medio ciclo.
      //  Se revisa si ya pasó el tiempo del medio ciclo.
      player->current = micros();

      if (player->current - player->previous >= player->halfcycle_in_micros) { 
        //  Ya se completó un ciclo:
        //  - Medio ciclo HIGH.
        //  - Medio ciclo LOW.
        player->cycles++;

        //  Se vuelve a revisar si se deben generar más ciclos.
        player->state = GENERATING_CYCLE;
      }
      break;

    case PAUSING_BETWEEN_TONES: // Estado 4: Pausa entre notas.
      //  Se revisa si ya pasó el tiempo de la pausa.
      player->current = millis();

      if (player->current - player->previous >=
          (unsigned long)(player->melody[player->note].length * PAUSE_FACTOR)) {
        //  Cuando termina la pausa, se avanza a la siguiente nota.
        player->note++;

        //  Se vuelve al estado inicial para calcular la siguiente nota.
        player->state = PLAYING_NOTES;
      }
      break;

    default:
      //  Si por alguna razón el estado se daña, se reinicia la FSM.
      player->state = PLAYING_NOTES;
      break;
  }
}