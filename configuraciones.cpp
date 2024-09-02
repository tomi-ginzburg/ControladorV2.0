#include "configuraciones.h"


// =====[Declaracion de defines privados]============

#define TIEMPO_BASICO_MS            2000
#define TIEMPO_AVANZADO_MS          5000
#define DELAY_LOOP_MS               10

// =====[Declaracion de tipos de datos privados]=====

const int TIEMPO_CAMBIO_RAPIDO_MS = 50; //Tiene que ser multiplo de 10
const int UMBRAL_CAMBIO_RAPIDO_MS = 1500;
const int CANT_CAMBIOS_UMBRAL = 5;
const float MINIMO_CAMBIO_SP = 0.1;
const float MAXIMO_CAMBIO_SP = 1.0; 
const float MAX_SP = 100.0;
const float MIN_SP = 1.0;

const float MINIMO_CAMBIO_HISTERESIS = 0.5;
const float MAX_HISTERESIS = 10.0;
const float MIN_HISTERESIS = 0.0;

const float MINIMO_CAMBIO_RETARDO = 1.0;
const float MAX_RETARDO = 15.0;
const float MIN_RETARDO = 0.0;

const float MAYOR_MAX_ALARMA = 100.0;
const float MENOR_MAX_ALARMA = 80.0;
const float MAYOR_MIN_ALARMA = 10.0;
const float MENOR_MIN_ALARMA  = 0.0;

Preferences nvs;
configuracion_t configuracion;
estadoConfiguraciones_t estadoConfiguraciones;
estadoMenuBasico_t estadoMenuBasico;
estadoMenuAvanzado_t estadoMenuAvanzado;
estadoMostrarParametros_t estadoMostrarParametros;

const boton_t *botonesConfiguracion = NULL;
const estadoControl_t *estadoControlConfiguraciones = NULL;

// botonesTerminados sirve para que al tocar una vez haga el cambio de estado pero no se vuelva a cambiar continuamente, sino que pare
bool botonesTerminados[CANTIDAD_BOTONES] = {true, true, true, true};

// =====[Declaracion de funciones privadas]=====

void inicializarValoresConfiguraciones();
void actualizarEstadoConfiguraciones();
void actualizarMenuBasico();
void actualizarMenuAvanzado();
void actualizarMostrarParametros();
void habilitarBotones(bool botonesTerminados[]);
void cambioValorContinuo(float *parametro, float maximo, float minimo, float minimoCambio, float maximoCambio, bool activacion = false);

// =====[Implementacion de funciones publicas]=====

void inicializarConfiguraciones(){
    inicializarValoresConfiguraciones();
    botonesConfiguracion = leerBotones();
    estadoControlConfiguraciones = leerEstadoControl();
    estadoConfiguraciones = IDLE_CONFIGURACION; 
    estadoMenuBasico = IDLE_BASICO;
    estadoMenuAvanzado = IDLE_AVANZADO;
    estadoMostrarParametros = IDLE_MP;
}

void actualizarConfiguraciones(){
    
    // Se actualiza la habilitacion de los botones luego de los cambios de estado
    habilitarBotones(botonesTerminados);
    
    switch (*estadoControlConfiguraciones){
        static bool primeraEntrada;
        case CONTROL_APAGADO:
        case ALARMA:
        case FUNCIONANDO:
            primeraEntrada = true;
            actualizarEstadoConfiguraciones();
            break;
        case CAMBIO_SENSORES:
            // Si es la primera vez que se entra, se guarda el estado en memoria
            if (primeraEntrada == true){ 
                configuracion.cambioSensores = true;
                nvs.begin("Configs",false);
                nvs.putBool("Sensores", configuracion.cambioSensores);
                nvs.end();
                primeraEntrada = false;
            }

            // Si se presiona ENTER, se vuelve guarda el nuevo modo de configuracino en memoria
            if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
              configuracion.cambioSensores = false;
              nvs.begin("Configs",false);
              nvs.putBool("Sensores", configuracion.cambioSensores);
              nvs.end();
              estadoConfiguraciones = IDLE_CONFIGURACION;
              botonesTerminados[INDICE_ENTER] = false;
              }
            break;
    }
}

const estadoConfiguraciones_t* leerEstadoConfiguraciones(){
    return &estadoConfiguraciones;
}

const estadoMenuBasico_t* leerEstadoMenuBasico(){
    return &estadoMenuBasico;
}

const estadoMenuAvanzado_t* leerEstadoMenuAvanzado(){
    return &estadoMenuAvanzado;
}

const estadoMostrarParametros_t* leerEstadoMostrarParametros(){
    return &estadoMostrarParametros;
}

const configuracion_t* leerConfiguraciones(){
    return &configuracion;
}

// =====[Implementacion de funciones privadas]=====

void inicializarValoresConfiguraciones(){
    float valorConfiguracion;
    bool modoConfiguracion;
    nvs.begin("Configs", false); // false para hacer el clear
    //nvs.clear();

    // VALORES DE LOS SP
    valorConfiguracion = nvs.getFloat("SP1",-1.0);
    if (valorConfiguracion != -1.0){
        configuracion.SP[0].valor = valorConfiguracion;    
    } else {
        configuracion.SP[0].valor = 0;
    }

    valorConfiguracion = nvs.getFloat("SP1-HIST",-1.0);
    if (valorConfiguracion != -1.0){
        configuracion.SP[0].histeresis.valor = valorConfiguracion;    
    } else {
        configuracion.SP[0].histeresis.valor = MIN_HISTERESIS;
    }

    valorConfiguracion = nvs.getFloat("SP2",-1.0);
    if (valorConfiguracion != -1.0){
        configuracion.SP[1].valor = valorConfiguracion;    
    } else {
        configuracion.SP[1].valor = 0;
    }

    valorConfiguracion = nvs.getFloat("SP2-HIST",-1.0);
    if (valorConfiguracion != -1.0){
        configuracion.SP[1].histeresis.valor = valorConfiguracion;    
    } else {
        configuracion.SP[1].histeresis.valor = MIN_HISTERESIS;
    }

    valorConfiguracion = nvs.getFloat("SP3",-1.0);
    if (valorConfiguracion != -1.0){
        configuracion.SP[2].valor = valorConfiguracion;    
    } else {
        configuracion.SP[2].valor = 0;
    }

    valorConfiguracion = nvs.getFloat("SP3-HIST",-1.0);
    if (valorConfiguracion != -1.0){
        configuracion.SP[2].histeresis.valor = valorConfiguracion;    
    } else {
        configuracion.SP[2].histeresis.valor = MIN_HISTERESIS;
    }

     valorConfiguracion = nvs.getFloat("SP4",-1.0);
    if (valorConfiguracion != -1.0){
        configuracion.SP[3].valor = valorConfiguracion;    
    } else {
        configuracion.SP[3].valor = 0;
    }
    
    valorConfiguracion = nvs.getFloat("SP4-HIST",-1.0);
    if (valorConfiguracion != -1.0){
        configuracion.SP[3].histeresis.valor = valorConfiguracion;    
    } else {
        configuracion.SP[3].histeresis.valor = MIN_HISTERESIS;
    }
    
    // MAXIMO Y MINIMO SP
    valorConfiguracion = nvs.getFloat("SP-MAX",-1.0);
    if (valorConfiguracion != -1.0){
        configuracion.limitesSP.maximo = valorConfiguracion;    
    } else {
        configuracion.limitesSP.maximo = MAX_SP;
    }    
    
    valorConfiguracion = nvs.getFloat("SP-MIN",-1.0);
    if (valorConfiguracion != -1.0){
        configuracion.limitesSP.minimo = valorConfiguracion;    
    } else {
        configuracion.limitesSP.minimo = MIN_SP;
    }    
    
    // RETARDO PRENDIDO Y APAGADO
    valorConfiguracion = nvs.getFloat("RET_PREN",-1.0);
    if (valorConfiguracion != -1.0){
        configuracion.retardoPrendido.valor = valorConfiguracion;    
    } else {
        configuracion.retardoPrendido.valor = MAX_RETARDO;
    }
    
    valorConfiguracion = nvs.getFloat("RET_APAG",-1.0);
    if (valorConfiguracion != -1.0){
        configuracion.retardoApagado.valor = valorConfiguracion;    
    } else {
        configuracion.retardoApagado.valor = MIN_RETARDO;
    }

    // ALARMA
    valorConfiguracion = nvs.getFloat("MIN_ALARMA",-1.0);
    if (valorConfiguracion != -1.0){
        configuracion.alarma.minimo = valorConfiguracion;    
    } else {
        configuracion.alarma.minimo = MENOR_MIN_ALARMA;
    }

    valorConfiguracion = nvs.getFloat("MAX_ALARMA",-1.0);
    if (valorConfiguracion != -1.0){
        configuracion.alarma.maximo = valorConfiguracion;    
    } else {
        configuracion.alarma.maximo = MAYOR_MAX_ALARMA;
    }
    
    modoConfiguracion = nvs.getBool("MIN_ALAR_ACT",-1.0);
    if (valorConfiguracion != -1.0){
        configuracion.alarma.minimoActivo = modoConfiguracion;    
    } else {
        configuracion.alarma.minimoActivo = false;
    }
    
    modoConfiguracion = nvs.getBool("MAX_ALAR_ACT",-1.0);
    if (valorConfiguracion != -1.0){
        configuracion.alarma.maximoActivo = modoConfiguracion;    
    } else {
        configuracion.alarma.maximoActivo = false;
    }

    // MODOS
    modoConfiguracion = nvs.getBool("Sensores",-1.0);
    if (valorConfiguracion != -1.0){
        configuracion.cambioSensores = modoConfiguracion;    
    } else {
        configuracion.cambioSensores = false;
    }

    nvs.end();

    // CODIGO MENU AVANZADO
    for (int i=0;i<CANT_DIGITOS_INGRESO;i++){
        configuracion.codigo.valor[i] = 0;
    }
    configuracion.codigo.cantidadDigitosIngresados = 0;
    configuracion.codigo.digitoActual = 0;
}

