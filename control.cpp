#include "control.h"

// =====[Declaracion de defines privados]============

// =====[Declaracion de tipos de datos privados]=====

typedef enum {
    APAGADO_CALENTADOR,
    PRENDIDO_MEDIO,
    PRENDIDO_COMPLETO
} estadoControlCalentador_t;

typedef enum {
    APAGADO_BOMBA,
    PRENDIDO_BOMBA
} estadoControlBomba_t;


estadoControl_t estadoControl = FUNCIONANDO;
estadoControlCalentador_t estadoControlCalentador = APAGADO_CALENTADOR;
estadoControlBomba_t estadoControlBomba = APAGADO_BOMBA;
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
void actualizarControlCalentador();
void actualizarControlBomba();
void verificarSensores();
void actualizarContador();

// =====[Implementacion de funciones publicas]=======

void inicializarControles(){
    const float *valoresSensores = leerSensores();
    temperaturaCalentador = &valoresSensores[0]; 
    temperaturaBomba = &valoresSensores[1]; 
    temperaturaSeguridad = &valoresSensores[2]; 

    configuracionesControl = leerConfiguraciones();

}

void actualizarControles(){

    actualizarContador();
    verificarSensores();

    actualizarEstadoControl();

    switch (estadoControl){
        case CAMBIO_SENSORES:
        case DIAGNOSTICO:
        case CONTROL_APAGADO: 
            solicitarDesactivarRele(RELE_CALENTADOR_1);
            solicitarDesactivarRele(RELE_CALENTADOR_2_1);
            solicitarDesactivarRele(RELE_CALENTADOR_2_2);
            solicitarDesactivarRele(RELE_BOMBA);
            estadoControlCalentador = APAGADO_CALENTADOR;
            estadoControlBomba = APAGADO_BOMBA;
            break;
        case FUNCIONANDO:
            if (configuracionesControl->SP1.valor > 0){
                actualizarControlCalentador();
            }
            if (configuracionesControl->SP2.valor > 0){
                actualizarControlBomba();
            }
            break;
        default: estadoControl = CONTROL_APAGADO;
    }
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

// Si se pierde el estado de la FSM se apaga
void actualizarControlCalentador(){
    switch (estadoControlCalentador){
        case APAGADO_CALENTADOR: 
        
            // si estaba apagado pero bajo de SP1 - HISTERESIS1: 
            // se activa el RELE_CALENTADOR_1, se inicia el timerCalentador y se cambia a PRENDIDO_MEDIO
            if (*temperaturaCalentador < configuracionesControl->SP1.valor - configuracionesControl->histeresis1.valor){
                solicitarActivarRele(RELE_CALENTADOR_1);
                finTemporizador = false;
                contadorEspera =  configuracionesControl->retardoPrendido.valor;
                estadoControlCalentador = PRENDIDO_MEDIO;
            }
        
            break;     
        case PRENDIDO_MEDIO: 
            if ((*temperaturaCalentador > configuracionesControl->SP1.valor) && (finTemporizador == true)){
                solicitarDesactivarRele(RELE_CALENTADOR_1);
                finTemporizador = false;
                estadoControlCalentador = APAGADO_CALENTADOR;
            }
            if ((*temperaturaCalentador < configuracionesControl->SP1.valor - configuracionesControl->histeresis2.valor) && (finTemporizador == true)){
                solicitarActivarRele(RELE_CALENTADOR_2_1);
                solicitarActivarRele(RELE_CALENTADOR_2_2);
                finTemporizador = false;
                estadoControlCalentador = PRENDIDO_COMPLETO;
            }
        
            break;
        case PRENDIDO_COMPLETO: 
            
            if (*temperaturaCalentador > configuracionesControl->SP1.valor){
                solicitarDesactivarRele(RELE_CALENTADOR_2_1);
                solicitarDesactivarRele(RELE_CALENTADOR_2_2);
                finTemporizador = false;
                contadorEspera = configuracionesControl->retardoApagado.valor;
                estadoControlCalentador = PRENDIDO_MEDIO;
            }

            break;
        default: estadoControlCalentador = APAGADO_CALENTADOR; break;
    }
}

void actualizarControlBomba(){
    switch (estadoControlBomba){
        case APAGADO_BOMBA:
                if (*temperaturaBomba < configuracionesControl->SP2.valor - configuracionesControl->histeresis1.valor){
                    solicitarActivarRele(RELE_BOMBA);
                    estadoControlBomba = PRENDIDO_BOMBA;
                }
            break;
        case PRENDIDO_BOMBA:
                if (*temperaturaBomba > configuracionesControl->SP2.valor){
                    solicitarDesactivarRele(RELE_BOMBA);
                    estadoControlBomba = APAGADO_BOMBA;
                }
            break;
        default: estadoControlBomba = APAGADO_BOMBA; break;
    }
}

void verificarSensores(){
    if (*temperaturaBomba == -127 ){
        cantidadLecturasErroneas[1]++;
        if (cantidadLecturasErroneas[1] >= 10){
            estadoControl = CONTROL_APAGADO;
        }
    } else { 
        cantidadLecturasErroneas[1] = 0;
    }
    
    if (*temperaturaSeguridad > 500){
        cantidadLecturasErroneas[0]++;
        if (cantidadLecturasErroneas[0] >= 10){
            estadoControl = CONTROL_APAGADO;
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