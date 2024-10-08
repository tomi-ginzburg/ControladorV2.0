#ifndef _CONTROL_H_
#define _CONTROL_H_

#include "Arduino.h"
#include "sensores.h"
#include "reles.h"
#include "configuraciones.h"
#include "buzzer.h"

// =====[Declaracion de tipos de datos publicos]=====

typedef enum {
    CONTROL_APAGADO,
    FUNCIONANDO,
    CAMBIO_SENSORES,
    ALARMA
} estadoControl_t;


// =====[Declaracion de funciones publicas]==========

void inicializarControles();

void actualizarControles();

const estadoControl_t* leerEstadoControl();

#endif