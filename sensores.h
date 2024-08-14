#ifndef _SENSORES_H_
#define _SENSORES_H

#include "Arduino.h"
#include <OneWire.h>                
#include <DallasTemperature.h>
#include "esp_adc_cal.h"

#define CANTIDAD_MUESTRAS 100

// =====[Declaracion de tipos de datos publicos]=====


// =====[Declaracion de funciones publicas]==========

void inicializarSensores();

void leerDatosSensores();

void actualizarSensores();

void solicitarActivarSensor();

void solicitarDesactivarSensor();

const float (*leerSensores())[1+CANTIDAD_MUESTRAS];

#endif