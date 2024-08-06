#ifndef _CONTADOR_H_
#define _CONTADOR_H_

#include "Arduino.h"

// =====[Declaracion de tipos de datos publicos]=====

typedef enum{
    LOOP,
    CONTROL,
    IU
} contador_t;

// =====[Declaracion de funciones publicas]=====

void inicializarContador(uint64_t periodo_ms);
bool contadorIgualA(uint64_t intervalo, contador_t tipoContador);

#endif