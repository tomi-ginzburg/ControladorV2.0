#include "interfaz_usuario.h"


// =====[Declaracion de tipos de datos privados]=====

LiquidCrystal_I2C lcd(0x27,16,2);

// Punteros a datos de otros modulos
const float *temperaturaCalentadorIU = NULL;
const float *temperaturaBombaIU = NULL; 
const configuracion_t *configuraciones = NULL;
const estadoConfiguraciones_t *estadoConfiguracionesIU = NULL;
const estadoMenuBasico_t *estadoMenuBasicoIU = NULL;
const estadoMenuAvanzado_t *estadoMenuAvanzadoIU = NULL;
const estadoMostrarParametros_t *estadoMostrarParametrosIU = NULL;
const estadoMostrarSensores_t *estadoMostrarSensoresIU = NULL;

// Variables para el registro de los estado anteriores
estadoConfiguraciones_t estadoConfiguracionesAnteriorIU;
estadoMenuBasico_t estadoMenuBasicoAnteriorIU;
estadoMenuAvanzado_t estadoMenuAvanzadoAnteriorIU;
estadoMostrarParametros_t estadoMostrarParametrosAnteriorIU;
estadoMostrarSensores_t estadoMostrarSensoresAnteriorIU;

bool primeraEntradaEstado = false;

// =====[Declaracion de funciones privadas]==========

void actualizarDisplayBasico();
void actualizarDisplayAvanzado();
void actualizarDisplayParametros();
void actualizarDisplaySensores();
void actualizarPrimeraEntradaEstado();
void titilarCursor(int x,int y);

// =====[Implementacion de funciones publicas]=====

