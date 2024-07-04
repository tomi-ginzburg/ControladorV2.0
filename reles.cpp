#include "reles.h"

// =====[Declaracion de defines privados]============


// =====[Declaracion de tipos de datos privados]=====

int reles[CANT_ETAPAS] = {RELE_ETAPA_1, RELE_ETAPA_2, RELE_ETAPA_3, RELE_ETAPA_4};

bool flagsActivarRele[CANT_ETAPAS];
bool flagsDesactivarRele[CANT_ETAPAS];

// =====[Implementacion de funciones publicas]=====

void inicializarReles(){

	for (int i=0; i<CANT_ETAPAS; i++){
		pinMode(reles[i],OUTPUT);
		flagsActivarRele[i] = false;
		digitalWrite(reles[i],HIGH);
	}

}

void actualizarReles(){
	for (int i=0; i<CANT_ETAPAS; i++){
		if (flagsActivarRele[i] == true){
			digitalWrite(reles[i], LOW);
			flagsActivarRele[i] = false;
		}
		if (flagsDesactivarRele[i] == true){
			digitalWrite(reles[i], HIGH);
			flagsDesactivarRele[i] = false;	
		}
	}
}

void solicitarActivarRele(int numeroRele){
	for (int i=0; i<CANT_ETAPAS; i++){
		if (reles[i] == numeroRele){
			flagsActivarRele[i] = true;
		} 
	}
}

void solicitarDesactivarRele(int numeroRele){
	for (int i=0; i<CANT_ETAPAS; i++){
		if (reles[i] == numeroRele){
			flagsDesactivarRele[i] = true;
		} 
	}
	
}