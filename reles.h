#ifndef _RELES_H
#define _RELES_H

#include "Arduino.h"

//=====[Declaration of public defines]=========================================

#define RELE_CALENTADOR_1 	14
#define RELE_CALENTADOR_2_1 25
#define RELE_CALENTADOR_2_2 27
#define RELE_BOMBA			32

//=====[Declaration of public data types]======================================

//=====[Declarations (prototypes) of public functions]=========================

void inicializarReles();

void actualizarReles();

void solicitarActivarRele(int numeroRele);

void solicitarDesactivarRele(int numeroRele);

#endif