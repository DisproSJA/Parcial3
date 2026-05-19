# Parcial DISPRO - Reproductor concurrente de melodías con FSM

## Descripción del proyecto

Este proyecto corresponde al parcial práctico de DISPRO. La idea principal es implementar un sistema en Arduino UNO capaz de reproducir tres melodías usando una Máquina de Estados Finitos, también conocida como FSM.

El sistema está basado en el reto del **Melody Player**, donde una canción se representa como un arreglo de notas. Cada nota tiene dos datos principales:

- La frecuencia de la nota en Hz.
- La duración de la nota en milisegundos.

A diferencia de una implementación tradicional con `delay()` o ciclos `while` bloqueantes, este proyecto usa una FSM no bloqueante. Esto permite que el Arduino pueda actualizar varias melodías al mismo tiempo, sin quedarse esperando a que una canción termine para iniciar la siguiente.

---

## Objetivo del sistema

El objetivo es reproducir tres canciones usando tres salidas diferentes del Arduino. Cada canción tiene su propio buzzer pasivo o piezo conectado a un pin distinto.

El sistema usa una misma lógica de reproducción para las tres canciones, pero cada una tiene su propia instancia del reproductor. Esto permite que cada canción tenga su propio estado, su propia nota actual, sus propios tiempos y su propio pin de salida.

---

## Arquitectura del proyecto

La estructura del proyecto es la siguiente:

```text
Parcial_Dispro/
│
├── Parcial_Dispro.ino
├── melody_player.h
├── melody_player.c
├── himno_alegria.c
├── cancion2.c
└── cancion3.c
```

---

## Descripción de cada archivo

### `Parcial_Dispro.ino`

Este es el archivo principal del proyecto. Es el punto de entrada del sistema y el que une todos los módulos.

**Variables y constantes declaradas:**


| Variable / Constante | Tipo              | Descripción                                                                                             |
| -------------------- | ----------------- | ------------------------------------------------------------------------------------------------------- |
| `PIN_MELODY_1`       | `#define`         | Pin digital 8. Aquí se conecta el buzzer de la primera canción.                                         |
| `PIN_MELODY_2`       | `#define`         | Pin digital 9. Aquí se conecta el buzzer de la segunda canción.                                         |
| `PIN_MELODY_3`       | `#define`         | Pin digital 10. Aquí se conecta el buzzer de la tercera canción.                                        |
| `player1`            | `MELODY_PLAYER_T` | Instancia del reproductor para el Himno de la Alegría. Guarda toda la memoria de estado de esa canción. |
| `player2`            | `MELODY_PLAYER_T` | Instancia del reproductor para la canción 2. Completamente independiente de `player1`.                  |
| `player3`            | `MELODY_PLAYER_T` | Instancia del reproductor para la canción 3. Completamente independiente de las anteriores.             |


Las canciones (`melody1`, `melody2`, `melody3`) y sus tamaños se importan con `extern` desde sus respectivos archivos `.c`, lo que significa que el compilador sabe que están definidas en otro lado.

**Funciones:**

- `setup()`: Inicializa los tres reproductores llamando a `Melody_Player_Init()` para cada uno, pasándole la melodía, el tamaño del arreglo y el pin asignado. Después de esta función, los tres reproductores quedan listos para empezar a reproducir.
- `loop()`: Llama a `Melody_Player_Update()` para los tres reproductores en cada iteración. Como ninguna de esas llamadas bloquea el programa, las tres FSM avanzan en paralelo, logrando así la reproducción concurrente sin necesidad de hilos ni `delay()`.

---

### `melody_player.h`

Es el archivo de cabecera del módulo reproductor. Todo archivo que quiera usar el reproductor debe incluir este header. Acá se concentran todas las definiciones de tipos, estructuras, constantes y prototipos de funciones.

**Constante:**


| Constante      | Valor  | Descripción                                                                                                                                                                                        |
| -------------- | ------ | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `PAUSE_FACTOR` | `0.20` | Factor que multiplica la duración de cada nota para calcular la pausa entre notas. Por ejemplo, si una nota dura 400 ms, la pausa será de 80 ms. Esto evita que las notas suenen pegadas entre sí. |


**Estructura `TONE_T`:**

Representa una nota musical individual. Tiene dos campos:


| Campo    | Tipo           | Descripción                                                                                      |
| -------- | -------------- | ------------------------------------------------------------------------------------------------ |
| `tone`   | `unsigned int` | Frecuencia de la nota en Hz. Por ejemplo, 440 Hz es LA4. Si vale 0, se interpreta como silencio. |
| `length` | `unsigned int` | Duración de la nota en milisegundos. Por ejemplo, 400 ms es una negra a tempo moderado.          |


**Enum `MELODY_STATE_T`:**

Define los cinco estados posibles de la FSM. El reproductor siempre está en uno de estos estados:


| Estado                  | Valor | Descripción                                                                                    |
| ----------------------- | ----- | ---------------------------------------------------------------------------------------------- |
| `PLAYING_NOTES`         | 0     | Estado inicial. Calcula el medio ciclo y la cantidad de ciclos necesarios para la nota actual. |
| `GENERATING_CYCLE`      | 1     | Decide si quedan ciclos por generar. Si sí, pone el pin en HIGH. Si no, pasa a la pausa.       |
| `KEEPING_HIGH_LEVEL`    | 2     | Mantiene el pin en HIGH durante medio periodo de la onda cuadrada.                             |
| `KEEPING_LOW_LEVEL`     | 3     | Mantiene el pin en LOW durante medio periodo. Al terminar, cuenta un ciclo completo.           |
| `PAUSING_BETWEEN_TONES` | 4     | Espera la pausa calculada con `PAUSE_FACTOR` antes de pasar a la siguiente nota.               |


**Estructura `MELODY_PLAYER_T`:**

Esta es la estructura más importante del proyecto. Cada instancia de esta estructura es un reproductor independiente. Sus campos son:


| Campo                 | Tipo             | Descripción                                                                                                 |
| --------------------- | ---------------- | ----------------------------------------------------------------------------------------------------------- |
| `melody`              | `TONE_T *`       | Puntero al arreglo de notas de la canción asignada.                                                         |
| `melody_size`         | `int`            | Número total de notas en la canción. Se usa para saber cuándo reiniciar.                                    |
| `melody_pin`          | `int`            | Pin de salida donde se genera la señal de audio para esta canción.                                          |
| `state`               | `MELODY_STATE_T` | Estado actual de la FSM de este reproductor.                                                                |
| `note`                | `unsigned int`   | Índice de la nota que se está reproduciendo en este momento.                                                |
| `halfcycle_in_micros` | `unsigned long`  | Mitad del periodo de la onda cuadrada actual, en microsegundos. Se calcula como `1000000 / frecuencia / 2`. |
| `max_cycles`          | `unsigned long`  | Número de ciclos completos que debe generar para que la nota dure el tiempo indicado.                       |
| `cycles`              | `unsigned long`  | Contador de ciclos generados hasta el momento para la nota actual.                                          |
| `previous`            | `unsigned long`  | Marca de tiempo de referencia para medir cuánto ha pasado.                                                  |
| `current`             | `unsigned long`  | Marca de tiempo actual. Se compara con `previous` para saber si ya pasó el tiempo requerido.                |


**Prototipos de funciones:**

- `Melody_Player_Init(player, melody, melody_size, melody_pin)`: Prepara un reproductor para empezar a reproducir una canción.
- `Melody_Player_Update(player)`: Avanza un paso la FSM del reproductor. Debe llamarse repetidamente desde el `loop()`.

---

### `melody_player.c`

Este archivo contiene la implementación completa de la FSM. Es el módulo más importante del proyecto porque acá vive toda la lógica de reproducción no bloqueante.

`**Melody_Player_Init(player, melody, melody_size, melody_pin)`**

Esta función recibe un puntero a un `MELODY_PLAYER_T` y lo deja listo para empezar a reproducir. Lo que hace internamente es:

1. Guarda el puntero al arreglo de notas en `player->melody`.
2. Guarda el tamaño total de la canción en `player->melody_size`.
3. Guarda el pin de salida en `player->melody_pin`.
4. Pone el índice `note` en 0 para empezar desde la primera nota.
5. Inicializa `cycles`, `max_cycles` y `halfcycle_in_micros` en 0.
6. Pone `previous` y `current` en 0.
7. Establece el estado inicial de la FSM en `PLAYING_NOTES`.
8. Configura el pin como salida con `pinMode()` y lo deja en `LOW` con `digitalWrite()`.

`**Melody_Player_Update(player)`**

Esta es la función que hace avanzar la FSM. Se llama continuamente desde el `loop()` y cada llamada ejecuta exactamente un estado. El flujo entre estados es el siguiente:

- `**PLAYING_NOTES**`: Revisa si se llegó al final de la canción y si es así reinicia el índice a 0. Luego calcula el `halfcycle_in_micros` con la fórmula `1000000 / frecuencia / 2` y el `max_cycles` con `frecuencia * duración_ms / 1000`. Reinicia el contador `cycles` y pasa al estado `GENERATING_CYCLE`.
- `**GENERATING_CYCLE**`: Compara `cycles` con `max_cycles`. Si todavía faltan ciclos, pone el pin en `HIGH`, guarda el tiempo actual en `previous` y pasa a `KEEPING_HIGH_LEVEL`. Si ya se completaron todos los ciclos, pone el pin en `LOW` y pasa a `PAUSING_BETWEEN_TONES`.
- `**KEEPING_HIGH_LEVEL**`: En cada llamada revisa si ya pasó el tiempo de medio ciclo comparando `current - previous >= halfcycle_in_micros`. Mientras no haya pasado, no hace nada y retorna. Cuando ya pasó, baja el pin a `LOW` y pasa a `KEEPING_LOW_LEVEL`.
- `**KEEPING_LOW_LEVEL**`: Igual que el estado anterior pero con el pin en `LOW`. Cuando ya pasó el medio ciclo, incrementa el contador `cycles` y regresa a `GENERATING_CYCLE` para decidir si hay más ciclos.
- `**PAUSING_BETWEEN_TONES**`: Espera que pase el tiempo de pausa calculado como `length * PAUSE_FACTOR`. Cuando termina, incrementa `note` y vuelve a `PLAYING_NOTES` para procesar la siguiente nota.
- `**default**`: Si por algún error el estado queda con un valor inválido, se reinicia a `PLAYING_NOTES` como medida de seguridad.

---

### `himno_alegria.c`

Contiene la melodía del **Himno de la Alegría** codificada como un arreglo de estructuras `TONE_T`.

**Macros de notas musicales definidas:**


| Macro     | Frecuencia (Hz) | Nota musical |
| --------- | --------------- | ------------ |
| `NOTE_C4` | 262             | Do4          |
| `NOTE_D4` | 294             | Re4          |
| `NOTE_E4` | 330             | Mi4          |
| `NOTE_F4` | 349             | Fa4          |
| `NOTE_G4` | 392             | Sol4         |
| `NOTE_A4` | 440             | La4          |
| `NOTE_B4` | 494             | Si4          |
| `NOTE_C5` | 523             | Do5          |


**Variables declaradas:**


| Variable       | Tipo        | Descripción                                                                                                                                                                                                                                                                  |
| -------------- | ----------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `melody1[]`    | `TONE_T`    | Arreglo de 30 notas que forman el Himno de la Alegría. Cada entrada tiene frecuencia y duración. Todas las notas duran 400 ms excepto tres notas al final de cada estrofa que tienen duraciones de 600 ms, 200 ms y 800 ms respectivamente, para respetar el ritmo original. |
| `melody1_size` | `const int` | Tamaño del arreglo calculado automáticamente con `sizeof(melody1) / sizeof(melody1[0])`. Esto evita tener que contar las notas a mano y hace el código más fácil de mantener.                                                                                                |


La melodía sigue el patrón musical clásico: **E E F G  G F E D  C C D E  E D D**, repetido dos veces con una pequeña variación al final de la segunda estrofa (D C C).

---

### `cancion2.c`

Contiene la segunda melodía del sistema, almacenada en el arreglo `melody2`. Es una canción de ejemplo construida con notas de Do a Do de la cuarta y quinta octava. La secuencia musical sigue un patrón sencillo de cuatro notas que se repite dos veces, seguido de un patrón ascendente de tres notas que también se repite. Las duraciones son de 300 ms para notas cortas y 600 ms para notas largas. Al igual que los demás archivos de canciones, incluye la variable `melody2_size` calculada automáticamente.

---

### `cancion3.c`

Contiene la tercera melodía del sistema en el arreglo `melody3`. También es una canción de ejemplo con notas en la cuarta octava. Su secuencia musical tiene un patrón descendente y luego una subida progresiva, con duraciones mezcladas de 250 ms y 500 ms para dar un ritmo ligeramente diferente a las otras dos canciones. Igual que los demás archivos, termina con el cálculo automático de `melody3_size` para facilitar el mantenimiento del código.