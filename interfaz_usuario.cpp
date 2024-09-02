#include "interfaz_usuario.h"


#define TIEMPO_TITILAR  10

// =====[Declaracion de tipos de datos privados]=====

LiquidCrystal_I2C lcd(0x27,16,2);

// Punteros a datos de otros modulos
const float *temperaturaCalentadorIU = NULL;
const configuracion_t *configuraciones = NULL;
const estadoControl_t *estadoControlIU = NULL;
const estadoConfiguraciones_t *estadoConfiguracionesIU = NULL;
const estadoMenuBasico_t *estadoMenuBasicoIU = NULL;
const estadoMenuAvanzado_t *estadoMenuAvanzadoIU = NULL;
const estadoMostrarParametros_t *estadoMostrarParametrosIU = NULL;

// Variables para el registro de los estado anteriores

estadoControl_t estadoControlAnteriorIU;
estadoConfiguraciones_t estadoConfiguracionesAnteriorIU;
estadoMenuBasico_t estadoMenuBasicoAnteriorIU;
estadoMenuAvanzado_t estadoMenuAvanzadoAnteriorIU;
estadoMostrarParametros_t estadoMostrarParametrosAnteriorIU;

bool primeraEntradaEstado = false;

// =====[Declaracion de funciones privadas]==========

void actualizarDisplayBasico();
void actualizarDisplayAvanzado();
void actualizarDisplayParametros();
void actualizarPrimeraEntradaEstado();
void titilarCursor(int x,int y);

// =====[Implementacion de funciones publicas]=====

void inicializarInterfazUsuario(){
    
    const float (*valoresSensores)[CANTIDAD_MUESTRAS+1] = leerSensores();
    temperaturaCalentadorIU = &valoresSensores[0][0]; 

    configuraciones = leerConfiguraciones();
    estadoControlIU = leerEstadoControl();
    estadoConfiguracionesIU = leerEstadoConfiguraciones();
    estadoMenuBasicoIU = leerEstadoMenuBasico();
    estadoMenuAvanzadoIU = leerEstadoMenuAvanzado();
    estadoMostrarParametrosIU = leerEstadoMostrarParametros();

    estadoConfiguracionesAnteriorIU = *estadoConfiguracionesIU;
    estadoMenuBasicoAnteriorIU = *estadoMenuBasicoIU;
    estadoMenuAvanzadoAnteriorIU = *estadoMenuAvanzadoIU;
    estadoMostrarParametrosAnteriorIU = *estadoMostrarParametrosIU;

    lcd.init();           
    lcd.backlight();
    lcd.setCursor(2,0);
    lcd.print("TEMPERATURA:");
    lcd.setCursor(10,1);
    lcd.write(byte(223));
    lcd.print("C");

}

void actualizarInterfazUsuario(){

    actualizarPrimeraEntradaEstado();
    
    // Si esta en estado CAMBIO SENSORES, se muestra por pantalla VERIFICAR SENSOR
    
    if (*estadoControlIU == ALARMA){
        if (primeraEntradaEstado == true){
            lcd.clear();
            lcd.setCursor(5,0);
            lcd.print("APAGAR");
            lcd.setCursor(5,1);
            lcd.print("EQUIPO");
        }
    } else if (*estadoControlIU == CAMBIO_SENSORES){
        if (primeraEntradaEstado == true){
            lcd.clear();
            lcd.setCursor(3,0);
            lcd.print("VERIFICAR");
            lcd.setCursor(4,1);
            lcd.print("SENSOR");
        }
    } else {

        // Si no esta en cambio de sensores, se muestran las configuraciones regulares 
        switch (*estadoConfiguracionesIU){
        case IDLE_CONFIGURACION:
            if (primeraEntradaEstado == true){
                lcd.clear();
                lcd.setCursor(2,0);
                lcd.print("TEMPERATURA:");
                lcd.setCursor(10,1);
                lcd.write(byte(223));
                lcd.print("C");
            }

            lcd.setCursor(5,1);
            lcd.print(*temperaturaCalentadorIU,1);
            break;
        
        case BASICO:
            actualizarDisplayBasico();
            break;

        case AVANZADO: 
            actualizarDisplayAvanzado();
            break;
            
        case MOSTRAR_PARAMETROS:
            actualizarDisplayParametros();
            break;
        
        }
    }
}

// =====[Implementacion de funciones privadas]=====