void habilitarBotones(bool botonesTerminados[]){
    for (int i=0;i<CANTIDAD_BOTONES;i++){
        if (botonesTerminados[i] == false && botonesConfiguracion[i].estado == NO_PRESIONADO){
            botonesTerminados[i] = true;
        }
    }
}

void actualizarEstadoConfiguraciones(){
    switch (estadoConfiguraciones){
    case IDLE_CONFIGURACION:
        // Si se aprieta el ENTER por mas de TIEMPO_AVANZADO_MS se entra al menu avanzado, y el basico vuelve a idle
        if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesConfiguracion[INDICE_ENTER].tiempoPresionadoMs > TIEMPO_AVANZADO_MS && botonesTerminados[INDICE_ENTER] == true){
            estadoConfiguraciones = AVANZADO;
            estadoMenuBasico = IDLE_BASICO;
        } 
        // Si se aprieta por mas de TIEMPO_BASICO_MS, se entra al menu basico
        else if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesConfiguracion[INDICE_ENTER].tiempoPresionadoMs > TIEMPO_BASICO_MS && botonesTerminados[INDICE_ENTER] == true){
            estadoConfiguraciones = BASICO;
        }
        // Si se aprieta por menos de TIEMPO_BASICO_MS, se entra al mostrar parametros
        else if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesConfiguracion[INDICE_ENTER].tiempoPresionadoMs < TIEMPO_BASICO_MS && botonesTerminados[INDICE_ENTER] == true){
            estadoConfiguraciones = MOSTRAR_PARAMETROS;
        }
        break;

    case BASICO:
        // Si no se encuentra en cambio y se aprieta el Atras, se vuelve al modo IDLE
        actualizarMenuBasico();
        if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesConfiguracion[INDICE_ENTER].tiempoPresionadoMs > TIEMPO_AVANZADO_MS){
            estadoConfiguraciones = AVANZADO;
            estadoMenuBasico = IDLE_BASICO;
        } 
        break;

    case AVANZADO:
        actualizarMenuAvanzado();
        break;

    case MOSTRAR_PARAMETROS:
        actualizarMostrarParametros();
        break;

    default: estadoConfiguraciones = IDLE_CONFIGURACION;
    }
}

