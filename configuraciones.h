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
    float valorConfiguracion;
} retardo_t;

typedef struct{
    float valor;
    float valorConfiguracion;
} histeresis_t;

typedef struct{
    float minimo;
    float maximo;
    float minimoConfiguracion;
    float maximoConfiguracion;
} limitesSP_t;

typedef struct{
    float valor;
    float valorConfiguracion;
    histeresis_t histeresis;
} setpoint_t;

typedef struct{
    float minimo;
    float maximo;
    float minimoConfiguracion;
    float maximoConfiguracion;
    bool maximoActivo;
    bool minimoActivo;
} alarma_t;

typedef struct{
    int valor[CANT_DIGITOS_INGRESO];
    int ingresado[CANT_DIGITOS_INGRESO];
    int cantidadDigitosIngresados;
    int digitoActual;
} codigo_t;

typedef struct{
    setpoint_t SP[CANT_ETAPAS];
    limitesSP_t limitesSP;
    retardo_t retardoPrendido;
    retardo_t retardoApagado;
    alarma_t alarma;
    bool cambioSensores;
    bool modoDiagnostico;
    codigo_t codigo;
} configuracion_t;

typedef enum{
    IDLE_CONFIGURACION,
    MOSTRAR_PARAMETROS,
    BASICO,
    AVANZADO
} estadoConfiguraciones_t;

typedef enum{
    IDLE_BASICO,
    SP1_ESTABLE,
    SP1_EN_CAMBIO,
    SP2_ESTABLE,
    SP2_EN_CAMBIO,
    SP3_ESTABLE,
    SP3_EN_CAMBIO,
    SP4_ESTABLE,
    SP4_EN_CAMBIO
} estadoMenuBasico_t;

typedef enum{
    IDLE_AVANZADO,
    INGRESANDO,
    INGRESO_INCORRECTO,
    SP1_HISTERESIS_ESTABLE,
    SP1_HISTERESIS_EN_CAMBIO,
    SP2_HISTERESIS_ESTABLE,
    SP2_HISTERESIS_EN_CAMBIO,
    SP3_HISTERESIS_ESTABLE,
    SP3_HISTERESIS_EN_CAMBIO,
    SP4_HISTERESIS_ESTABLE,
    SP4_HISTERESIS_EN_CAMBIO,
    LIMITES_ESPERANDO,
    LIMITE_MAXIMO_EN_CAMBIO,
    LIMITE_MINIMO_EN_CAMBIO,
    RETARDOS_ESPERANDO,
    RETARDO_PRENDIDO_EN_CAMBIO,
    RETARDO_APAGADO_EN_CAMBIO,
    MINIMO_ALARMA_ESPERANDO,
    MINIMO_ALARMA_EN_CAMBIO,
    MINIMO_ALARMA_ACTIVO_EN_CAMBIO,
    MAXIMO_ALARMA_ESPERANDO,
    MAXIMO_ALARMA_EN_CAMBIO,
    MAXIMO_ALARMA_ACTIVO_EN_CAMBIO,
    VERSION_SOFTWARE
} estadoMenuAvanzado_t;

typedef enum{
    IDLE_MP,
    MOSTRAR_SP1,
    MOSTRAR_SP2,
    MOSTRAR_SP3,
    MOSTRAR_SP4,
} estadoMostrarParametros_t;


//=====[Declarations (prototypes) of public functions]=========================

void inicializarConfiguraciones();
void actualizarConfiguraciones();
const estadoConfiguraciones_t* leerEstadoConfiguraciones();
const estadoMenuBasico_t* leerEstadoMenuBasico();
const estadoMenuAvanzado_t* leerEstadoMenuAvanzado();
const estadoMostrarParametros_t* leerEstadoMostrarParametros();
const configuracion_t* leerConfiguraciones();

#endif