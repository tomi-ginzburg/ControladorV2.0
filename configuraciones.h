#ifndef _CONFIGURACIONES_H_
#define _CONFIGURACIONES_H_

#include "Arduino.h"
#include "botones.h"
#include "control.h"
#include <Preferences.h>

//=====[Declaration of public defines]=========================================

#define CANT_DIGITOS_INGRESO        4

//=====[Declaration of public data types]======================================

typedef struct{
    float valor;
    float minimo;
    float maximo;
    float valorConfiguracion;
    float minimoConfiguracion;
    float maximoConfiguracion;
} setpoint_t;

typedef struct{
    float valor;
    float valorConfiguracion;
} retardo_t;

typedef struct{
    float valor;
    float valorConfiguracion;
} histeresis_t;

typedef struct{
    int valor[CANT_DIGITOS_INGRESO];
    int ingresado[CANT_DIGITOS_INGRESO];
    int cantidadDigitosIngresados;
    int digitoActual;
} codigo_t;

typedef struct{
    setpoint_t SP1;
    setpoint_t SP2;
    codigo_t codigo;
    retardo_t retardoPrendido;
    retardo_t retardoApagado;
    histeresis_t histeresis1;
    histeresis_t histeresis2;
    bool cambioSensores;
    bool modoDiagnostico;
} configuracion_t;

typedef enum{
    IDLE_CONFIGURACION,
    MOSTRAR_PARAMETROS,
    MOSTRAR_SENSORES,
    BASICO,
    AVANZADO
} estadoConfiguraciones_t;

typedef enum{
    IDLE_BASICO,
    SP1_ESTABLE,
    SP1_EN_CAMBIO,
    SP2_ESTABLE,
    SP2_EN_CAMBIO
} estadoMenuBasico_t;

typedef enum{
    IDLE_AVANZADO,
    INGRESANDO,
    INGRESO_INCORRECTO,
    SP1_ESPERANDO,
    SP1_MAX_EN_CAMBIO,
    SP1_MIN_EN_CAMBIO,
    SP2_ESPERANDO,
    SP2_MAX_EN_CAMBIO,
    SP2_MIN_EN_CAMBIO,
    HISTERESIS_ESPERANDO,
    H1_EN_CAMBIO,
    H2_EN_CAMBIO,
    RETARDOS_ESPERANDO,
    RETARDO_PRENDIDO_EN_CAMBIO,
    RETARDO_APAGADO_EN_CAMBIO,
    CONFIG_SENSORES,
    MODO_DIAGNOSTICO,
    VERSION_SOFTWARE
} estadoMenuAvanzado_t;

typedef enum{
    IDLE_MP,
    MOSTRAR_SP1,
    MOSTRAR_SP2
} estadoMostrarParametros_t;

typedef enum{
    IDLE_MS,
    MOSTRAR_SENSOR2
} estadoMostrarSensores_t;

//=====[Declarations (prototypes) of public functions]=========================

void inicializarConfiguraciones();
void actualizarConfiguraciones();
const estadoConfiguraciones_t* leerEstadoConfiguraciones();
const estadoMenuBasico_t* leerEstadoMenuBasico();
const estadoMenuAvanzado_t* leerEstadoMenuAvanzado();
const estadoMostrarParametros_t* leerEstadoMostrarParametros();
const estadoMostrarSensores_t* leerEstadoMostrarSensores();
const configuracion_t* leerConfiguraciones();

#endif