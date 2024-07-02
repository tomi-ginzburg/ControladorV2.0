#include "sensores.h"

// =====[Declaracion de defines privados]============

#define SENSOR_CALENTADOR 13
#define SENSOR_BOMBA 33
#define SENSOR_SEGURIDAD 12
#define CANTIDAD_MUESTRAS 10
#define CANTIDAD_SESNORES 3

// =====[Declaracion de tipos de datos privados]=====

// Cada sesnor tiene CANTIDAD_MUESTRAS +1 porque la primera es el promedio de todo el resto
// Para el DS18B20 no uso el promedio, uso directo la medicion por el tiempo de retardo que tiene
float valorSensores[CANTIDAD_SESNORES][1+CANTIDAD_MUESTRAS];
 
OneWire ourWire(SENSOR_BOMBA);  
DallasTemperature sensorDS(&ourWire); 


// =====[Declaracion de funciones privadas]==========

float obtenerPromedio(const float *valores);

// =====[Implementacion de funciones publicas]=======

void inicializarSensores(){
    
    for (int i=0; i<CANTIDAD_SESNORES;i++){
        for (int j=0;j<1+CANTIDAD_MUESTRAS;j++){
            valorSensores[i][j] = 0;
        }
    }

    // CONFIGURACIONES DS18B20
    sensorDS.begin();
    sensorDS.setResolution(12);
    sensorDS.setWaitForConversion(false);
    sensorDS.requestTemperatures();

    // TENSION DEL PUENTE DE WHEATSTONE PARA PT100
    dacWrite(26, 85);
}

void leerDatosSensores(){
    static int numeroMuestra = 1;
    static float sup = 3750, inf = 5, b = (100)/(1-sup/inf), a = (-b)/inf;

    valorSensores[0][numeroMuestra] = analogRead(SENSOR_CALENTADOR);
    valorSensores[0][numeroMuestra] = (a*valorSensores[0][numeroMuestra] + b);

    valorSensores[2][numeroMuestra] = analogRead(SENSOR_SEGURIDAD);
    
    if(numeroMuestra == CANTIDAD_MUESTRAS){
        numeroMuestra = 1;
    } else{
        numeroMuestra++;
    }
}

void actualizarSensores(){
    valorSensores[0][0] = obtenerPromedio(valorSensores[0]);

    valorSensores[1][0] = sensorDS.getTempCByIndex(0);
    sensorDS.requestTemperatures();
    
    valorSensores[2][0] = obtenerPromedio(valorSensores[2]);
}

const float* leerSensores(){
    return (const float*)&valorSensores[0][0];;
}

// =====[Implementacion de funciones privadas]=======

float obtenerPromedio(const float *valores){
    float promedio = 0;
    for (int i=1;i<CANTIDAD_MUESTRAS+1;i++){
        promedio = promedio + valores[i];
    }
    return (promedio/CANTIDAD_MUESTRAS);
}