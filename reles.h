#ifndef RELES_H
#define RELES_H

#include "Arduino.h"

//=====[Declaration of public defines]=========================================

#define CANT_ETAPAS 4

#define RELE_ETAPA_1 	14
#define RELE_ETAPA_2    25
#define RELE_ETAPA_3    27
#define RELE_ETAPA_4    32

//=====[Declaration of public data types]======================================
extern int reles[CANT_ETAPAS];

//=====[Declarations (prototypes) of public functions]=========================

void inicializarReles();

void actualizarReles();

void solicitarActivarRele(int numeroRele);

void solicitarDesactivarRele(int numeroRele);

#endif