void actualizarDisplayBasico(){

    switch (*estadoMenuBasicoIU){
        case SP1_ESTABLE: 
            if (primeraEntradaEstado == true){
                lcd.clear();
                lcd.setCursor(6,0);
                lcd.print("SP1:");
                lcd.setCursor(6,1);
                lcd.print(configuraciones->SP[0].valor,1);
                lcd.print(" ");
            }
            break;

        case SP1_EN_CAMBIO: 
            titilarCursor(3,1);

            lcd.setCursor(6,1);
            lcd.print(configuraciones->SP[0].valorConfiguracion,1);
            lcd.print(" ");
            break;

        case SP2_ESTABLE: 
            if (primeraEntradaEstado == true){
                lcd.clear();
                lcd.setCursor(6,0);
                lcd.print("SP2:");
                lcd.setCursor(6,1);
                lcd.print(configuraciones->SP[1].valor,1);
                lcd.print(" ");
            }
            break;

        case SP2_EN_CAMBIO: 
            titilarCursor(3,1);
            
            lcd.setCursor(6,1);
            lcd.print(configuraciones->SP[1].valorConfiguracion,1);
            lcd.print(" ");
            
            break;
            
        case SP3_ESTABLE: 
            if (primeraEntradaEstado == true){
                lcd.clear();
                lcd.setCursor(6,0);
                lcd.print("SP3:");
                lcd.setCursor(6,1);
                lcd.print(configuraciones->SP[2].valor,1);
                lcd.print(" ");
            }
            break;

        case SP3_EN_CAMBIO: 
            titilarCursor(3,1);
            
            lcd.setCursor(6,1);
            lcd.print(configuraciones->SP[2].valorConfiguracion,1);
            lcd.print(" ");
            
            break;
            
        case SP4_ESTABLE: 
            if (primeraEntradaEstado == true){
                lcd.clear();
                lcd.setCursor(6,0);
                lcd.print("SP4:");
                lcd.setCursor(6,1);
                lcd.print(configuraciones->SP[3].valor,1);
                lcd.print(" ");
            }
            break;

        case SP4_EN_CAMBIO: 
            titilarCursor(3,1);
            
            lcd.setCursor(6,1);
            lcd.print(configuraciones->SP[3].valorConfiguracion,1);
            lcd.print(" ");
            
            break;
    }
}