void actualizarMenuBasico(){

    switch (estadoMenuBasico){
        case IDLE_BASICO: 
            // Inicia en SP1_ESTBLE
            if (estadoConfiguraciones == BASICO){
                estadoMenuBasico = SP1_ESTABLE;
                for (int i=0; i < CANT_ETAPAS; i++){
                    configuracion.SP[i].valorConfiguracion = configuracion.SP[i].valor;
                }
                botonesTerminados[INDICE_ENTER] = false;
            }
            break;
        case SP1_ESTABLE: 

            // Si se aprieta Enter se pasa al estado SP1_EN_CAMBIO
            if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                estadoMenuBasico = SP1_EN_CAMBIO;
                botonesTerminados[INDICE_ENTER] = false;
            } 
            
            // Si se aprieta Atras se vuelve al modo IDLE de estadoMenuBasico y de estadoConfiguraciones
            else if (botonesConfiguracion[INDICE_ATRAS].estado == PRESIONADO && botonesTerminados[INDICE_ATRAS] == true){
                estadoMenuBasico = IDLE_BASICO;
                estadoConfiguraciones = IDLE_CONFIGURACION;
                botonesTerminados[INDICE_ATRAS] = false;
            }

            // Si se aprieta menos con el boton que ya haya terminado, se pasa al modo SP1_ESTABLE
            else if (botonesConfiguracion[INDICE_MENOS].estado == PRESIONADO && botonesTerminados[INDICE_MENOS] == true){
                estadoMenuBasico = SP4_ESTABLE;
                botonesTerminados[INDICE_MENOS] = false;
            }

            // Si se aprieta mas con el boton que ya haya terminado, se pasa al modo SP2_ESTABLE
            else if (botonesConfiguracion[INDICE_MAS].estado == PRESIONADO && botonesTerminados[INDICE_MAS] == true){
                estadoMenuBasico = SP2_ESTABLE;
                botonesTerminados[INDICE_MAS] = false;
            }

            break;
        case SP1_EN_CAMBIO: 

            // Si se aprieta el ENTER se guarda el valor y se vuelve a SP1_ESTABLE
            if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                nvs.begin("Configs",false);
                nvs.putFloat("SP1",configuracion.SP[0].valorConfiguracion);
                nvs.end();
                configuracion.SP[0].valor = configuracion.SP[0].valorConfiguracion;
                estadoMenuBasico = SP1_ESTABLE;
                botonesTerminados[INDICE_ENTER] = false;
            } 
            
            // Si se aprieta Atras se vuelve a SP1_ESTABLE
            else if (botonesConfiguracion[INDICE_ATRAS].estado == PRESIONADO){
                estadoMenuBasico = SP1_ESTABLE;
                botonesTerminados[INDICE_ATRAS] = false;
            }

            // Si se aprieta Mas o Menos se cambia el valor de configuracionSP1
            else{
                cambioValorContinuo(&configuracion.SP[0].valorConfiguracion, configuracion.limitesSP.maximo, configuracion.limitesSP.minimo, MINIMO_CAMBIO_SP, MAXIMO_CAMBIO_SP, true);
            }

            break;
        case SP2_ESTABLE:

            // Si se aprieta Enter se pasa al estado SP1_EN_CAMBIO
            if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                estadoMenuBasico = SP2_EN_CAMBIO;
                botonesTerminados[INDICE_ENTER] = false;
            } 
            
            // Si se aprieta Atras se vuelve al modo IDLE de estadoMenuBasico y de estadoConfiguraciones
            else if (botonesConfiguracion[INDICE_ATRAS].estado == PRESIONADO){
                estadoMenuBasico = IDLE_BASICO;
                estadoConfiguraciones = IDLE_CONFIGURACION;
                botonesTerminados[INDICE_ATRAS] = false;
            }


            // Si se aprieta menos con el boton que ya haya terminado, se pasa al modo SP1_ESTABLE
            else if (botonesConfiguracion[INDICE_MENOS].estado == PRESIONADO && botonesTerminados[INDICE_MENOS] == true){
                estadoMenuBasico = SP1_ESTABLE;
                botonesTerminados[INDICE_MENOS] = false;
            }

            // Si se aprieta mas con el boton que ya haya terminado, se pasa al modo SP3_ESTABLE
            else if (botonesConfiguracion[INDICE_MAS].estado == PRESIONADO && botonesTerminados[INDICE_MAS] == true){
                estadoMenuBasico = SP3_ESTABLE;
                botonesTerminados[INDICE_MAS] = false;
            }

            break;
        case SP2_EN_CAMBIO:

            // Si se aprieta el ENTER se guarda el valor y se vuelve a SP2_ESTABLE
            if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                nvs.begin("Configs",false);
                nvs.putFloat("SP2",configuracion.SP[1].valorConfiguracion);
                nvs.end();
                configuracion.SP[1].valor = configuracion.SP[1].valorConfiguracion;
                estadoMenuBasico = SP2_ESTABLE;
                botonesTerminados[INDICE_ENTER] = false;
            } 
            
            // Si se aprieta Atras se vuelve a SP2_ESTABLE
            else if (botonesConfiguracion[INDICE_ATRAS].estado == PRESIONADO){
                estadoMenuBasico = SP2_ESTABLE;
                botonesTerminados[INDICE_ATRAS] = false;
                }

            // Si se aprieta Mas o Menos se cambia el valor de configuracionSP1
            else{
                cambioValorContinuo(&configuracion.SP[1].valorConfiguracion, configuracion.limitesSP.maximo, configuracion.limitesSP.minimo, MINIMO_CAMBIO_SP, MAXIMO_CAMBIO_SP, true);
            }

            break;
        case SP3_ESTABLE:

            // Si se aprieta Enter se pasa al estado SP1_EN_CAMBIO
            if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                estadoMenuBasico = SP3_EN_CAMBIO;
                botonesTerminados[INDICE_ENTER] = false;
            } 
            
            // Si se aprieta Atras se vuelve al modo IDLE de estadoMenuBasico y de estadoConfiguraciones
            else if (botonesConfiguracion[INDICE_ATRAS].estado == PRESIONADO){
                estadoMenuBasico = IDLE_BASICO;
                estadoConfiguraciones = IDLE_CONFIGURACION;
                botonesTerminados[INDICE_ATRAS] = false;
            }


            // Si se aprieta menos con el boton que ya haya terminado, se pasa al modo SP1_ESTABLE
            else if (botonesConfiguracion[INDICE_MENOS].estado == PRESIONADO && botonesTerminados[INDICE_MENOS] == true){
                estadoMenuBasico = SP2_ESTABLE;
                botonesTerminados[INDICE_MENOS] = false;
            }

            // Si se aprieta mas con el boton que ya haya terminado, se pasa al modo SP3_ESTABLE
            else if (botonesConfiguracion[INDICE_MAS].estado == PRESIONADO && botonesTerminados[INDICE_MAS] == true){
                estadoMenuBasico = SP4_ESTABLE;
                botonesTerminados[INDICE_MAS] = false;
            }

            break;
        case SP3_EN_CAMBIO:

            // Si se aprieta el ENTER se guarda el valor y se vuelve a SP2_ESTABLE
            if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                nvs.begin("Configs",false);
                nvs.putFloat("SP3",configuracion.SP[2].valorConfiguracion);
                nvs.end();
                configuracion.SP[2].valor = configuracion.SP[2].valorConfiguracion;
                estadoMenuBasico = SP3_ESTABLE;
                botonesTerminados[INDICE_ENTER] = false;
            } 
            
            // Si se aprieta Atras se vuelve a SP3_ESTABLE
            else if (botonesConfiguracion[INDICE_ATRAS].estado == PRESIONADO){
                estadoMenuBasico = SP3_ESTABLE;
                botonesTerminados[INDICE_ATRAS] = false;
                }

            // Si se aprieta Mas o Menos se cambia el valor de configuracionSP1
            else{
                cambioValorContinuo(&configuracion.SP[2].valorConfiguracion, configuracion.limitesSP.maximo, configuracion.limitesSP.minimo, MINIMO_CAMBIO_SP, MAXIMO_CAMBIO_SP, true);
            }

            break;
            
        case SP4_ESTABLE:

            // Si se aprieta Enter se pasa al estado SP1_EN_CAMBIO
            if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                estadoMenuBasico = SP4_EN_CAMBIO;
                botonesTerminados[INDICE_ENTER] = false;
            } 
            
            // Si se aprieta Atras se vuelve al modo IDLE de estadoMenuBasico y de estadoConfiguraciones
            else if (botonesConfiguracion[INDICE_ATRAS].estado == PRESIONADO){
                estadoMenuBasico = IDLE_BASICO;
                estadoConfiguraciones = IDLE_CONFIGURACION;
                botonesTerminados[INDICE_ATRAS] = false;
            }


            // Si se aprieta menos con el boton que ya haya terminado, se pasa al modo SP1_ESTABLE
            else if (botonesConfiguracion[INDICE_MENOS].estado == PRESIONADO && botonesTerminados[INDICE_MENOS] == true){
                estadoMenuBasico = SP3_ESTABLE;
                botonesTerminados[INDICE_MENOS] = false;
            }

            // Si se aprieta mas con el boton que ya haya terminado, se pasa al modo SP3_ESTABLE
            else if (botonesConfiguracion[INDICE_MAS].estado == PRESIONADO && botonesTerminados[INDICE_MAS] == true){
                estadoMenuBasico = SP1_ESTABLE;
                botonesTerminados[INDICE_MAS] = false;
            }

            break;
        case SP4_EN_CAMBIO:

            // Si se aprieta el ENTER se guarda el valor y se vuelve a SP2_ESTABLE
            if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                nvs.begin("Configs",false);
                nvs.putFloat("SP4",configuracion.SP[3].valorConfiguracion);
                nvs.end();
                configuracion.SP[3].valor = configuracion.SP[3].valorConfiguracion;
                estadoMenuBasico = SP4_ESTABLE;
                botonesTerminados[INDICE_ENTER] = false;
            } 
            
            // Si se aprieta Atras se vuelve a SP4_ESTABLE
            else if (botonesConfiguracion[INDICE_ATRAS].estado == PRESIONADO){
                estadoMenuBasico = SP4_ESTABLE;
                botonesTerminados[INDICE_ATRAS] = false;
                }

            // Si se aprieta Mas o Menos se cambia el valor de SP3
            else{
                cambioValorContinuo(&configuracion.SP[3].valorConfiguracion, configuracion.limitesSP.maximo, configuracion.limitesSP.minimo, MINIMO_CAMBIO_SP, MAXIMO_CAMBIO_SP, true);
            }

            break;
            
        default: estadoMenuBasico = IDLE_BASICO;
    }
}

