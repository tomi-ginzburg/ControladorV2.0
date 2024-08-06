#include "control.h"

// =====[Declaracion de defines privados]============

#define TEMP_SEGURIDAD  500

// =====[Declaracion de tipos de datos privados]=====

typedef enum {
    ETAPA_ENCENDIDA,
    ETAPA_APAGADA
} estadoEtapa_t;

estadoControl_t estadoControl;
estadoEtapa_t estadosEtapas[CANT_ETAPAS];
float contadorEspera = -1;
bool finTemporizador = false;

// punteros a variables de otros modulos
const float *temperaturaCalentador = NULL;
const float *temperaturaSeguridad = NULL;
const configuracion_t *configuracionesControl = NULL;

// =====[Declaracion de funciones privadas]==========

void actualizarEstadoControl();
void actualizarEtapas();
void actualizarEtapa(int etapa);
bool verificarSensor();
void actualizarContador();

// =====[Implementacion de funciones publicas]=======

void inicializarControles(){

    // Punteros a variables de otros modulos
    const float (*valoresSensores)[CANTIDAD_MUESTRAS+1] = leerSensores();
    temperaturaCalentador = &valoresSensores[0][0]; 
    temperaturaSeguridad = &valoresSensores[1][0]; 
    configuracionesControl = leerConfiguraciones();

    // Inicializacion del control funcionando y de las etapas apagadas
    estadoControl = FUNCIONANDO;
    for (int i=0;i<CANT_ETAPAS;i++){
        estadosEtapas[i] = ETAPA_APAGADA;
    }

}

void actualizarControles(){

    actualizarContador();
    actualizarEstadoControl();
    actualizarEtapas();

}

const estadoControl_t* leerEstadoControl(){
    return &estadoControl;
}

// =====[Implementacion de funciones privadas]=======

void actualizarEstadoControl(){

    switch (estadoControl){
        case CONTROL_APAGADO: break;

        case FUNCIONANDO: 
            // Verifica si el sensor esta leyendo valores correctos
            if (verificarSensor() == false){
                estadoControl = CAMBIO_SENSORES;
                solicitarDesactivarSensor();
            }

            // Se verifica la alarma
            if ((configuracionesControl->alarma.maximoActivo == true && *temperaturaCalentador > configuracionesControl->alarma.maximo)
                || (configuracionesControl->alarma.minimoActivo == true && *temperaturaCalentador < configuracionesControl->alarma.minimo)){
                estadoControl = ALARMA;
                solicitarActivarAlarma();
            }
            break;

        case CAMBIO_SENSORES: 
            // Si se cambia desde las configuraciones se vuelve al modo FUNCIONANDO
            if (configuracionesControl->cambioSensores == false){
                estadoControl = FUNCIONANDO;
                solicitarActivarSensor();
            }
            break;

        case ALARMA:
            // Se vuelve al modo FUNCIONANDO en caso de que la temperatura vuelva a los limites o se apague la alarma en las configuraciones
            if ((configuracionesControl->alarma.maximoActivo == true && *temperaturaCalentador < configuracionesControl->alarma.maximo)
                || (configuracionesControl->alarma.minimoActivo == true && *temperaturaCalentador > configuracionesControl->alarma.minimo)
                || (configuracionesControl->alarma.minimoActivo == false && *temperaturaCalentador <= configuracionesControl->alarma.minimo)
                || (configuracionesControl->alarma.maximoActivo == false && *temperaturaCalentador >= configuracionesControl->alarma.maximo)){
                estadoControl = FUNCIONANDO;
                solicitarDesactivarAlarma();
            }
            
            if (verificarSensor() == false){
                estadoControl = CAMBIO_SENSORES;
                solicitarDesactivarSensor();
            }
            break;

        default: estadoControl = CONTROL_APAGADO;
    }
}

void actualizarEtapas(){
    switch (estadoControl){
    case CAMBIO_SENSORES:
    case CONTROL_APAGADO: 
        for (int i=0;i<CANT_ETAPAS;i++){
            solicitarDesactivarRele(reles[i]);
            estadosEtapas[i] = ETAPA_APAGADA;
        }
        break;

    case FUNCIONANDO:
        for (int i=0;i<CANT_ETAPAS;i++){
            // Actualizo unicamente las que tienen valor > 0
            if (configuracionesControl->SP[i].valor > 0){
                actualizarEtapa(i);
            } else if (estadosEtapas[i] == ETAPA_ENCENDIDA){
                solicitarDesactivarRele(reles[i]);
                estadosEtapas[i] == ETAPA_APAGADA;
            }
        }
        break;

    case ALARMA: 
        // Desactiva las primeras 3 etapas
        for (int i=0;i<CANT_ETAPAS-2;i++){
            solicitarDesactivarRele(reles[i]);
            estadosEtapas[i] = ETAPA_APAGADA;
        }
        
        // Activa la ultima etapa
        solicitarActivarRele(reles[CANT_ETAPAS-1]);
        estadosEtapas[CANT_ETAPAS-1] = ETAPA_ENCENDIDA;

        break;

    default: estadoControl = CONTROL_APAGADO;
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

    }
}

bool verificarSensor(){
    bool funcionamientoCorrecto = true;
    static int cantidadLecturasErroneas = 0;
    
    // Si la temperaturar de seguridad esta por encima del limite de seguridad por mas de 5 lecturas consecutivas se pasa al estado CAMBIO_SENSORES
    if (*temperaturaSeguridad > TEMP_SEGURIDAD){
        cantidadLecturasErroneas++;
        if (cantidadLecturasErroneas >= 5){
            funcionamientoCorrecto = false;
            cantidadLecturasErroneas = 0;
        }
    } else { 
        cantidadLecturasErroneas = 0;
    }
    return funcionamientoCorrecto;
}

void actualizarContador(){
    if (contadorEspera > -1){
        if (contadorEspera == 0){
            finTemporizador = true;
        }
        contadorEspera--;
    }
}