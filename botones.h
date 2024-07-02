#ifndef _BOTONES_H_
#define _BOTONES_H_

#include "Arduino.h"

// =====[Declaracion de defines publicos]============

#define CANTIDAD_BOTONES    4
#define INDICE_ENTER        0
#define INDICE_ATRAS        1
#define INDICE_MAS          2
#define INDICE_MENOS        3

// =====[Declaracion de tipos de datos publicos]=====

typedef enum{
    NO_PRESIONADO,
    BAJANDO,
    PRESIONADO,
    SUBIENDO
} estadoCambio_t;

typedef struct{
    int pin;
    estadoCambio_t estado;
    unsigned int tiempoCambioMs;
    unsigned int tiempoPresionadoMs;
} boton_t;

// =====[Declaracion de funciones publicas]==========

void inicializarBotones();
void actualizarBotones();
const boton_t* leerBotones();

#endif