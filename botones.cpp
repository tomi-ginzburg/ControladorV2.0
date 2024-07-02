#include "botones.h"

// =====[Declaracion de defines privados]============

#define TIEMPO_CAMBIO_MS    50 
#define DELAY_LOOP_MS       10
#define BOTON_ENTER         18
#define BOTON_ATRAS         4
#define BOTON_MAS           2
#define BOTON_MENOS         15

// =====[Declaracion de tipos de datos privados]=====

boton_t botones[CANTIDAD_BOTONES];

// =====[Declaracion de funciones privadas]==========

void cambioEstadoBoton(void *arg);
bool terminoTiempoCambio(boton_t *boton);
void cambiarEstadoFinTiempo(boton_t *boton);

// =====[Implementacion de funciones publicas]=======

void inicializarBotones(){

    botones[0].pin = BOTON_ENTER;
    botones[1].pin = BOTON_ATRAS;
    botones[2].pin = BOTON_MAS;
    botones[3].pin = BOTON_MENOS;

    for (int i=0; i<CANTIDAD_BOTONES;i++){
        pinMode(botones[i].pin, INPUT_PULLDOWN);
        
        if (digitalRead(botones[i].pin) == LOW){
            botones[i].estado = NO_PRESIONADO;
        } else{
            botones[i].estado = PRESIONADO;
        }
        
        botones[i].tiempoCambioMs = 0;
        botones[i].tiempoPresionadoMs = 0;
        attachInterruptArg(botones[i].pin, cambioEstadoBoton, &botones[i], CHANGE);
    }
}

void actualizarBotones(){
    for (int i=0;i<CANTIDAD_BOTONES;i++){
        switch (botones[i].estado){
            case NO_PRESIONADO:
                break;
            case BAJANDO:
                if (terminoTiempoCambio(&botones[i])){
                    cambiarEstadoFinTiempo(&botones[i]);
                } else {
                    botones[i].tiempoCambioMs = botones[i].tiempoCambioMs + DELAY_LOOP_MS; 
                }
                break;
            case PRESIONADO:
                botones[i].tiempoPresionadoMs = botones[i].tiempoPresionadoMs + DELAY_LOOP_MS;

                break;
            case SUBIENDO:
                if (terminoTiempoCambio(&botones[i])){
                    cambiarEstadoFinTiempo(&botones[i]);
                } else {
                    botones[i].tiempoCambioMs = botones[i].tiempoCambioMs + DELAY_LOOP_MS; 
                }
                break;
        }
    }
}


const boton_t* leerBotones(){
    return botones;
}

// =====[Implementacion de funciones privadas]=======

void cambioEstadoBoton(void *arg){
    boton_t *boton = static_cast<boton_t *>(arg);
    if (boton->estado == NO_PRESIONADO){
        boton->estado = BAJANDO;
        boton->tiempoPresionadoMs = 0;
    } else if (boton->estado == PRESIONADO){
        boton->estado = SUBIENDO;
    }
}

bool terminoTiempoCambio(boton_t *boton){
    return (boton->tiempoCambioMs >= TIEMPO_CAMBIO_MS);
}

void cambiarEstadoFinTiempo(boton_t *boton){
    if (digitalRead(boton->pin) == LOW){
        boton->estado = NO_PRESIONADO;
    } else {
        boton->estado = PRESIONADO;
    }
    boton->tiempoCambioMs = 0;
}