void actualizarMenuAvanzado(){
    switch (estadoMenuAvanzado){
        case IDLE_AVANZADO:
            if (estadoConfiguraciones == AVANZADO){
                estadoMenuAvanzado = INGRESANDO;
                configuracion.codigo.cantidadDigitosIngresados = 0;
                configuracion.limitesSP.maximoConfiguracion = configuracion.limitesSP.maximo;
                configuracion.limitesSP.minimoConfiguracion = configuracion.limitesSP.minimo;
                for (int i=0;i<CANT_ETAPAS;i++){
                    configuracion.SP[i].histeresis.valorConfiguracion = configuracion.SP[i].histeresis.valor; 
                }
                configuracion.retardoPrendido.valorConfiguracion = configuracion.retardoPrendido.valor;
                configuracion.retardoApagado.valorConfiguracion = configuracion.retardoApagado.valor;
                configuracion.alarma.minimoConfiguracion = configuracion.alarma.minimo;
                configuracion.alarma.maximoConfiguracion = configuracion.alarma.maximo;
                botonesTerminados[INDICE_ENTER] = false;
            }
            break;
        case INGRESANDO: 
            // Si se aprieta Enter se pasa al siguiente digito o se verifica la contraseña
            if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                configuracion.codigo.ingresado[configuracion.codigo.cantidadDigitosIngresados] = configuracion.codigo.digitoActual;
                configuracion.codigo.cantidadDigitosIngresados++;
                configuracion.codigo.digitoActual = 0;
                if (configuracion.codigo.cantidadDigitosIngresados == CANT_DIGITOS_INGRESO){
                    bool coinciden = true;
                    for (int i=0;i<CANT_DIGITOS_INGRESO;i++){
                        if (configuracion.codigo.ingresado[i] != configuracion.codigo.valor[i]){
                            coinciden = false;
                        }
                    }

                    if (coinciden){
                        estadoMenuAvanzado = SP1_HISTERESIS_ESTABLE;
                    } else{
                        estadoMenuAvanzado = INGRESO_INCORRECTO;
                    }
                    configuracion.codigo.cantidadDigitosIngresados = 0;
                }
                botonesTerminados[INDICE_ENTER] = false;
            } 
            
            // Si se aprieta Atras se vuelve al modo IDLE de estadoMenuAvanzado y de estadoConfiguraciones
            else if (botonesConfiguracion[INDICE_ATRAS].estado == PRESIONADO && botonesTerminados[INDICE_ATRAS] == true){
                estadoMenuAvanzado = IDLE_AVANZADO;
                estadoConfiguraciones = IDLE_CONFIGURACION;
                botonesTerminados[INDICE_ATRAS] = false;
            }

            else{
                if (botonesConfiguracion[INDICE_MAS].estado == PRESIONADO && botonesTerminados[INDICE_MAS] == true && configuracion.codigo.digitoActual < 9){
                    configuracion.codigo.digitoActual = configuracion.codigo.digitoActual + MINIMO_CAMBIO_RETARDO;
                    botonesTerminados[INDICE_MAS] = false;
                }
                if (botonesConfiguracion[INDICE_MENOS].estado == PRESIONADO && botonesTerminados[INDICE_MENOS] == true && configuracion.codigo.digitoActual > 0){
                    configuracion.codigo.digitoActual = configuracion.codigo.digitoActual - MINIMO_CAMBIO_RETARDO;
                    botonesTerminados[INDICE_MENOS] = false;
                }
            }

            break;
        case INGRESO_INCORRECTO:
            static int contador = 0;
            if (contador == 100){
                estadoMenuAvanzado = INGRESANDO;
                contador = -1;
            }
            contador++;
            break;
        case SP1_HISTERESIS_ESTABLE:
            // Si se aprieta Enter se pasa al estado SP1_HISTERESIS_EN_CAMBIO
            if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                estadoMenuAvanzado = SP1_HISTERESIS_EN_CAMBIO;
                botonesTerminados[INDICE_ENTER] = false;
            } 
            
            // Si se aprieta Atras se vuelve al modo IDLE de estadoMenuAvanzado y de estadoConfiguraciones
            else if (botonesConfiguracion[INDICE_ATRAS].estado == PRESIONADO && botonesTerminados[INDICE_ATRAS] == true){
                estadoMenuAvanzado = IDLE_AVANZADO;
                estadoConfiguraciones = IDLE_CONFIGURACION;
                botonesTerminados[INDICE_ATRAS] = false;
            }

            // Si se aprieta mas con el boton que ya haya terminado, se pasa al modo SP2_HISTERESIS_ESTABLE
            else if (botonesConfiguracion[INDICE_MAS].estado == PRESIONADO && botonesTerminados[INDICE_MAS] == true){
                estadoMenuAvanzado = SP2_HISTERESIS_ESTABLE;
                botonesTerminados[INDICE_MAS] = false;
            }

            break;
        case SP1_HISTERESIS_EN_CAMBIO:
            // Si se aprieta Enter se guarda el valor y pasa al estado H2_EN_CAMBIO
            if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                nvs.begin("Configs",false);
                nvs.putFloat("SP1-HIST",configuracion.SP[0].histeresis.valorConfiguracion);
                configuracion.SP[0].histeresis.valor = configuracion.SP[0].histeresis.valorConfiguracion;
                nvs.end();
                estadoMenuAvanzado = SP1_HISTERESIS_ESTABLE;
                botonesTerminados[INDICE_ENTER] = false;
            } 

            // Si se aprieta Atras se vuelve a SP1_HISTERESIS_ESTABLE
            else if (botonesConfiguracion[INDICE_ATRAS].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                estadoMenuAvanzado = SP1_HISTERESIS_ESTABLE;
                botonesTerminados[INDICE_ATRAS] = false;
                }

            // Si se aprieta Mas o Menos se cambia el valor de configuracion
            else{
                if (botonesConfiguracion[INDICE_MAS].estado == PRESIONADO && botonesTerminados[INDICE_MAS] == true && configuracion.SP[0].histeresis.valorConfiguracion < MAX_HISTERESIS){
                    configuracion.SP[0].histeresis.valorConfiguracion = configuracion.SP[0].histeresis.valorConfiguracion + MINIMO_CAMBIO_HISTERESIS;
                    botonesTerminados[INDICE_MAS] = false;
                }
                if (botonesConfiguracion[INDICE_MENOS].estado == PRESIONADO && botonesTerminados[INDICE_MENOS] == true && configuracion.SP[0].histeresis.valorConfiguracion > MIN_HISTERESIS){
                    configuracion.SP[0].histeresis.valorConfiguracion = configuracion.SP[0].histeresis.valorConfiguracion - MINIMO_CAMBIO_HISTERESIS;
                    botonesTerminados[INDICE_MENOS] = false;
                }
            }

            break;
        case SP2_HISTERESIS_ESTABLE:
            // Si se aprieta Enter se pasa al estado SP1_HISTERESIS_EN_CAMBIO
            if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                estadoMenuAvanzado = SP2_HISTERESIS_EN_CAMBIO;
                botonesTerminados[INDICE_ENTER] = false;
            } 
            
            // Si se aprieta Atras se vuelve al modo IDLE de estadoMenuAvanzado y de estadoConfiguraciones
            else if (botonesConfiguracion[INDICE_ATRAS].estado == PRESIONADO && botonesTerminados[INDICE_ATRAS] == true){
                estadoMenuAvanzado = IDLE_AVANZADO;
                estadoConfiguraciones = IDLE_CONFIGURACION;
                botonesTerminados[INDICE_ATRAS] = false;
            }

            // Si se aprieta mas con el boton que ya haya terminado, se pasa al modo SP2_HISTERESIS_ESTABLE
            else if (botonesConfiguracion[INDICE_MAS].estado == PRESIONADO && botonesTerminados[INDICE_MAS] == true){
                estadoMenuAvanzado = SP3_HISTERESIS_ESTABLE;
                botonesTerminados[INDICE_MAS] = false;
            }

            // Si se aprieta menos con el boton que ya haya terminado, se pasa al modo HISTERESIS_ESPERANDO
            else if (botonesConfiguracion[INDICE_MENOS].estado == PRESIONADO && botonesTerminados[INDICE_MENOS] == true){
                estadoMenuAvanzado = SP1_HISTERESIS_ESTABLE;
                botonesTerminados[INDICE_MENOS] = false;
            }

            break;
        case SP2_HISTERESIS_EN_CAMBIO:
            // Si se aprieta Enter se guarda el valor y pasa al estado H2_EN_CAMBIO
            if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                nvs.begin("Configs",false);
                nvs.putFloat("SP2-HIST",configuracion.SP[1].histeresis.valorConfiguracion);
                configuracion.SP[1].histeresis.valor = configuracion.SP[1].histeresis.valorConfiguracion;
                nvs.end();
                estadoMenuAvanzado = SP2_HISTERESIS_ESTABLE;
                botonesTerminados[INDICE_ENTER] = false;
            } 

            // Si se aprieta Atras se vuelve a SP2_HISTERESIS_ESTABLE
            else if (botonesConfiguracion[INDICE_ATRAS].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                estadoMenuAvanzado = SP2_HISTERESIS_ESTABLE;
                botonesTerminados[INDICE_ATRAS] = false;
                }

            // Si se aprieta Mas o Menos se cambia el valor de configuracion
            else{
                if (botonesConfiguracion[INDICE_MAS].estado == PRESIONADO && botonesTerminados[INDICE_MAS] == true && configuracion.SP[1].histeresis.valorConfiguracion < MAX_HISTERESIS){
                    configuracion.SP[1].histeresis.valorConfiguracion = configuracion.SP[1].histeresis.valorConfiguracion + MINIMO_CAMBIO_HISTERESIS;
                    botonesTerminados[INDICE_MAS] = false;
                }
                if (botonesConfiguracion[INDICE_MENOS].estado == PRESIONADO && botonesTerminados[INDICE_MENOS] == true && configuracion.SP[1].histeresis.valorConfiguracion > MIN_HISTERESIS){
                    configuracion.SP[1].histeresis.valorConfiguracion = configuracion.SP[1].histeresis.valorConfiguracion - MINIMO_CAMBIO_HISTERESIS;
                    botonesTerminados[INDICE_MENOS] = false;
                }
            }

            break;
        case SP3_HISTERESIS_ESTABLE:
            // Si se aprieta Enter se pasa al estado SP1_HISTERESIS_EN_CAMBIO
            if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                estadoMenuAvanzado = SP3_HISTERESIS_EN_CAMBIO;
                botonesTerminados[INDICE_ENTER] = false;
            } 
            
            // Si se aprieta Atras se vuelve al modo IDLE de estadoMenuAvanzado y de estadoConfiguraciones
            else if (botonesConfiguracion[INDICE_ATRAS].estado == PRESIONADO && botonesTerminados[INDICE_ATRAS] == true){
                estadoMenuAvanzado = IDLE_AVANZADO;
                estadoConfiguraciones = IDLE_CONFIGURACION;
                botonesTerminados[INDICE_ATRAS] = false;
            }

            // Si se aprieta mas con el boton que ya haya terminado, se pasa al modo SP2_HISTERESIS_ESTABLE
            else if (botonesConfiguracion[INDICE_MAS].estado == PRESIONADO && botonesTerminados[INDICE_MAS] == true){
                estadoMenuAvanzado = SP4_HISTERESIS_ESTABLE;
                botonesTerminados[INDICE_MAS] = false;
            }

            // Si se aprieta menos con el boton que ya haya terminado, se pasa al modo HISTERESIS_ESPERANDO
            else if (botonesConfiguracion[INDICE_MENOS].estado == PRESIONADO && botonesTerminados[INDICE_MENOS] == true){
                estadoMenuAvanzado = SP2_HISTERESIS_ESTABLE;
                botonesTerminados[INDICE_MENOS] = false;
            }

            break;
        case SP3_HISTERESIS_EN_CAMBIO:
            // Si se aprieta Enter se guarda el valor y pasa al estado H2_EN_CAMBIO
            if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                nvs.begin("Configs",false);
                nvs.putFloat("SP3-HIST",configuracion.SP[2].histeresis.valorConfiguracion);
                configuracion.SP[2].histeresis.valor = configuracion.SP[2].histeresis.valorConfiguracion;
                nvs.end();
                estadoMenuAvanzado = SP3_HISTERESIS_ESTABLE;
                botonesTerminados[INDICE_ENTER] = false;
            } 

            // Si se aprieta Atras se vuelve a SP3_HISTERESIS_ESTABLE
            else if (botonesConfiguracion[INDICE_ATRAS].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                estadoMenuAvanzado = SP2_HISTERESIS_ESTABLE;
                botonesTerminados[INDICE_ATRAS] = false;
                }

            // Si se aprieta Mas o Menos se cambia el valor de configuracion
            else{
                if (botonesConfiguracion[INDICE_MAS].estado == PRESIONADO && botonesTerminados[INDICE_MAS] == true && configuracion.SP[2].histeresis.valorConfiguracion < MAX_HISTERESIS){
                    configuracion.SP[2].histeresis.valorConfiguracion = configuracion.SP[2].histeresis.valorConfiguracion + MINIMO_CAMBIO_HISTERESIS;
                    botonesTerminados[INDICE_MAS] = false;
                }
                if (botonesConfiguracion[INDICE_MENOS].estado == PRESIONADO && botonesTerminados[INDICE_MENOS] == true && configuracion.SP[2].histeresis.valorConfiguracion > MIN_HISTERESIS){
                    configuracion.SP[2].histeresis.valorConfiguracion = configuracion.SP[2].histeresis.valorConfiguracion - MINIMO_CAMBIO_HISTERESIS;
                    botonesTerminados[INDICE_MENOS] = false;
                }
            }

            break;
        case SP4_HISTERESIS_ESTABLE:
            // Si se aprieta Enter se pasa al estado SP1_HISTERESIS_EN_CAMBIO
            if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                estadoMenuAvanzado = SP4_HISTERESIS_EN_CAMBIO;
                botonesTerminados[INDICE_ENTER] = false;
            } 
            
            // Si se aprieta Atras se vuelve al modo IDLE de estadoMenuAvanzado y de estadoConfiguraciones
            else if (botonesConfiguracion[INDICE_ATRAS].estado == PRESIONADO && botonesTerminados[INDICE_ATRAS] == true){
                estadoMenuAvanzado = IDLE_AVANZADO;
                estadoConfiguraciones = IDLE_CONFIGURACION;
                botonesTerminados[INDICE_ATRAS] = false;
            }

            // Si se aprieta mas con el boton que ya haya terminado, se pasa al modo SP2_HISTERESIS_ESTABLE
            else if (botonesConfiguracion[INDICE_MAS].estado == PRESIONADO && botonesTerminados[INDICE_MAS] == true){
                estadoMenuAvanzado = LIMITES_ESPERANDO;
                botonesTerminados[INDICE_MAS] = false;
            }

            // Si se aprieta menos con el boton que ya haya terminado, se pasa al modo HISTERESIS_ESPERANDO
            else if (botonesConfiguracion[INDICE_MENOS].estado == PRESIONADO && botonesTerminados[INDICE_MENOS] == true){
                estadoMenuAvanzado = SP3_HISTERESIS_ESTABLE;
                botonesTerminados[INDICE_MENOS] = false;
            }

            break;
        case SP4_HISTERESIS_EN_CAMBIO:
            // Si se aprieta Enter se guarda el valor y pasa al estado H2_EN_CAMBIO
            if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                nvs.begin("Configs",false);
                nvs.putFloat("SP4-HIST",configuracion.SP[3].histeresis.valorConfiguracion);
                configuracion.SP[3].histeresis.valor = configuracion.SP[3].histeresis.valorConfiguracion;
                nvs.end();
                estadoMenuAvanzado = SP4_HISTERESIS_ESTABLE;
                botonesTerminados[INDICE_ENTER] = false;
            } 

            // Si se aprieta Atras se vuelve a SP4_HISTERESIS_ESTABLE
            else if (botonesConfiguracion[INDICE_ATRAS].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                estadoMenuAvanzado = SP4_HISTERESIS_ESTABLE;
                botonesTerminados[INDICE_ATRAS] = false;
                }

            // Si se aprieta Mas o Menos se cambia el valor de configuracion
            else{
                if (botonesConfiguracion[INDICE_MAS].estado == PRESIONADO && botonesTerminados[INDICE_MAS] == true && configuracion.SP[3].histeresis.valorConfiguracion < MAX_HISTERESIS){
                    configuracion.SP[3].histeresis.valorConfiguracion = configuracion.SP[3].histeresis.valorConfiguracion + MINIMO_CAMBIO_HISTERESIS;
                    botonesTerminados[INDICE_MAS] = false;
                }
                if (botonesConfiguracion[INDICE_MENOS].estado == PRESIONADO && botonesTerminados[INDICE_MENOS] == true && configuracion.SP[3].histeresis.valorConfiguracion > MIN_HISTERESIS){
                    configuracion.SP[3].histeresis.valorConfiguracion = configuracion.SP[3].histeresis.valorConfiguracion - MINIMO_CAMBIO_HISTERESIS;
                    botonesTerminados[INDICE_MENOS] = false;
                }
            }

            break;
    
        case LIMITES_ESPERANDO:
            // Si se aprieta Enter se pasa al estado LIMITE_MAXIMO_EN_CAMBIO
            if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                estadoMenuAvanzado = LIMITE_MAXIMO_EN_CAMBIO;
                botonesTerminados[INDICE_ENTER] = false;
            } 
            
            // Si se aprieta Atras se vuelve al modo IDLE de estadoMenuAvanzado y de estadoConfiguraciones
            else if (botonesConfiguracion[INDICE_ATRAS].estado == PRESIONADO && botonesTerminados[INDICE_ATRAS] == true){
                estadoMenuAvanzado = IDLE_AVANZADO;
                estadoConfiguraciones = IDLE_CONFIGURACION;
                botonesTerminados[INDICE_ATRAS] = false;
            }

            // Si se aprieta mas con el boton que ya haya terminado, se pasa al modo RETARDOS_ESPERANDO
            else if (botonesConfiguracion[INDICE_MAS].estado == PRESIONADO && botonesTerminados[INDICE_MAS] == true){
                estadoMenuAvanzado = RETARDOS_ESPERANDO;
                botonesTerminados[INDICE_MAS] = false;
            }

            // Si se aprieta menos con el boton que ya haya terminado, se pasa al modo HISTERESIS_ESPERANDO
            else if (botonesConfiguracion[INDICE_MENOS].estado == PRESIONADO && botonesTerminados[INDICE_MENOS] == true){
                estadoMenuAvanzado = SP4_HISTERESIS_ESTABLE;
                botonesTerminados[INDICE_MENOS] = false;
            }

            break;
        case LIMITE_MAXIMO_EN_CAMBIO:
            // Si se aprieta Enter se guarda el valor maximo y pasa al estado LIMITE_MINIMO_EN_CAMBIO
            // En caso de que el maximo sea menor que el actual configurado, se cambia
            if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                nvs.begin("Configs",false);
                nvs.putFloat("SP-MAX",configuracion.limitesSP.maximoConfiguracion);
                configuracion.limitesSP.maximo = configuracion.limitesSP.maximoConfiguracion;
                nvs.end();
                estadoMenuAvanzado = LIMITE_MINIMO_EN_CAMBIO;
                botonesTerminados[INDICE_ENTER] = false;
            } 

            // Si se aprieta Atras se vuelve a LIMITES_ESPERANDO
            else if (botonesConfiguracion[INDICE_ATRAS].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                estadoMenuAvanzado = LIMITES_ESPERANDO;
                botonesTerminados[INDICE_ATRAS] = false;
                }

            // Si se aprieta Mas o Menos se cambia el valor de configuracionconfiguracion.SP1.maximo
            else{
                cambioValorContinuo(&configuracion.limitesSP.maximoConfiguracion, MAX_SP, configuracion.limitesSP.minimo, MINIMO_CAMBIO_SP, MAXIMO_CAMBIO_SP, false);
            }

            break;
        case LIMITE_MINIMO_EN_CAMBIO:
            // Si se aprieta Enter se guarda el valor minimo y vuelve al estado LIMITES_ESPERANDO
            // En caso de que el minimo sea mayor que el actual configurado, se cambia
            if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                nvs.begin("Configs",false);
                nvs.putFloat("SP-MIN",configuracion.limitesSP.minimoConfiguracion);
                configuracion.limitesSP.minimo = configuracion.limitesSP.minimoConfiguracion;
                nvs.end();
                estadoMenuAvanzado = LIMITES_ESPERANDO;
                botonesTerminados[INDICE_ENTER] = false;
            } 

            // Si se aprieta Atras se vuelve a LIMITES_ESPERANDO
            else if (botonesConfiguracion[INDICE_ATRAS].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                estadoMenuAvanzado = LIMITES_ESPERANDO;
                botonesTerminados[INDICE_ATRAS] = false;
                }

            // Si se aprieta Mas o Menos se cambia el valor de configuracionconfiguracion.limitesSP.minimo
            else{
                cambioValorContinuo(&configuracion.limitesSP.minimoConfiguracion, configuracion.limitesSP.maximo, MIN_SP, MINIMO_CAMBIO_SP, MAXIMO_CAMBIO_SP, false);
            }

            break;

        case RETARDOS_ESPERANDO:
            // Si se aprieta Enter se pasa al estado RETARDO_PRENDIDO_EN_CAMBIO
            if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                estadoMenuAvanzado = RETARDO_PRENDIDO_EN_CAMBIO;
                botonesTerminados[INDICE_ENTER] = false;
            } 
            
            // Si se aprieta Atras se vuelve al modo IDLE de estadoMenuAvanzado y de estadoConfiguraciones
            else if (botonesConfiguracion[INDICE_ATRAS].estado == PRESIONADO && botonesTerminados[INDICE_ATRAS] == true){
                estadoMenuAvanzado = IDLE_AVANZADO;
                estadoConfiguraciones = IDLE_CONFIGURACION;
                botonesTerminados[INDICE_ATRAS] = false;
            }

            // Si se aprieta menos con el boton que ya haya terminado, se pasa al modo HISTERESIS_ESPERANDO
            else if (botonesConfiguracion[INDICE_MENOS].estado == PRESIONADO && botonesTerminados[INDICE_MENOS] == true){
                estadoMenuAvanzado = LIMITES_ESPERANDO;
                botonesTerminados[INDICE_MENOS] = false;
            }


            // Si se aprieta mas con el boton que ya haya terminado, se pasa al modo MINIMO_ALARMA_ESPERANDO
            else if (botonesConfiguracion[INDICE_MAS].estado == PRESIONADO && botonesTerminados[INDICE_MAS] == true){
                estadoMenuAvanzado = MINIMO_ALARMA_ESPERANDO;
                botonesTerminados[INDICE_MAS] = false;
            }

            break;
        case RETARDO_PRENDIDO_EN_CAMBIO:
            // Si se aprieta Enter se guarda el valor y pasa al estado RETARDO_APAGADO_EN_CAMBIO
            if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                nvs.begin("Configs",false);
                nvs.putFloat("RET_PREN",configuracion.retardoPrendido.valorConfiguracion);
                configuracion.retardoPrendido.valor = configuracion.retardoPrendido.valorConfiguracion;
                nvs.end();
                estadoMenuAvanzado = RETARDO_APAGADO_EN_CAMBIO;
                botonesTerminados[INDICE_ENTER] = false;
            } 

            // Si se aprieta Atras se vuelve a RETARDOS_ESPERANDO
            else if (botonesConfiguracion[INDICE_ATRAS].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                estadoMenuAvanzado = RETARDOS_ESPERANDO;
                botonesTerminados[INDICE_ATRAS] = false;
                }

            // Si se aprieta Mas o Menos se cambia el valor de configuracion
            else{
                if (botonesConfiguracion[INDICE_MAS].estado == PRESIONADO && botonesTerminados[INDICE_MAS] == true && configuracion.retardoPrendido.valorConfiguracion < MAX_RETARDO){
                    configuracion.retardoPrendido.valorConfiguracion = configuracion.retardoPrendido.valorConfiguracion + MINIMO_CAMBIO_RETARDO;
                    botonesTerminados[INDICE_MAS] = false;
                }
                if (botonesConfiguracion[INDICE_MENOS].estado == PRESIONADO && botonesTerminados[INDICE_MENOS] == true && configuracion.retardoPrendido.valorConfiguracion > MIN_RETARDO){
                    configuracion.retardoPrendido.valorConfiguracion = configuracion.retardoPrendido.valorConfiguracion - MINIMO_CAMBIO_RETARDO;
                    botonesTerminados[INDICE_MENOS] = false;
                }
            }

            break;
        case RETARDO_APAGADO_EN_CAMBIO:
            // Si se aprieta Enter se guarda el valor y vuelve al estado RETARDOS_ESPERANDO
            if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                nvs.begin("Configs",false);
                nvs.putFloat("RET_APAG",configuracion.retardoApagado.valorConfiguracion);
                configuracion.retardoApagado.valor = configuracion.retardoApagado.valorConfiguracion;
                nvs.end();
                estadoMenuAvanzado = RETARDOS_ESPERANDO;
                botonesTerminados[INDICE_ENTER] = false;
            } 

            // Si se aprieta Atras se vuelve a RETARDOS_ESPERANDO
            else if (botonesConfiguracion[INDICE_ATRAS].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                estadoMenuAvanzado = RETARDOS_ESPERANDO;
                botonesTerminados[INDICE_ATRAS] = false;
                }

            // Si se aprieta Mas o Menos se cambia el valor de configuracion
            else{
                if (botonesConfiguracion[INDICE_MAS].estado == PRESIONADO && botonesTerminados[INDICE_MAS] == true && configuracion.retardoApagado.valorConfiguracion < MAX_RETARDO){
                    configuracion.retardoApagado.valorConfiguracion = configuracion.retardoApagado.valorConfiguracion + MINIMO_CAMBIO_RETARDO;
                    botonesTerminados[INDICE_MAS] = false;
                }
                if (botonesConfiguracion[INDICE_MENOS].estado == PRESIONADO && botonesTerminados[INDICE_MENOS] == true && configuracion.retardoApagado.valorConfiguracion > MIN_RETARDO){
                    configuracion.retardoApagado.valorConfiguracion = configuracion.retardoApagado.valorConfiguracion - MINIMO_CAMBIO_RETARDO;
                    botonesTerminados[INDICE_MENOS] = false;
                }
            }

            break;
        
        case MINIMO_ALARMA_ESPERANDO:
             // Si se aprieta Enter se pasa al estado MINIMO_ALARMA_EN_CAMBIO
            if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                estadoMenuAvanzado = MINIMO_ALARMA_EN_CAMBIO;
                botonesTerminados[INDICE_ENTER] = false;
            } 
            
            // Si se aprieta Atras se vuelve al modo IDLE de estadoMenuAvanzado y de estadoConfiguraciones
            else if (botonesConfiguracion[INDICE_ATRAS].estado == PRESIONADO && botonesTerminados[INDICE_ATRAS] == true){
                estadoMenuAvanzado = IDLE_AVANZADO;
                estadoConfiguraciones = IDLE_CONFIGURACION;
                botonesTerminados[INDICE_ATRAS] = false;
            }

            // Si se aprieta menos con el boton que ya haya terminado, se pasa al modo RETARDOS_ESPERANDO
            else if (botonesConfiguracion[INDICE_MENOS].estado == PRESIONADO && botonesTerminados[INDICE_MENOS] == true){
                estadoMenuAvanzado = RETARDOS_ESPERANDO;
                botonesTerminados[INDICE_MENOS] = false;
            }


            // Si se aprieta mas con el boton que ya haya terminado, se pasa al modo CONFIG_SENSORES
            else if (botonesConfiguracion[INDICE_MAS].estado == PRESIONADO && botonesTerminados[INDICE_MAS] == true){
                estadoMenuAvanzado = MAXIMO_ALARMA_ESPERANDO;
                botonesTerminados[INDICE_MAS] = false;
            }
            break;
        case MINIMO_ALARMA_EN_CAMBIO:
            // Si se aprieta Enter se guarda el valor minimo y pasa al estado MINIMO_ALARMA_ACTIVO_EN_CAMBIO
            if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                nvs.begin("Configs",false);
                nvs.putFloat("MIN_ALARMA",configuracion.alarma.minimoConfiguracion);
                nvs.end();
                configuracion.alarma.minimo = configuracion.alarma.minimoConfiguracion;
                estadoMenuAvanzado = MINIMO_ALARMA_ACTIVO_EN_CAMBIO;
                botonesTerminados[INDICE_ENTER] = false;
            } 

            // Si se aprieta Atras se vuelve a MINIMO_ALARMA_ESPERANDO
            else if (botonesConfiguracion[INDICE_ATRAS].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                estadoMenuAvanzado = MINIMO_ALARMA_ESPERANDO;
                botonesTerminados[INDICE_ATRAS] = false;
                }

            // Si se aprieta Mas o Menos se cambia el valor de configuracionconfiguracion.alarma.minimoConfiguracion
            else{
                cambioValorContinuo(&configuracion.alarma.minimoConfiguracion, MAYOR_MIN_ALARMA, MENOR_MIN_ALARMA, MINIMO_CAMBIO_SP, MAXIMO_CAMBIO_SP, false);
            }

            break;
        case MINIMO_ALARMA_ACTIVO_EN_CAMBIO:

            // Si se aprieta ENTER se guarda la configuracion y se vuelve a MINIMO_ALARMA_ESPERANDO
            if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                nvs.begin("Configs",false);
                nvs.putBool("MIN_ALAR_ACT", configuracion.alarma.minimoActivo);
                nvs.end();
                botonesTerminados[INDICE_ENTER] = false;
                estadoMenuAvanzado = MINIMO_ALARMA_ESPERANDO;

            }

            // Si se aprieta Atras se vuelve al modo MINIMO_ALARMA_EN_CAMBIO
            else if (botonesConfiguracion[INDICE_ATRAS].estado == PRESIONADO && botonesTerminados[INDICE_ATRAS] == true){
                estadoMenuAvanzado = MINIMO_ALARMA_EN_CAMBIO;
                botonesTerminados[INDICE_ATRAS] = false;
            }

            // Si se aprieta MAS se habilita o deshabilita el MINIMO_ALARMA
            else if (botonesConfiguracion[INDICE_MAS].estado == PRESIONADO && botonesTerminados[INDICE_MAS] == true){
                if (configuracion.alarma.minimoActivo == false){
                    configuracion.alarma.minimoActivo = true;
                } else {
                    configuracion.alarma.minimoActivo = false;
                }

                botonesTerminados[INDICE_MAS] = false;
            } 

            // Si se aprieta MENOS se habilita o deshabilita el MINIMO_ALARMA
            else if (botonesConfiguracion[INDICE_MENOS].estado == PRESIONADO && botonesTerminados[INDICE_MENOS] == true){
                if (configuracion.alarma.minimoActivo == false){
                    configuracion.alarma.minimoActivo = true;
                } else {
                    configuracion.alarma.minimoActivo = false;
                }

                botonesTerminados[INDICE_MENOS] = false;
            } 
            
            break;
        case MAXIMO_ALARMA_ESPERANDO:
             // Si se aprieta Enter se pasa al estado MAXIMO_ALARMA_EN_CAMBIO
            if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                estadoMenuAvanzado = MAXIMO_ALARMA_EN_CAMBIO;
                botonesTerminados[INDICE_ENTER] = false;
            } 
            
            // Si se aprieta Atras se vuelve al modo IDLE de estadoMenuAvanzado y de estadoConfiguraciones
            else if (botonesConfiguracion[INDICE_ATRAS].estado == PRESIONADO && botonesTerminados[INDICE_ATRAS] == true){
                estadoMenuAvanzado = IDLE_AVANZADO;
                estadoConfiguraciones = IDLE_CONFIGURACION;
                botonesTerminados[INDICE_ATRAS] = false;
            }

            // Si se aprieta menos con el boton que ya haya terminado, se pasa al modo MINIMO_ALARMA_ESPERANDO
            else if (botonesConfiguracion[INDICE_MENOS].estado == PRESIONADO && botonesTerminados[INDICE_MENOS] == true){
                estadoMenuAvanzado = MINIMO_ALARMA_ESPERANDO;
                botonesTerminados[INDICE_MENOS] = false;
            }


            // Si se aprieta mas con el boton que ya haya terminado, se pasa al modo CONFIG_SENSORES
            else if (botonesConfiguracion[INDICE_MAS].estado == PRESIONADO && botonesTerminados[INDICE_MAS] == true){
                estadoMenuAvanzado = VERSION_SOFTWARE;
                botonesTerminados[INDICE_MAS] = false;
            }
            break;
        case MAXIMO_ALARMA_EN_CAMBIO:
            // Si se aprieta Enter se guarda el valor minimo y pasa al estado MAXIMO_ALARMA_ACTIVO_EN_CAMBIO
            if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                nvs.begin("Configs",false);
                nvs.putFloat("MAX_ALARMA",configuracion.alarma.maximoConfiguracion);
                nvs.end();
                configuracion.alarma.maximo = configuracion.alarma.maximoConfiguracion;
                estadoMenuAvanzado = MAXIMO_ALARMA_ACTIVO_EN_CAMBIO;
                botonesTerminados[INDICE_ENTER] = false;
            } 

            // Si se aprieta Atras se vuelve a MAXIMO_ALARMA_ESPERANDO
            else if (botonesConfiguracion[INDICE_ATRAS].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                estadoMenuAvanzado = MAXIMO_ALARMA_ESPERANDO;
                botonesTerminados[INDICE_ATRAS] = false;
                }

            // Si se aprieta Mas o Menos se cambia el valor de configuracionconfiguracion.alarma.maximoConfiguracion
            else{
                cambioValorContinuo(&configuracion.alarma.maximoConfiguracion, MAYOR_MAX_ALARMA, MENOR_MAX_ALARMA, MINIMO_CAMBIO_SP, MAXIMO_CAMBIO_SP, false);
            }

            break;
        case MAXIMO_ALARMA_ACTIVO_EN_CAMBIO:
            // Si se aprieta ENTER se guarda la configuracion y se vuelve a MAXIMO_ALARMA_ESPERANDO
            if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                nvs.begin("Configs",false);
                nvs.putBool("MAX_ALAR_ACT", configuracion.alarma.maximoActivo);
                nvs.end();
                botonesTerminados[INDICE_ENTER] = false;
                estadoMenuAvanzado = MAXIMO_ALARMA_ESPERANDO;

            }

            // Si se aprieta Atras se vuelve al modo MAXIMO_ALARMA_EN_CAMBIO
            else if (botonesConfiguracion[INDICE_ATRAS].estado == PRESIONADO && botonesTerminados[INDICE_ATRAS] == true){
                estadoMenuAvanzado = MAXIMO_ALARMA_EN_CAMBIO;
                botonesTerminados[INDICE_ATRAS] = false;
            }

            // Si se aprieta MAS se habilita o deshabilita el MAXIMO_ALARMA
            else if (botonesConfiguracion[INDICE_MAS].estado == PRESIONADO && botonesTerminados[INDICE_MAS] == true){
                if (configuracion.alarma.maximoActivo == false){
                    configuracion.alarma.maximoActivo = true;
                } else {
                    configuracion.alarma.maximoActivo = false;
                }

                botonesTerminados[INDICE_MAS] = false;
            } 

            // Si se aprieta MENOS se habilita o deshabilita el MAXIMO_ALARMA
            else if (botonesConfiguracion[INDICE_MENOS].estado == PRESIONADO && botonesTerminados[INDICE_MENOS] == true){
                if (configuracion.alarma.maximoActivo == false){
                    configuracion.alarma.maximoActivo = true;
                } else {
                    configuracion.alarma.maximoActivo = false;
                }

                botonesTerminados[INDICE_MENOS] = false;
            } 
            
            break;

        case VERSION_SOFTWARE:
            // Si se aprieta Atras se vuelve al modo IDLE de estadoMenuAvanzado y de estadoConfiguraciones
            if (botonesConfiguracion[INDICE_ATRAS].estado == PRESIONADO && botonesTerminados[INDICE_ATRAS] == true){
                estadoMenuAvanzado = IDLE_AVANZADO;
                estadoConfiguraciones = IDLE_CONFIGURACION;
                botonesTerminados[INDICE_ATRAS] = false;
            }

            // Si se aprieta menos se pasa al modo MAXIMO_ALARMA_ESPERANDO
            else if (botonesConfiguracion[INDICE_MENOS].estado == PRESIONADO && botonesTerminados[INDICE_MENOS] == true){
                estadoMenuAvanzado = MAXIMO_ALARMA_ESPERANDO;
                botonesTerminados[INDICE_MENOS] = false;
            }
            
            break;
        default: estadoMenuAvanzado = IDLE_AVANZADO;
    }
}

