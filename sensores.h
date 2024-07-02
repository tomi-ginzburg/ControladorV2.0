#ifndef _SENSORES_H_
#define _SENSORES_H

#include "Arduino.h"
#include <OneWire.h>                
#include <DallasTemperature.h>

// =====[Declaracion de tipos de datos publicos]=====


// =====[Declaracion de funciones publicas]==========

void inicializarSensores();

void leerDatosSensores();

void actualizarSensores();

const float* leerSensores();

#endif