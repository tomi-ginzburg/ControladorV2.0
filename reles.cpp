#include "reles.h"

// =====[Declaracion de defines privados]============

#define CANTIDAD_RELES 4

// =====[Declaracion de tipos de datos privados]=====

int reles[CANTIDAD_RELES] = {RELE_CALENTADOR_1, RELE_CALENTADOR_2_1, RELE_CALENTADOR_2_2, RELE_BOMBA};
bool flagsActivarRele[CANTIDAD_RELES];
bool flagsDesactivarRele[CANTIDAD_RELES];

// =====[Implementacion de funciones publicas]=====

void inicializarReles(){
	pinMode(RELE_CALENTADOR_1,OUTPUT);
	pinMode(RELE_CALENTADOR_2_1,OUTPUT);
	pinMode(RELE_CALENTADOR_2_2,OUTPUT);
	pinMode(RELE_BOMBA,OUTPUT);

	for (int i=0; i<CANTIDAD_RELES; i++){
		flagsActivarRele[i] = false;
	}

	digitalWrite(RELE_CALENTADOR_1,HIGH);
	digitalWrite(RELE_CALENTADOR_2_1,HIGH);
	digitalWrite(RELE_CALENTADOR_2_2,HIGH);
	digitalWrite(RELE_BOMBA,HIGH);
}

void actualizarReles(){
	for (int i=0; i<CANTIDAD_RELES; i++){
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
	for (int i=0; i<CANTIDAD_RELES; i++){
		if (reles[i] == numeroRele){
			flagsActivarRele[i] = true;
		} 
	}
}

void solicitarDesactivarRele(int numeroRele){
	for (int i=0; i<CANTIDAD_RELES; i++){
		if (reles[i] == numeroRele){
			flagsDesactivarRele[i] = true;
		} 
	}
	
}