void actualizarDisplayAvanzado(){

    switch (*estadoMenuAvanzadoIU){
        case INGRESANDO:
            if (primeraEntradaEstado == true){
                lcd.clear();
                lcd.setCursor(5,0);
                lcd.print("CODIGO:");
                lcd.setCursor(6,1);
                lcd.print("____");
            }

            for(int i=0;i<configuraciones->codigo.cantidadDigitosIngresados;i++){
                lcd.setCursor(6+i,1);
                lcd.print("*");
            }
            static int contador = 0;
            if (contador > TIEMPO_TITILAR){
                lcd.setCursor(6+configuraciones->codigo.cantidadDigitosIngresados,1);
                lcd.print("_");
            }
            if (contador > 2* TIEMPO_TITILAR ){
                lcd.setCursor(6+configuraciones->codigo.cantidadDigitosIngresados,1);
                 lcd.print(configuraciones->codigo.digitoActual,0);
                contador = 0;
            }
            contador++;
            break;

        case INGRESO_INCORRECTO:
            if (primeraEntradaEstado == true){
                lcd.clear();
                lcd.setCursor(5,0);
                lcd.print("CODIGO");
                lcd.setCursor(3,1);
                lcd.print("INCORRECTO");
            }

            break;

        case SP1_HISTERESIS_ESTABLE: 
            if (primeraEntradaEstado == true){
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("HISTERESIS SP1:");
                lcd.setCursor(6,1);
                lcd.print(configuraciones->SP[0].histeresis.valor,1);
            }
            break;

        case SP1_HISTERESIS_EN_CAMBIO: 
            titilarCursor(3,1);

            lcd.setCursor(6,1);
            lcd.print(configuraciones->SP[0].histeresis.valorConfiguracion,1);
            lcd.print(" ");
            break;

        case SP2_HISTERESIS_ESTABLE: 
            if (primeraEntradaEstado == true){
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("HISTERESIS SP2:");
                lcd.setCursor(6,1);
                lcd.print(configuraciones->SP[1].histeresis.valor,1);
            }
            break;

        case SP2_HISTERESIS_EN_CAMBIO: 
            titilarCursor(3,1);

            lcd.setCursor(6,1);
            lcd.print(configuraciones->SP[1].histeresis.valorConfiguracion,1);
            lcd.print(" ");
            break;

        case SP3_HISTERESIS_ESTABLE: 
            if (primeraEntradaEstado == true){
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("HISTERESIS SP3:");
                lcd.setCursor(6,1);
                lcd.print(configuraciones->SP[2].histeresis.valor,1);
            }
            break;

        case SP3_HISTERESIS_EN_CAMBIO: 
            titilarCursor(3,1);

            lcd.setCursor(6,1);
            lcd.print(configuraciones->SP[2].histeresis.valorConfiguracion,1);
            lcd.print(" ");
            break;

        case SP4_HISTERESIS_ESTABLE: 
            if (primeraEntradaEstado == true){
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("HISTERESIS SP4:");
                lcd.setCursor(6,1);
                lcd.print(configuraciones->SP[3].histeresis.valor,1);
            }
            break;

        case SP4_HISTERESIS_EN_CAMBIO: 
            titilarCursor(3,1);

            lcd.setCursor(6,1);
            lcd.print(configuraciones->SP[3].histeresis.valorConfiguracion,1);
            lcd.print(" ");
            break;

        case LIMITES_ESPERANDO: 
            if (primeraEntradaEstado == true){
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("MAX SP:");
                lcd.setCursor(10,0);
                lcd.print(configuraciones->limitesSP.maximoConfiguracion,1);
                lcd.setCursor(0,1);
                lcd.print("MIN SP:");
                lcd.setCursor(10,1);
                lcd.print(configuraciones->limitesSP.minimoConfiguracion,1);
            }
            break;

        case LIMITE_MAXIMO_EN_CAMBIO: 
        
            if (primeraEntradaEstado == true){
                lcd.clear();
                lcd.setCursor(3,0);
                lcd.print("MAXIMO SP:");
            }

            titilarCursor(3,1);

            lcd.setCursor(6,1);
            lcd.print(configuraciones->limitesSP.maximoConfiguracion,1);
            lcd.print(" ");
            break;

        case LIMITE_MINIMO_EN_CAMBIO:

            if (primeraEntradaEstado == true){
                lcd.clear();
                lcd.setCursor(3,0);
                lcd.print("MINIMO SP:");
            }

            titilarCursor(3,1);

            lcd.setCursor(6,1);
            lcd.print(configuraciones->limitesSP.minimoConfiguracion,1);
            lcd.print(" ");
            break;

        case RETARDOS_ESPERANDO: 
            if (primeraEntradaEstado == true){
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("RET PREN:");
                lcd.setCursor(10,0);
                lcd.print(configuraciones->retardoPrendido.valor,1);
                lcd.setCursor(0,1);
                lcd.print("RET APAG:");
                lcd.setCursor(10,1);
                lcd.print(configuraciones->retardoApagado.valor,1);
            }
            break;

        case RETARDO_PRENDIDO_EN_CAMBIO:
            if (primeraEntradaEstado == true){
                lcd.clear();
                lcd.setCursor(3,0);
                lcd.print("PRENDIDO:");
            }

            titilarCursor(3,1);

            lcd.setCursor(6,1);
            lcd.print(configuraciones->retardoPrendido.valorConfiguracion,1);
            lcd.print(" ");
            break;

        case RETARDO_APAGADO_EN_CAMBIO: 
            if (primeraEntradaEstado == true){
                lcd.clear();
                lcd.setCursor(4,0);
                lcd.print("APAGADO:");
            }

            titilarCursor(3,1);

            lcd.setCursor(6,1);
            lcd.print(configuraciones->retardoApagado.valorConfiguracion,1);
            lcd.print(" ");
            break;
        
        case MINIMO_ALARMA_ESPERANDO:
            if (primeraEntradaEstado == true){
                lcd.clear();
                lcd.setCursor(1,0);
                lcd.print("MINIMO ALARMA:");
                lcd.setCursor(1,1);
                lcd.print(configuraciones->alarma.minimo,1);
                lcd.setCursor(12,1);
                if (configuraciones->alarma.minimoActivo == true){
                    lcd.print("ON ");
                } else {
                    lcd.print("OFF");
                }
            }
            break;
        case MINIMO_ALARMA_EN_CAMBIO:

            if (primeraEntradaEstado == true){
                lcd.clear();
                lcd.setCursor(1,0);
                lcd.print("MINIMO ALARMA:");
            }

            titilarCursor(3,1);

            lcd.setCursor(6,1);
            lcd.print(configuraciones->alarma.minimoConfiguracion,1);
            lcd.print(" ");
            break;
        case MINIMO_ALARMA_ACTIVO_EN_CAMBIO:
            if (primeraEntradaEstado == true){
                lcd.clear();
                lcd.setCursor(4,0);
                lcd.print("ACTIVAR:");
            }

            titilarCursor(4,1);
            lcd.setCursor(7,1);
            if (configuraciones->alarma.minimoActivo == true){
                lcd.print("ON ");
            } else {
                lcd.print("OFF");
            }

            break;
        
        case MAXIMO_ALARMA_ESPERANDO:
            if (primeraEntradaEstado == true){
                lcd.clear();
                lcd.setCursor(1,0);
                lcd.print("MAXIMO ALARMA:");
                lcd.setCursor(1,1);
                lcd.print(configuraciones->alarma.maximo,1);
                lcd.setCursor(12,1);
                if (configuraciones->alarma.maximoActivo == true){
                    lcd.print("ON ");
                } else {
                    lcd.print("OFF");
                }
            }
            break;
        case MAXIMO_ALARMA_EN_CAMBIO:

            if (primeraEntradaEstado == true){
                lcd.clear();
                lcd.setCursor(1,0);
                lcd.print("MAXIMO ALARMA:");
            }

            titilarCursor(3,1);

            lcd.setCursor(6,1);
            lcd.print(configuraciones->alarma.maximoConfiguracion,1);
            lcd.print(" ");
            break;
        case MAXIMO_ALARMA_ACTIVO_EN_CAMBIO:
            if (primeraEntradaEstado == true){
                lcd.clear();
                lcd.setCursor(4,0);
                lcd.print("ACTIVAR:");
            }

            titilarCursor(4,1);
            lcd.setCursor(7,1);
            if (configuraciones->alarma.maximoActivo == true){
                lcd.print("ON ");
            } else {
                lcd.print("OFF");
            }

            break;
        
        case VERSION_SOFTWARE:
            if (primeraEntradaEstado == true){
                lcd.clear();
                lcd.setCursor(4,0);
                lcd.print("VERSION:");
                lcd.setCursor(6,1);
                lcd.print(1.0,1);

            }
            break;

    }
}

