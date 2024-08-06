#include "contador.h"

// =====[Declaracion de defines privados]=====

// =====[Declaracion de tipos de datos privados]=====

hw_timer_t * timerContador = NULL;

volatile  uint64_t valorContador = 0;
uint64_t ultimoValorLoop = 0;
uint64_t ultimoValorControl = 0;
uint64_t ultimoValorIu = 0;

// =====[Declaracion de funciones privadas]=====

static void IRAM_ATTR handlerTimerContador();

// =====[Implementacion de funciones publicas]======

void inicializarContador(uint64_t periodo_ms){
    timerContador = timerBegin(0, 80, true);
    timerAttachInterrupt(timerContador, &handlerTimerContador, true);
    timerAlarmWrite(timerContador, periodo_ms*1000, true);
    timerAlarmEnable(timerContador);
}


bool contadorIgualA(uint64_t intervalo, contador_t tipoContador){
    bool esIgual = false;
    switch (tipoContador){
        case LOOP:
            if (valorContador - ultimoValorLoop >= intervalo){
            ultimoValorLoop = valorContador;
            esIgual = true;
            }
            break;
        case CONTROL:
            if (valorContador - ultimoValorControl >= intervalo){
            ultimoValorControl = valorContador;
            esIgual = true;
            }
            break;
        case IU:
            if (valorContador - ultimoValorIu >= intervalo){
            ultimoValorIu = valorContador;
            esIgual = true;
            }
            break;
    }
    return esIgual;
}

// =====[Implementacion de funciones privadas]======

static void IRAM_ATTR handlerTimerContador(){
    valorContador++;
}