void inicializarInterfazUsuario(){
    
    const float *valoresSensores = leerSensores();
    temperaturaCalentadorIU = &valoresSensores[0]; 
    temperaturaBombaIU = &valoresSensores[1]; 

    configuraciones = leerConfiguraciones();
    estadoConfiguracionesIU = leerEstadoConfiguraciones();
    estadoMenuBasicoIU = leerEstadoMenuBasico();
    estadoMenuAvanzadoIU = leerEstadoMenuAvanzado();
    estadoMostrarParametrosIU = leerEstadoMostrarParametros();
    estadoMostrarSensoresIU = leerEstadoMostrarSensores();

    estadoConfiguracionesAnteriorIU = *estadoConfiguracionesIU;
    estadoMenuBasicoAnteriorIU = *estadoMenuBasicoIU;
    estadoMenuAvanzadoAnteriorIU = *estadoMenuAvanzadoIU;
    estadoMostrarParametrosAnteriorIU = *estadoMostrarParametrosIU;
    estadoMostrarSensoresAnteriorIU = *estadoMostrarSensoresIU;

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
    
    case MOSTRAR_SENSORES:
        actualizarDisplaySensores();
      break;
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
                lcd.print(configuraciones->SP1.valor,1);
            }
            break;

        case SP1_EN_CAMBIO: 
            titilarCursor(3,1);

            lcd.setCursor(6,1);
            lcd.print(configuraciones->SP1.valorConfiguracion,1);
            break;

        case SP2_ESTABLE: 
            if (primeraEntradaEstado == true){
                lcd.clear();
                lcd.setCursor(6,0);
                lcd.print("SP2:");
                lcd.setCursor(6,1);
                lcd.print(configuraciones->SP2.valor,1);
            }
            break;

        case SP2_EN_CAMBIO: 
            titilarCursor(3,1);
            
            lcd.setCursor(6,1);
            lcd.print(configuraciones->SP2.valorConfiguracion,1);
            
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
            if (contador > 50){
                lcd.setCursor(6+configuraciones->codigo.cantidadDigitosIngresados,1);
                lcd.print("_");
            }
            if (contador > 100 ){
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

        case SP1_ESPERANDO: 
            if (primeraEntradaEstado == true){
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("MAX SP1:");
                lcd.setCursor(10,0);
                lcd.print(configuraciones->SP1.maximoConfiguracion,1);
                lcd.setCursor(0,1);
                lcd.print("MIN SP1:");
                lcd.setCursor(10,1);
                lcd.print(configuraciones->SP1.minimoConfiguracion,1);
            }
            break;

        case SP1_MAX_EN_CAMBIO: 
        
            if (primeraEntradaEstado == true){
                lcd.clear();
                lcd.setCursor(2,0);
                lcd.print("MAXIMO SP1:");
            }

            titilarCursor(3,1);

            lcd.setCursor(6,1);
            lcd.print(configuraciones->SP1.maximoConfiguracion,1);
            break;

        case SP1_MIN_EN_CAMBIO:

            if (primeraEntradaEstado == true){
                lcd.clear();
                lcd.setCursor(1,0);
                lcd.print("MINIMO SP1:");
            }

            titilarCursor(3,1);

            lcd.setCursor(6,1);
            lcd.print(configuraciones->SP1.minimoConfiguracion,1);
            break;

        case SP2_ESPERANDO: 
            if (primeraEntradaEstado == true){
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("MAX SP2:");
                lcd.setCursor(10,0);
                lcd.print(configuraciones->SP2.maximoConfiguracion,1);
                lcd.setCursor(0,1);
                lcd.print("MIN SP2:");
                lcd.setCursor(10,1);
                lcd.print(configuraciones->SP2.minimoConfiguracion,1);
            }
            break;

        case SP2_MAX_EN_CAMBIO: 

            if (primeraEntradaEstado == true){
                lcd.clear();
                lcd.setCursor(2,0);
                lcd.print("MAXIMO SP2:");
            }

            titilarCursor(3,1);

            lcd.setCursor(6,1);
            lcd.print(configuraciones->SP2.maximoConfiguracion,1);
            break;

        case SP2_MIN_EN_CAMBIO: 
            if (primeraEntradaEstado == true){
                lcd.clear();
                lcd.setCursor(1,0);
                lcd.print("MINIMO SP2:");
            }

            titilarCursor(3,1);

            lcd.setCursor(6,1);
            lcd.print(configuraciones->SP2.minimoConfiguracion,1);
            break;

        case HISTERESIS_ESPERANDO: 
            if (primeraEntradaEstado == true){
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("HIST 1:");
                lcd.setCursor(10,0);
                lcd.print(configuraciones->histeresis1.valor,1);
                lcd.setCursor(0,1);
                lcd.print("HIST 2:");
                lcd.setCursor(10,1);
                lcd.print(configuraciones->histeresis2.valor,1);
            }
            break;

        case H1_EN_CAMBIO: 

            if (primeraEntradaEstado == true){
                lcd.clear();
                lcd.setCursor(2,0);
                lcd.print("HISTERESIS 1:");
            }

            titilarCursor(3,1);

            lcd.setCursor(6,1);
            lcd.print(configuraciones->histeresis1.valorConfiguracion,1);
            break;

        case H2_EN_CAMBIO: 
            if (primeraEntradaEstado == true){
                lcd.clear();
                lcd.setCursor(1,0);
                lcd.print("HISTERESIS 2:");
            }

            titilarCursor(3,1);

            lcd.setCursor(6,1);
            lcd.print(configuraciones->histeresis2.valorConfiguracion,1);
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
            break;
        
        case CONFIG_SENSORES: 
            if (primeraEntradaEstado == true){
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("CAMBIO SENSORES:");
            }

            titilarCursor(4,1);
            lcd.setCursor(7,1);
            if (configuraciones->cambioSensores == true){
                lcd.print("ON ");
            } else {
                lcd.print("OFF");
            }

            break;
        
        case MODO_DIAGNOSTICO: 
            if (primeraEntradaEstado == true){
                lcd.clear();
                lcd.setCursor(2,0);
                lcd.print("DIAGNOSTICO:");
            }

            titilarCursor(4,1);
            lcd.setCursor(7,1);
            if (configuraciones->modoDiagnostico == true){
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
            lcd.print(configuraciones->SP1.valor,1);
            }
        break;
        case MOSTRAR_SP2: 
            if (primeraEntradaEstado == true){
            lcd.clear();
            lcd.setCursor(6,0);
            lcd.print("SP2:");
            lcd.setCursor(6,1);
            lcd.print(configuraciones->SP2.valor,1);
            }
        break;
    }
}

void actualizarDisplaySensores(){
    switch (*estadoMostrarSensoresIU) {
        case MOSTRAR_SENSOR2:
            if (primeraEntradaEstado == true){
            lcd.clear();
            lcd.setCursor(2,0);
            lcd.print("CALEFACCION:");
            lcd.setCursor(10,1);
            lcd.write(byte(223));
            lcd.print("C");
            }
            lcd.setCursor(5,1);
            lcd.print(*temperaturaBombaIU,1);
        break;
    }
}

void actualizarPrimeraEntradaEstado(){
    // Solo debe hacer una pasada en true, si ya paso una, lo cambia a false
    if (primeraEntradaEstado == true){
        primeraEntradaEstado = false;
    } else {
        // Si no estaba en true, compara los estados actuales con los anteriores para ver si cambiaron
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

            if (estadoMostrarSensoresAnteriorIU != *estadoMostrarSensoresIU){
                primeraEntradaEstado = true;
                estadoMostrarSensoresAnteriorIU = *estadoMostrarSensoresIU;
            }

        }
    }
}

void titilarCursor(int x,int y){
    static int contador = 0;
    if (contador > 50){
        lcd.setCursor(x,y);
        lcd.print("->");
    }
    if (contador > 100 ){
        lcd.setCursor(x,y);
        lcd.print("  ");
        contador = 0;
    }
    contador++;
            
}