void actualizarDisplayParametros(){
    switch (*estadoMostrarParametrosIU){
        case MOSTRAR_SP1: 
            if (primeraEntradaEstado == true){
            lcd.clear();
            lcd.setCursor(6,0);
            lcd.print("SP1:");
            lcd.setCursor(6,1);
            lcd.print(configuraciones->SP[0].valor,1);
            lcd.print(" ");
            }
        break;
        case MOSTRAR_SP2: 
            if (primeraEntradaEstado == true){
            lcd.clear();
            lcd.setCursor(6,0);
            lcd.print("SP2:");
            lcd.setCursor(6,1);
            lcd.print(configuraciones->SP[1].valor,1);
            lcd.print(" ");
            }
        break;
        case MOSTRAR_SP3: 
            if (primeraEntradaEstado == true){
            lcd.clear();
            lcd.setCursor(6,0);
            lcd.print("SP3:");
            lcd.setCursor(6,1);
            lcd.print(configuraciones->SP[2].valor,1);
            lcd.print(" ");
            }
        break;
        case MOSTRAR_SP4: 
            if (primeraEntradaEstado == true){
            lcd.clear();
            lcd.setCursor(6,0);
            lcd.print("SP4:");
            lcd.setCursor(6,1);
            lcd.print(configuraciones->SP[3].valor,1);
            lcd.print(" ");
            }
        break;
    }
}

void actualizarPrimeraEntradaEstado(){
    // Solo debe hacer una pasada en true, si ya paso una, lo cambia a false
    if (primeraEntradaEstado == true){
        primeraEntradaEstado = false;
    } else {
        // Si no estaba en true, compara los estados actuales con los anteriores para ver si cambiaron
        if (estadoControlAnteriorIU != *estadoControlIU){
            primeraEntradaEstado = true;
            estadoControlAnteriorIU = *estadoControlIU;
        }
        if (estadoConfiguracionesAnteriorIU != *estadoConfiguracionesIU){
            primeraEntradaEstado = true;
            estadoConfiguracionesAnteriorIU = *estadoConfiguracionesIU;
        } else {
            if (estadoMenuBasicoAnteriorIU != *estadoMenuBasicoIU){
                primeraEntradaEstado = true;
                estadoMenuBasicoAnteriorIU = *estadoMenuBasicoIU;
            }

            if (estadoMenuAvanzadoAnteriorIU != *estadoMenuAvanzadoIU){
                primeraEntradaEstado = true;
                estadoMenuAvanzadoAnteriorIU = *estadoMenuAvanzadoIU;
            }

            if (estadoMostrarParametrosAnteriorIU != *estadoMostrarParametrosIU){
                primeraEntradaEstado = true;
                estadoMostrarParametrosAnteriorIU = *estadoMostrarParametrosIU;
            }

        }
    }
}

void titilarCursor(int x,int y){
    static int contador = 0;
    if (contador == TIEMPO_TITILAR){
        lcd.setCursor(x,y);
        lcd.print("->");
    }
    if (contador == 2* TIEMPO_TITILAR ){
        lcd.setCursor(x,y);
        lcd.print("  ");
        contador = 0;
    }
    contador++;
            
}