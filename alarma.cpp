#include "alarma.h"

// =====[Declaracion de defines privados]============

#define ALARMA          23
#define FRECUENCIA      2500
#define CANAL_ALARMA    1

// =====[Declaracion de tipos de datos privados]=====

typedef enum {
    ENCENDIDA,
    APAGADA
} estadoAlarma_t;

estadoAlarma_t estadoAlarma;
bool flagActivarAlarma;
bool flagDesactivarAlarma;

// =====[Implementacion de funciones publicas]=====

void inicializarAlarma(){
    ledcSetup(CANAL_ALARMA, FRECUENCIA, 8);
    ledcAttachPin(ALARMA, CANAL_ALARMA);
    ledcWriteTone(CANAL_ALARMA,0);
    estadoAlarma = APAGADA;
}

void solicitarActivarAlarma(){
    flagActivarAlarma = true;
}

void solicitarDesactivarAlarma(){
    flagDesactivarAlarma = true;
}

void actualizarAlarma(){
    switch(estadoAlarma){
        case ENCENDIDA:

            // Actualiza el sonido de la alarma
            if (ledcRead(1) == 0){
                ledcWriteTone(CANAL_ALARMA,FRECUENCIA);
            } else {
                ledcWriteTone(CANAL_ALARMA,0);
            }

            // Actualiza el estado de la alarma
            if (flagDesactivarAlarma == true){
                estadoAlarma = APAGADA;
                ledcWriteTone(CANAL_ALARMA,0);
                flagDesactivarAlarma = false;
            }

            break;
        case APAGADA:

            // Actualiza el estado de la alarma
            if (flagActivarAlarma == true){
                estadoAlarma = ENCENDIDA;
                flagActivarAlarma = false;
            }
            break;
        default: estadoAlarma = APAGADA;
    }

}