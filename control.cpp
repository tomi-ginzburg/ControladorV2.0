#include "control.h"

// =====[Declaracion de defines privados]============

// =====[Declaracion de tipos de datos privados]=====

typedef enum {
    ETAPA_ENCENDIDA,
    ETAPA_APAGADA
} estadoEtapa_t;

estadoControl_t estadoControl = FUNCIONANDO;
estadoEtapa_t estadosEtapas[CANT_ETAPAS];
float contadorEspera = -1;
bool finTemporizador = false;
int cantidadLecturasErroneas[2] = {0,0};

// punteros a variables de otros modulos
const float *temperaturaCalentador = NULL;
const float *temperaturaBomba = NULL;
const float *temperaturaSeguridad = NULL;
const configuracion_t *configuracionesControl = NULL;

// =====[Declaracion de funciones privadas]==========

void actualizarEstadoControl();
void actualizarEtapa(int etapa);
void verificarSensores();
void actualizarContador();

// =====[Implementacion de funciones publicas]=======

void inicializarControles(){
    const float (*valoresSensores)[CANTIDAD_MUESTRAS+1] = leerSensores();
    temperaturaCalentador = &valoresSensores[0][0]; 
    temperaturaBomba = &valoresSensores[1][0]; 
    temperaturaSeguridad = &valoresSensores[2][0]; 

    configuracionesControl = leerConfiguraciones();

    // inicializacion de las etapas apagadas
    for (int i=0;i<CANT_ETAPAS;i++){
        estadosEtapas[i] = ETAPA_APAGADA;
    }

}

void actualizarControles(){

    actualizarContador();
    actualizarEstadoControl();

    switch (estadoControl){
        case CAMBIO_SENSORES:
        case DIAGNOSTICO:
        case CONTROL_APAGADO: 
            for (int i=0;i<CANT_ETAPAS;i++){
                solicitarDesactivarRele(reles[i]);
                estadosEtapas[i] = ETAPA_APAGADA;
            }
            break;
        case FUNCIONANDO:
            verificarSensores();
            for (int i=0;i<CANT_ETAPAS;i++){
                if (configuracionesControl->SP[i].valor > 0){
                    actualizarEtapa(i);
                }
            }
            break;
        default: estadoControl = CONTROL_APAGADO;
    }
}

const estadoControl_t* leerEstadoControl(){
    return &estadoControl;
}

// =====[Implementacion de funciones privadas]=======

void actualizarEstadoControl(){

    switch (estadoControl){
        case CONTROL_APAGADO: break;
        case FUNCIONANDO: 
            if (configuracionesControl->cambioSensores == true){
                estadoControl = CAMBIO_SENSORES;
            }
            if (configuracionesControl->modoDiagnostico == true){
                estadoControl = DIAGNOSTICO;
            }
            break;
        case CAMBIO_SENSORES: 
            if (configuracionesControl->cambioSensores == false){
                estadoControl = FUNCIONANDO;
                solicitarActivarSensor();
            }
            if (configuracionesControl->modoDiagnostico == true){
                estadoControl = DIAGNOSTICO;
            }
            break;
        case DIAGNOSTICO: 
            if (configuracionesControl->modoDiagnostico == false){
                estadoControl = FUNCIONANDO;
            }
            if (configuracionesControl->cambioSensores == true){
                estadoControl = CAMBIO_SENSORES;
            }
            break;
        default: CONTROL_APAGADO;
    }
}

void actualizarEtapa(int etapa){
    switch (estadosEtapas[etapa]){
        case ETAPA_APAGADA:
                if (*temperaturaCalentador < configuracionesControl->SP[etapa].valor - configuracionesControl->SP[etapa].histeresis.valor){
                    switch (etapa){
                        case 1:
                            if (finTemporizador == true){
                                finTemporizador = false;
                                solicitarActivarRele(reles[etapa]);
                                estadosEtapas[etapa] = ETAPA_ENCENDIDA;
                            }
                            break;
                        case 0:
                            // inicio el contador
                            if (configuracionesControl->retardoPrendido.valor == 0){
                                finTemporizador = true;
                            } else {
                                contadorEspera = configuracionesControl->retardoPrendido.valor - 1;
                            }
                            
                        case 2:
                        case 3:
                            solicitarActivarRele(reles[etapa]);
                            estadosEtapas[etapa] = ETAPA_ENCENDIDA;
                            break;
                    }
                }
            break;
        case ETAPA_ENCENDIDA:
                if (*temperaturaCalentador > configuracionesControl->SP[etapa].valor){
                    switch (etapa){
                        case 1:
                            if (finTemporizador == true){
                                finTemporizador = false;
                                solicitarDesactivarRele(reles[etapa]);
                                estadosEtapas[etapa] = ETAPA_APAGADA;
                            }
                            break;
                        case 0:
                            // inicio el contador
                            if (configuracionesControl->retardoApagado.valor == 0){
                                finTemporizador = true;
                            } else {
                                contadorEspera = configuracionesControl->retardoApagado.valor - 1;
                            }
                        case 2:
                        case 3:
                            solicitarDesactivarRele(reles[etapa]);
                            estadosEtapas[etapa] = ETAPA_APAGADA;
                            break;

                    }
                }
            break;
        default:  estadoControl = CONTROL_APAGADO;
    }
}

void verificarSensores(){
    // if (*temperaturaBomba == -127 ){
    //     cantidadLecturasErroneas[1]++;
    //     if (cantidadLecturasErroneas[1] >= 10){
    //         estadoControl = CONTROL_APAGADO;
    //     }
    // } else { 
    //     cantidadLecturasErroneas[1] = 0;
    // }
    
    if (*temperaturaSeguridad > 500){
        cantidadLecturasErroneas[0]++;
        if (cantidadLecturasErroneas[0] >= 5){
            estadoControl = CAMBIO_SENSORES;
            solicitarDesactivarSensor();
            cantidadLecturasErroneas[0] = 0;
        }
    } else { 
        cantidadLecturasErroneas[0] = 0;
    }

}

void actualizarContador(){
    if (contadorEspera > -1){
        if (contadorEspera == 0){
            finTemporizador = true;
        }
        contadorEspera--;
    }
}