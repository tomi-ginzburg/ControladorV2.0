#include "buzzer.h"

// =====[Declaracion de defines privados]============

#define BUZZER          23
#define FRECUENCIA      2500
#define CANAL_BUZZER    1
#define TIEMPO_ALARMA   100 // Es 100 veces el tiempo con el loop que se ejecuta actualizarAlarma
#define TIEMPO_BOCINA   4

// =====[Declaracion de tipos de datos privados]=====

typedef enum {
    BOCINA_ENCENDIDA,
    ALARMA_ENCENDIDA,
    APAGADO
} estadoBuzzer_t;

estadoBuzzer_t estadoBuzzer;
bool flagActivarAlarma;
bool flagDesactivarAlarma;
bool flagActivarBocina;

// =====[Implementacion de funciones publicas]=====

void inicializarBuzzer(){
    ledcSetup(CANAL_BUZZER, FRECUENCIA, 8);
    ledcAttachPin(BUZZER, CANAL_BUZZER);
    ledcWriteTone(CANAL_BUZZER,0);
    estadoBuzzer = APAGADO;
}

void solicitarActivarAlarma(){
    flagActivarAlarma = true;
}

void solicitarDesactivarAlarma(){
    flagDesactivarAlarma = true;
}

void solicitarActivarBocina(){
    flagActivarBocina = true;
}

void actualizarBuzzer(){
    Serial.println(estadoBuzzer);
    switch(estadoBuzzer){
        case APAGADO:

            // Actualiza el estado del buzzer
            if (flagActivarBocina == true){
                ledcWriteTone(CANAL_BUZZER,FRECUENCIA);
                estadoBuzzer = BOCINA_ENCENDIDA;
                flagActivarBocina = false;
            }

            if (flagActivarAlarma == true){
                estadoBuzzer = ALARMA_ENCENDIDA;
                flagActivarAlarma = false;
            }

            break;

        case BOCINA_ENCENDIDA:
            static int contadorBocina = 1;
            if (contadorBocina == TIEMPO_BOCINA){
                ledcWriteTone(CANAL_BUZZER,0);
                estadoBuzzer = APAGADO;
                contadorBocina = 1;
            } else{
                contadorBocina++;
            }
            break;

        case ALARMA_ENCENDIDA:
            static int contadorAlarma = 1;
            if (contadorAlarma==TIEMPO_ALARMA){
                // Actualiza el sonido de la buzzer
                if (ledcRead(1) == 0){
                    ledcWriteTone(CANAL_BUZZER,FRECUENCIA);
                } else {
                    ledcWriteTone(CANAL_BUZZER,0);
                }
                contadorAlarma = 1;
            } else{
                contadorAlarma++;
            }
            
            // Actualiza el estado del buzzer
            if (flagDesactivarAlarma == true){
                estadoBuzzer = APAGADO;
                ledcWriteTone(CANAL_BUZZER,0);
                flagDesactivarAlarma = false;
            }

            break;
        default: estadoBuzzer = APAGADO;
    }

}