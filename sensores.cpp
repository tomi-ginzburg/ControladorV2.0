#include "sensores.h"

// =====[Declaracion de defines privados]============

#define DEFAULT_VREF    1100    // Valor de referencia de 1.1V en mV

#define SENSOR_CALDERA 13
#define SENSOR_SEGURIDAD 12
#define CANTIDAD_SENSORES 2

// =====[Declaracion de tipos de datos privados]=====

bool flagActivarSensor = false;
bool flagDesactivarSensor = false;

// Cada sensor tiene CANTIDAD_MUESTRAS +1 porque la primera es el promedio de todo el resto
float valorSensores[CANTIDAD_SENSORES][1+CANTIDAD_MUESTRAS];

esp_adc_cal_characteristics_t *adc_chars;

// =====[Declaracion de funciones privadas]==========

float obtenerPromedio(const float *valores);

// =====[Implementacion de funciones publicas]=======

void inicializarSensores(){

    for (int i=0; i<CANTIDAD_SENSORES;i++){
        for (int j=0;j<1+CANTIDAD_MUESTRAS;j++){
            valorSensores[i][j] = 0;
        }
    }

    // Calibro el ADC
    adc_chars = (esp_adc_cal_characteristics_t*) calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_characterize(ADC_UNIT_2, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars);

    // TENSION DEL PUENTE DE WHEATSTONE PARA PT100
    dacWrite(26, 85);
}

void leerDatosSensores(){
    static int numeroMuestra = 1;
    static float Vmaxmed = 2950, Vminmed = 4.05; // Valores en mV
    uint32_t raw;

    if (flagActivarSensor == true){
        dacWrite(26, 85);
        flagActivarSensor = false;
    }

    if (flagDesactivarSensor == true){
        dacDisable(26);
        flagDesactivarSensor = false;
    }
    
    raw = analogRead(SENSOR_CALDERA);
    valorSensores[0][numeroMuestra] = esp_adc_cal_raw_to_voltage(raw, adc_chars);
    valorSensores[0][numeroMuestra] = 0 + (valorSensores[0][numeroMuestra]-Vminmed)*(100)/(Vmaxmed-Vminmed);
    
    raw = analogRead(SENSOR_SEGURIDAD);
    valorSensores[1][numeroMuestra] = esp_adc_cal_raw_to_voltage(raw, adc_chars);

    if(numeroMuestra == CANTIDAD_MUESTRAS){
        numeroMuestra = 1;
    } else{
        numeroMuestra++;
    }
}

void actualizarSensores(){
    valorSensores[0][0] = obtenerPromedio(valorSensores[0]);
    valorSensores[1][0] = obtenerPromedio(valorSensores[1]);
}


void solicitarActivarSensor(){
	flagActivarSensor = true;
}

void solicitarDesactivarSensor(){
	flagDesactivarSensor = true;
}

const float (*leerSensores())[1+CANTIDAD_MUESTRAS]{
    return valorSensores;
}

// =====[Implementacion de funciones privadas]=======

float obtenerPromedio(const float *valores){
    float promedio = 0;
    for (int i=1;i<CANTIDAD_MUESTRAS+1;i++){
        promedio += valores[i];
    }
    return (promedio/CANTIDAD_MUESTRAS);
}