void actualizarMostrarParametros(){
    static int contador = -1;
    if (contador == 0){
        estadoMostrarParametros = MOSTRAR_SP1;
    } else if (contador == TIEMPO_BASICO_MS/(DELAY_LOOP_MS*CANT_ETAPAS)){
        estadoMostrarParametros = MOSTRAR_SP2;
    } else if (contador == 2*TIEMPO_BASICO_MS/(DELAY_LOOP_MS*CANT_ETAPAS)){
        estadoMostrarParametros = MOSTRAR_SP3;
    } else if (contador == 3*TIEMPO_BASICO_MS/(DELAY_LOOP_MS*CANT_ETAPAS)){
        estadoMostrarParametros = MOSTRAR_SP4;
    } else if (contador == 4*TIEMPO_BASICO_MS/(DELAY_LOOP_MS*CANT_ETAPAS)){
        estadoMostrarParametros = IDLE_MP;
        estadoConfiguraciones = IDLE_CONFIGURACION;
        contador = -2;
    }
    contador++;
}

void cambioValorContinuo(float *parametro, float maximo, float minimo, float minimoCambio, float maximoCambio, bool activacion){
    
    // Si se aprieta el boton MAS y es menor que el maximo se aumenta el valor
    if (botonesConfiguracion[INDICE_MAS].estado == PRESIONADO && *parametro < maximo){
        // Si esta la activacion sirve para llevar el valor a 0 y poder desactivar el *parametro
        // LLeva de 0.0 al minimo
        if (*parametro == 0.0 && activacion == true){
            *parametro = minimo;
        } else{
            if (botonesConfiguracion[INDICE_MAS].tiempoPresionadoMs < UMBRAL_CAMBIO_RAPIDO_MS){
                if (botonesConfiguracion[INDICE_MAS].tiempoPresionadoMs % (UMBRAL_CAMBIO_RAPIDO_MS/CANT_CAMBIOS_UMBRAL) == 0){
                    *parametro = *parametro + MINIMO_CAMBIO_SP;
                }
            } else if (botonesConfiguracion[INDICE_MAS].tiempoPresionadoMs < 2 * UMBRAL_CAMBIO_RAPIDO_MS) {
                if (botonesConfiguracion[INDICE_MAS].tiempoPresionadoMs % (5 * TIEMPO_CAMBIO_RAPIDO_MS) == 0){
                    *parametro = *parametro + MAXIMO_CAMBIO_SP;
                }
            } else {
                if (botonesConfiguracion[INDICE_MAS].tiempoPresionadoMs % TIEMPO_CAMBIO_RAPIDO_MS == 0){
                    *parametro = *parametro + MAXIMO_CAMBIO_SP;
                }
            }

            // Por redondeo puede ser que se pase del maximo, entonces se asegura de que no supere
            if (*parametro > maximo){
                *parametro = maximo;
            }
        }
    }

    // Si se aprieta el MENOS y es mayor que el minimo
    if (botonesConfiguracion[INDICE_MENOS].estado == PRESIONADO && *parametro >= minimo){ 
        if (botonesConfiguracion[INDICE_MENOS].tiempoPresionadoMs < UMBRAL_CAMBIO_RAPIDO_MS){
            if (botonesConfiguracion[INDICE_MENOS].tiempoPresionadoMs % (UMBRAL_CAMBIO_RAPIDO_MS/CANT_CAMBIOS_UMBRAL) == 0){
                *parametro = *parametro - MINIMO_CAMBIO_SP;
            }
        } else if (botonesConfiguracion[INDICE_MENOS].tiempoPresionadoMs < 2 * UMBRAL_CAMBIO_RAPIDO_MS) {
            if (botonesConfiguracion[INDICE_MENOS].tiempoPresionadoMs % (5 * TIEMPO_CAMBIO_RAPIDO_MS) == 0){
                *parametro = *parametro - MAXIMO_CAMBIO_SP;
            }
        } else {
            if (botonesConfiguracion[INDICE_MENOS].tiempoPresionadoMs % TIEMPO_CAMBIO_RAPIDO_MS == 0){
                *parametro = *parametro - MAXIMO_CAMBIO_SP;
            }
        }
        
        // Si se baja por mas del minimo se asegura que vaya justo al minimo o a 0.0 si esta la activacion
        if (*parametro < minimo){ 
            if (activacion == true){
                *parametro = 0.0;
            } else {
                *parametro = minimo;
            }
        }
    }
}