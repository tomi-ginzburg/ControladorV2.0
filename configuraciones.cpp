#include "configuraciones.h"


// =====[Declaracion de defines privados]============

#define TIEMPO_BASICO_MS            2000
#define TIEMPO_AVANZADO_MS          5000

#define MINIMO_CAMBIO_SP            0.1
#define MAX_SP                      100
#define MIN_SP                      1
#define MINIMO_CAMBIO_HISTERESIS    0.5
#define MAX_HISTERESIS              5
#define MIN_HISTERESIS              0
#define MINIMO_CAMBIO_RETARDO       1
#define MAX_RETARDO                 5
#define MIN_RETARDO                 1


// =====[Declaracion de tipos de datos privados]=====
Preferences nvs;
configuracion_t configuracion;
estadoConfiguraciones_t estadoConfiguraciones;
estadoMenuBasico_t estadoMenuBasico;
estadoMenuAvanzado_t estadoMenuAvanzado;
estadoMostrarParametros_t estadoMostrarParametros;
estadoMostrarSensores_t estadoMostrarSensores;

const boton_t *botonesConfiguracion = NULL;

// botonesTerminados sirve para que al tocar una vez haga el cambio de estado pero no se vuelva a cambiar continuamente, sino que pare
bool botonesTerminados[CANTIDAD_BOTONES] = {true, true, true, true};

// =====[Declaracion de funciones privadas]=====

void inicializarValoresConfiguraciones();
void actualizarMenuBasico();
void actualizarMenuAvanzado();
void actualizarMostrarParametros();
void actualizarMostrarSensores();
void habilitarBotones(bool botonesTerminados[]);

// =====[Implementacion de funciones publicas]=====

void inicializarConfiguraciones(){
    inicializarValoresConfiguraciones();
    botonesConfiguracion = leerBotones();
    estadoConfiguraciones = IDLE_CONFIGURACION; 
    estadoMenuBasico = IDLE_BASICO;
    estadoMenuAvanzado = IDLE_AVANZADO;
    estadoMostrarParametros = IDLE_MP;
    estadoMostrarSensores = IDLE_MS;
}

void actualizarConfiguraciones(){
    
    // Se actualiza la habilitacion de los botones luego de los cambios de estado
    habilitarBotones(botonesTerminados);
    
    switch (estadoConfiguraciones){
        case IDLE_CONFIGURACION:
            // Si se aprieta el ENTER por mas de TIEMPO_AVANZADO_MS se entra al menu avanzado, y el basico vuelve a idle
            if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesConfiguracion[INDICE_ENTER].tiempoPresionadoMs > TIEMPO_AVANZADO_MS){
                estadoConfiguraciones = AVANZADO;
                estadoMenuBasico = IDLE_BASICO;
            } 
            // Si se aprieta por mas de TIEMPO_BASICO_MS, se entra al menu basico
            else if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesConfiguracion[INDICE_ENTER].tiempoPresionadoMs > TIEMPO_BASICO_MS){
                estadoConfiguraciones = BASICO;
            }
            // Si se aprieta por menos de TIEMPO_BASICO_MS, se entra al mostrar parametros
            else if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesConfiguracion[INDICE_ENTER].tiempoPresionadoMs < TIEMPO_BASICO_MS){
                estadoConfiguraciones = MOSTRAR_PARAMETROS;
            }
            // Si se aprieta mas o menos por menos de TIEMPO_BASICO_MS, se entra al mostrar sensores
            else if (botonesConfiguracion[INDICE_MAS].estado == PRESIONADO || (botonesConfiguracion[INDICE_MENOS].estado == PRESIONADO)){
                estadoConfiguraciones = MOSTRAR_SENSORES;
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

        case MOSTRAR_SENSORES: 
            actualizarMostrarSensores();
            break;

        default: estadoConfiguraciones = IDLE_CONFIGURACION;
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

const estadoMostrarSensores_t* leerEstadoMostrarSensores(){
    return &estadoMostrarSensores;
}

const configuracion_t* leerConfiguraciones(){
    return &configuracion;
}

// =====[Implementacion de funciones privadas]=====

void inicializarValoresConfiguraciones(){
    float valorConfiguracion;
    bool modoConfiguracion;
    nvs.begin("Configs", true);

    valorConfiguracion = nvs.getFloat("SP1",-1.0);
    if (valorConfiguracion != -1.0){
        configuracion.SP1.valor = valorConfiguracion;    
    } else {
        configuracion.SP1.valor = 0;
    }

    valorConfiguracion = nvs.getFloat("SP1-MIN",-1.0);
    if (valorConfiguracion != -1.0){
        configuracion.SP1.minimo = valorConfiguracion;    
    } else {
        configuracion.SP1.minimo = 1;
    }

    valorConfiguracion = nvs.getFloat("SP1-MAX",-1.0);
    if (valorConfiguracion != -1.0){
        configuracion.SP1.maximo = valorConfiguracion;    
    } else {
        configuracion.SP1.maximo = 100;
    }

     valorConfiguracion = nvs.getFloat("SP2",-1.0);
    if (valorConfiguracion != -1.0){
        configuracion.SP2.valor = valorConfiguracion;    
    } else {
        configuracion.SP2.valor = 0;
    }

    valorConfiguracion = nvs.getFloat("SP2-MIN",-1.0);
    if (valorConfiguracion != -1.0){
        configuracion.SP2.minimo = valorConfiguracion;    
    } else {
        configuracion.SP2.minimo = 1;
    }

    valorConfiguracion = nvs.getFloat("SP2-MAX",-1.0);
    if (valorConfiguracion != -1.0){
        configuracion.SP2.maximo = valorConfiguracion;    
    } else {
        configuracion.SP2.maximo = 100;
    }
    
    valorConfiguracion = nvs.getFloat("H1",-1.0);
    if (valorConfiguracion != -1.0){
        configuracion.histeresis1.valor = valorConfiguracion;    
    } else {
        configuracion.histeresis1.valor = 0.5;
    }
    
    valorConfiguracion = nvs.getFloat("H2",-1.0);
    if (valorConfiguracion != -1.0){
        configuracion.histeresis2.valor = valorConfiguracion;    
    } else {
        configuracion.histeresis2.valor = 1;
    }
    
    valorConfiguracion = nvs.getFloat("RET_PREN",-1.0);
    if (valorConfiguracion != -1.0){
        configuracion.retardoPrendido.valor = valorConfiguracion;    
    } else {
        configuracion.retardoPrendido.valor = 1;
    }
    
    valorConfiguracion = nvs.getFloat("RET_APAG",-1.0);
    if (valorConfiguracion != -1.0){
        configuracion.retardoApagado.valor = valorConfiguracion;    
    } else {
        configuracion.retardoApagado.valor = 0;
    }

    modoConfiguracion = nvs.getBool("Sensores",-1.0);
    if (valorConfiguracion != -1.0){
        configuracion.cambioSensores = modoConfiguracion;    
    } else {
        configuracion.cambioSensores = false;
    }
    
    modoConfiguracion = nvs.getBool("Diagnostico",-1.0);
    if (valorConfiguracion != -1.0){
        configuracion.modoDiagnostico = modoConfiguracion;    
    } else {
        configuracion.modoDiagnostico = false;
    }

    nvs.end();

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

void actualizarMenuBasico(){

    switch (estadoMenuBasico){
        case IDLE_BASICO: 
            // Inicia en SP1_ESTBLE
            if (estadoConfiguraciones == BASICO){
                estadoMenuBasico = SP1_ESTABLE;
                configuracion.SP1.valorConfiguracion = configuracion.SP1.valor;
                configuracion.SP2.valorConfiguracion = configuracion.SP2.valor;
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
                nvs.putFloat("SP1",configuracion.SP1.valorConfiguracion);
                nvs.end();
                configuracion.SP1.valor = configuracion.SP1.valorConfiguracion;
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
                if (botonesConfiguracion[INDICE_MAS].estado == PRESIONADO && configuracion.SP1.valorConfiguracion < configuracion.SP1.maximo){
                    configuracion.SP1.valorConfiguracion = configuracion.SP1.valorConfiguracion + MINIMO_CAMBIO_SP;
                }
                if (botonesConfiguracion[INDICE_MENOS].estado == PRESIONADO && configuracion.SP1.valorConfiguracion > configuracion.SP1.minimo){
                    configuracion.SP1.valorConfiguracion = configuracion.SP1.valorConfiguracion - MINIMO_CAMBIO_SP;
                }
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


            break;
        case SP2_EN_CAMBIO:

            // Si se aprieta el ENTER se guarda el valor y se vuelve a SP2_ESTABLE
            if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                nvs.begin("Configs",false);
                nvs.putFloat("SP2",configuracion.SP2.valorConfiguracion);
                nvs.end();
                configuracion.SP2.valor = configuracion.SP2.valorConfiguracion;
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
                if (botonesConfiguracion[INDICE_MAS].estado == PRESIONADO && configuracion.SP2.valorConfiguracion < configuracion.SP2.maximo){
                    configuracion.SP2.valorConfiguracion = configuracion.SP2.valorConfiguracion + MINIMO_CAMBIO_SP;
                }
                if (botonesConfiguracion[INDICE_MENOS].estado == PRESIONADO && configuracion.SP2.valorConfiguracion > configuracion.SP2.minimo){
                    configuracion.SP2.valorConfiguracion = configuracion.SP2.valorConfiguracion - MINIMO_CAMBIO_SP;
                }
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
                configuracion.SP1.maximoConfiguracion = configuracion.SP1.maximo;
                configuracion.SP1.minimoConfiguracion = configuracion.SP1.minimo;
                configuracion.SP2.maximoConfiguracion = configuracion.SP2.maximo;
                configuracion.SP2.minimoConfiguracion = configuracion.SP2.minimo;
                configuracion.histeresis1.valorConfiguracion = configuracion.histeresis1.valor;
                configuracion.histeresis2.valorConfiguracion = configuracion.histeresis2.valor;
                configuracion.retardoPrendido.valorConfiguracion = configuracion.retardoPrendido.valor;
                configuracion.retardoApagado.valorConfiguracion = configuracion.retardoApagado.valor;
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
                        estadoMenuAvanzado = SP1_ESPERANDO;
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
        case SP1_ESPERANDO:
            // Si se aprieta Enter se pasa al estado SP1_MAX_EN_CAMBIO
            if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                estadoMenuAvanzado = SP1_MAX_EN_CAMBIO;
                botonesTerminados[INDICE_ENTER] = false;
            } 
            
            // Si se aprieta Atras se vuelve al modo IDLE de estadoMenuAvanzado y de estadoConfiguraciones
            else if (botonesConfiguracion[INDICE_ATRAS].estado == PRESIONADO && botonesTerminados[INDICE_ATRAS] == true){
                estadoMenuAvanzado = IDLE_AVANZADO;
                estadoConfiguraciones = IDLE_CONFIGURACION;
                botonesTerminados[INDICE_ATRAS] = false;
            }

            // Si se aprieta mas con el boton que ya haya terminado, se pasa al modo SP2_ESPERANDO
            else if (botonesConfiguracion[INDICE_MAS].estado == PRESIONADO && botonesTerminados[INDICE_MAS] == true){
                estadoMenuAvanzado = SP2_ESPERANDO;
                botonesTerminados[INDICE_MAS] = false;
            }

            break;
        case SP1_MAX_EN_CAMBIO:
            // Si se aprieta Enter se guarda el valor maximo y pasa al estado SP1_MIN_EN_CAMBIO
            // En caso de que el maximo sea menor que el actual configurado, se cambia
            if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                nvs.begin("Configs",false);
                nvs.putFloat("SP1-MAX",configuracion.SP1.maximoConfiguracion);
                configuracion.SP1.maximo = configuracion.SP1.maximoConfiguracion;
                if (configuracion.SP1.maximo < configuracion.SP1.valor){
                    nvs.putFloat("SP1",configuracion.SP1.maximo);
                    configuracion.SP1.valor = configuracion.SP1.maximo;
                }
                nvs.end();
                estadoMenuAvanzado = SP1_MIN_EN_CAMBIO;
                botonesTerminados[INDICE_ENTER] = false;
            } 

            // Si se aprieta Atras se vuelve a SP1_ESPERANDO
            else if (botonesConfiguracion[INDICE_ATRAS].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                estadoMenuAvanzado = SP1_ESPERANDO;
                botonesTerminados[INDICE_ATRAS] = false;
                }

            // Si se aprieta Mas o Menos se cambia el valor de configuracionconfiguracion.SP1.maximo
            else{
                if (botonesConfiguracion[INDICE_MAS].estado == PRESIONADO && configuracion.SP1.maximoConfiguracion < MAX_SP){
                    configuracion.SP1.maximoConfiguracion = configuracion.SP1.maximoConfiguracion + MINIMO_CAMBIO_SP;
                }
                if (botonesConfiguracion[INDICE_MENOS].estado == PRESIONADO && configuracion.SP1.maximoConfiguracion > MIN_SP){
                    configuracion.SP1.maximoConfiguracion = configuracion.SP1.maximoConfiguracion - MINIMO_CAMBIO_SP;
                }
            }

            break;
        case SP1_MIN_EN_CAMBIO:
            // Si se aprieta Enter se guarda el valor minimo y vuelve al estado SP1_ESPERANDO
            // En caso de que el minimo sea mayor que el actual configurado, se cambia
            if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                nvs.begin("Configs",false);
                nvs.putFloat("SP1-MIN",configuracion.SP1.minimoConfiguracion);
                configuracion.SP1.minimo = configuracion.SP1.minimoConfiguracion;
                if (configuracion.SP1.minimo > configuracion.SP1.valor){
                    nvs.putFloat("SP1",configuracion.SP1.minimo);
                    configuracion.SP1.valor = configuracion.SP1.minimo;
                }
                nvs.end();
                estadoMenuAvanzado = SP1_ESPERANDO;
                botonesTerminados[INDICE_ENTER] = false;
            } 

            // Si se aprieta Atras se vuelve a SP1_ESPERANDO
            else if (botonesConfiguracion[INDICE_ATRAS].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                estadoMenuAvanzado = SP1_ESPERANDO;
                botonesTerminados[INDICE_ATRAS] = false;
                }

            // Si se aprieta Mas o Menos se cambia el valor de configuracionconfiguracion.SP1.minimo
            else{
                if (botonesConfiguracion[INDICE_MAS].estado == PRESIONADO && configuracion.SP1.minimoConfiguracion < MAX_SP){
                    configuracion.SP1.minimoConfiguracion = configuracion.SP1.minimoConfiguracion + MINIMO_CAMBIO_SP;
                }
                if (botonesConfiguracion[INDICE_MENOS].estado == PRESIONADO && configuracion.SP1.minimoConfiguracion > MIN_SP){
                    configuracion.SP1.minimoConfiguracion = configuracion.SP1.minimoConfiguracion - MINIMO_CAMBIO_SP;
                }
            }

            break;
        case SP2_ESPERANDO:
            // Si se aprieta Enter se pasa al estado SP2_MAX_EN_CAMBIO
            if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                estadoMenuAvanzado = SP2_MAX_EN_CAMBIO;
                botonesTerminados[INDICE_ENTER] = false;
            } 
            
            // Si se aprieta Atras se vuelve al modo IDLE de estadoMenuAvanzado y de estadoConfiguraciones
            else if (botonesConfiguracion[INDICE_ATRAS].estado == PRESIONADO && botonesTerminados[INDICE_ATRAS] == true){
                estadoMenuAvanzado = IDLE_AVANZADO;
                estadoConfiguraciones = IDLE_CONFIGURACION;
                botonesTerminados[INDICE_ATRAS] = false;
            }

            // Si se aprieta menos con el boton que ya haya terminado, se pasa al modo SP1_ESPERANDO
            else if (botonesConfiguracion[INDICE_MENOS].estado == PRESIONADO && botonesTerminados[INDICE_MENOS] == true){
                estadoMenuAvanzado = SP1_ESPERANDO;
                botonesTerminados[INDICE_MENOS] = false;
            }


            // Si se aprieta mas con el boton que ya haya terminado, se pasa al modo HISTERESIS_ESPERANDO
            else if (botonesConfiguracion[INDICE_MAS].estado == PRESIONADO && botonesTerminados[INDICE_MAS] == true){
                estadoMenuAvanzado = HISTERESIS_ESPERANDO;
                botonesTerminados[INDICE_MAS] = false;
            }

            break;
        case SP2_MAX_EN_CAMBIO:
            // Si se aprieta Enter se guarda el valor maximo y pasa al estado SP2_MIN_EN_CAMBIO
            // En caso de que el maximo sea menor que el actual configurado, se cambia
            if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                nvs.begin("Configs",false);
                nvs.putFloat("SP2-MAX",configuracion.SP2.maximoConfiguracion);
                configuracion.SP2.maximo = configuracion.SP2.maximoConfiguracion;
                if (configuracion.SP2.maximo < configuracion.SP2.valor){
                    nvs.putFloat("SP2",configuracion.SP2.maximo);
                    configuracion.SP2.valor = configuracion.SP2.maximo;
                }
                nvs.end();
                estadoMenuAvanzado = SP2_MIN_EN_CAMBIO;
                botonesTerminados[INDICE_ENTER] = false;
            } 

            // Si se aprieta Atras se vuelve a SP2_ESPERANDO
            else if (botonesConfiguracion[INDICE_ATRAS].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                estadoMenuAvanzado = SP2_ESPERANDO;
                botonesTerminados[INDICE_ATRAS] = false;
                }

            // Si se aprieta Mas o Menos se cambia el valor de configuracionconfiguracion.SP2.maximo
            else{
                if (botonesConfiguracion[INDICE_MAS].estado == PRESIONADO && configuracion.SP2.maximoConfiguracion < MAX_SP){
                    configuracion.SP2.maximoConfiguracion = configuracion.SP2.maximoConfiguracion + MINIMO_CAMBIO_SP;
                }
                if (botonesConfiguracion[INDICE_MENOS].estado == PRESIONADO && configuracion.SP2.maximoConfiguracion > MIN_SP){
                    configuracion.SP2.maximoConfiguracion = configuracion.SP2.maximoConfiguracion - MINIMO_CAMBIO_SP;
                }
            }

            break;
        case SP2_MIN_EN_CAMBIO:
            // Si se aprieta Enter se guarda el valor minimo y vuelve al estado SP1_ESPERANDO
            // En caso de que el minimo sea mayor que el actual configurado, se cambia
            if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                nvs.begin("Configs",false);
                nvs.putFloat("SP2-MIN",configuracion.SP2.minimoConfiguracion);
                configuracion.SP2.minimo = configuracion.SP2.minimoConfiguracion;
                if (configuracion.SP2.minimo > configuracion.SP2.valor){
                    nvs.putFloat("SP2",configuracion.SP2.minimo);
                    configuracion.SP2.valor = configuracion.SP2.minimo;
                }
                nvs.end();
                estadoMenuAvanzado = SP2_ESPERANDO;
                botonesTerminados[INDICE_ENTER] = false;
            } 

            // Si se aprieta Atras se vuelve a SP1_ESPERANDO
            else if (botonesConfiguracion[INDICE_ATRAS].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                estadoMenuAvanzado = SP2_ESPERANDO;
                botonesTerminados[INDICE_ATRAS] = false;
                }

            // Si se aprieta Mas o Menos se cambia el valor de configuracionMinSP2
            else{
                if (botonesConfiguracion[INDICE_MAS].estado == PRESIONADO && configuracion.SP2.minimoConfiguracion < MAX_SP){
                    configuracion.SP2.minimoConfiguracion = configuracion.SP2.minimoConfiguracion + MINIMO_CAMBIO_SP;
                }
                if (botonesConfiguracion[INDICE_MENOS].estado == PRESIONADO && configuracion.SP2.minimoConfiguracion > MIN_SP){
                    configuracion.SP2.minimoConfiguracion = configuracion.SP2.minimoConfiguracion - MINIMO_CAMBIO_SP;
                }
            }

            break;
        case HISTERESIS_ESPERANDO:
            // Si se aprieta Enter se pasa al estado H1_EN_CAMBIO
            if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                estadoMenuAvanzado = H1_EN_CAMBIO;
                botonesTerminados[INDICE_ENTER] = false;
            } 
            
            // Si se aprieta Atras se vuelve al modo IDLE de estadoMenuAvanzado y de estadoConfiguraciones
            else if (botonesConfiguracion[INDICE_ATRAS].estado == PRESIONADO && botonesTerminados[INDICE_ATRAS] == true){
                estadoMenuAvanzado = IDLE_AVANZADO;
                estadoConfiguraciones = IDLE_CONFIGURACION;
                botonesTerminados[INDICE_ATRAS] = false;
            }

            // Si se aprieta menos con el boton que ya haya terminado, se pasa al modo SP2_ESPERANDO
            else if (botonesConfiguracion[INDICE_MENOS].estado == PRESIONADO && botonesTerminados[INDICE_MENOS] == true){
                estadoMenuAvanzado = SP2_ESPERANDO;
                botonesTerminados[INDICE_MENOS] = false;
            }


            // Si se aprieta mas con el boton que ya haya terminado, se pasa al modo CONFIG_SENSORES
            else if (botonesConfiguracion[INDICE_MAS].estado == PRESIONADO && botonesTerminados[INDICE_MAS] == true){
                estadoMenuAvanzado = RETARDOS_ESPERANDO;
                botonesTerminados[INDICE_MAS] = false;
            }

            break;
        case H1_EN_CAMBIO:
            // Si se aprieta Enter se guarda el valor y pasa al estado H2_EN_CAMBIO
            if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                nvs.begin("Configs",false);
                nvs.putFloat("H1",configuracion.histeresis1.valorConfiguracion);
                configuracion.histeresis1.valor = configuracion.histeresis1.valorConfiguracion;
                nvs.end();
                estadoMenuAvanzado = H2_EN_CAMBIO;
                botonesTerminados[INDICE_ENTER] = false;
            } 

            // Si se aprieta Atras se vuelve a HISTERESIS_ESPERANDO
            else if (botonesConfiguracion[INDICE_ATRAS].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                estadoMenuAvanzado = HISTERESIS_ESPERANDO;
                botonesTerminados[INDICE_ATRAS] = false;
                }

            // Si se aprieta Mas o Menos se cambia el valor de configuracion
            else{
                if (botonesConfiguracion[INDICE_MAS].estado == PRESIONADO && botonesTerminados[INDICE_MAS] == true && configuracion.histeresis1.valorConfiguracion < configuracion.histeresis2.valorConfiguracion){
                    configuracion.histeresis1.valorConfiguracion = configuracion.histeresis1.valorConfiguracion + MINIMO_CAMBIO_HISTERESIS;
                    botonesTerminados[INDICE_MAS] = false;
                }
                if (botonesConfiguracion[INDICE_MENOS].estado == PRESIONADO && botonesTerminados[INDICE_MENOS] == true && configuracion.histeresis1.valorConfiguracion > MIN_HISTERESIS){
                    configuracion.histeresis1.valorConfiguracion = configuracion.histeresis1.valorConfiguracion - MINIMO_CAMBIO_HISTERESIS;
                    botonesTerminados[INDICE_MENOS] = false;
                }
            }

            break;
        case H2_EN_CAMBIO:
            // Si se aprieta Enter se guarda el valor y vuelve al estado HISTERESIS_ESPERANDO
            if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                nvs.begin("Configs",false);
                nvs.putFloat("H2",configuracion.histeresis2.valorConfiguracion);
                configuracion.histeresis2.valor = configuracion.histeresis2.valorConfiguracion;
                nvs.end();
                estadoMenuAvanzado = HISTERESIS_ESPERANDO;
                botonesTerminados[INDICE_ENTER] = false;
            } 

            // Si se aprieta Atras se vuelve a HISTERESIS_ESPERANDO
            else if (botonesConfiguracion[INDICE_ATRAS].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                estadoMenuAvanzado = HISTERESIS_ESPERANDO;
                botonesTerminados[INDICE_ATRAS] = false;
                }

            // Si se aprieta Mas o Menos se cambia el valor de configuracionMinSP2
            else{
                if (botonesConfiguracion[INDICE_MAS].estado == PRESIONADO && botonesTerminados[INDICE_MAS] == true && configuracion.histeresis2.valorConfiguracion < MAX_HISTERESIS){
                    configuracion.histeresis2.valorConfiguracion = configuracion.histeresis2.valorConfiguracion + MINIMO_CAMBIO_HISTERESIS;
                    botonesTerminados[INDICE_MAS] = false;
                }
                if (botonesConfiguracion[INDICE_MENOS].estado == PRESIONADO && botonesTerminados[INDICE_MENOS] == true && configuracion.histeresis2.valorConfiguracion > configuracion.histeresis1.valorConfiguracion){
                    configuracion.histeresis2.valorConfiguracion = configuracion.histeresis2.valorConfiguracion - MINIMO_CAMBIO_HISTERESIS;
                    botonesTerminados[INDICE_MENOS] = false;
                }
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
                estadoMenuAvanzado = HISTERESIS_ESPERANDO;
                botonesTerminados[INDICE_MENOS] = false;
            }


            // Si se aprieta mas con el boton que ya haya terminado, se pasa al modo CONFIG_SENSORES
            else if (botonesConfiguracion[INDICE_MAS].estado == PRESIONADO && botonesTerminados[INDICE_MAS] == true){
                estadoMenuAvanzado = CONFIG_SENSORES;
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
        case CONFIG_SENSORES:
            // Si se aprieta Enter se habilita o deshabilita el cambioSensores
            if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                if (configuracion.cambioSensores == false){
                    configuracion.cambioSensores = true;
                } else {
                    configuracion.cambioSensores = false;
                }
                nvs.begin("Configs",false);
                nvs.putBool("Sensores", configuracion.cambioSensores);
                nvs.end();
                botonesTerminados[INDICE_ENTER] = false;
            } 

            // Si se aprieta Atras se vuelve al modo IDLE de estadoMenuAvanzado y de estadoConfiguraciones
            else if (botonesConfiguracion[INDICE_ATRAS].estado == PRESIONADO && botonesTerminados[INDICE_ATRAS] == true){
                estadoMenuAvanzado = IDLE_AVANZADO;
                estadoConfiguraciones = IDLE_CONFIGURACION;
                botonesTerminados[INDICE_ATRAS] = false;
            }

            // Si se aprieta menos se pasa al modo RETARDOS_ESPERANDO
            else if (botonesConfiguracion[INDICE_MENOS].estado == PRESIONADO && botonesTerminados[INDICE_MENOS] == true){
                estadoMenuAvanzado = RETARDOS_ESPERANDO;
                botonesTerminados[INDICE_MENOS] = false;
            }


             // Si se aprieta mas se pasa al modo MODO_DIAGNOSTICO
            else if (botonesConfiguracion[INDICE_MAS].estado == PRESIONADO && botonesTerminados[INDICE_MAS] == true){
                estadoMenuAvanzado = MODO_DIAGNOSTICO;
                botonesTerminados[INDICE_MAS] = false;
            }
            
            break;
        case MODO_DIAGNOSTICO:
            // Si se aprieta Enter se cambia el modoDiagnostico
            if (botonesConfiguracion[INDICE_ENTER].estado == PRESIONADO && botonesTerminados[INDICE_ENTER] == true){
                if (configuracion.modoDiagnostico == false ){
                    configuracion.modoDiagnostico = true;
                } else {
                    configuracion.modoDiagnostico = false;
                }
                nvs.begin("Configs",false);
                nvs.putBool("Diagnostico", configuracion.cambioSensores);
                nvs.end();
                botonesTerminados[INDICE_ENTER] = false;
            } 

            // Si se aprieta Atras se vuelve al modo IDLE de estadoMenuAvanzado y de estadoConfiguraciones
            else if (botonesConfiguracion[INDICE_ATRAS].estado == PRESIONADO && botonesTerminados[INDICE_ATRAS] == true){
                estadoMenuAvanzado = IDLE_AVANZADO;
                estadoConfiguraciones = IDLE_CONFIGURACION;
                botonesTerminados[INDICE_ATRAS] = false;
            }

            // Si se aprieta menos se pasa al modo CONFIG_SENSORES
            else if (botonesConfiguracion[INDICE_MENOS].estado == PRESIONADO && botonesTerminados[INDICE_MENOS] == true){
                estadoMenuAvanzado = CONFIG_SENSORES;
                botonesTerminados[INDICE_MENOS] = false;
            }
            
            // Si se aprieta mas se pasa al modo VERSION_SOFTWARE
            else if (botonesConfiguracion[INDICE_MAS].estado == PRESIONADO && botonesTerminados[INDICE_MAS] == true){
                estadoMenuAvanzado = VERSION_SOFTWARE;
                botonesTerminados[INDICE_MAS] = false;
            }
            break;
        case VERSION_SOFTWARE:
            // Si se aprieta Atras se vuelve al modo IDLE de estadoMenuAvanzado y de estadoConfiguraciones
            if (botonesConfiguracion[INDICE_ATRAS].estado == PRESIONADO && botonesTerminados[INDICE_ATRAS] == true){
                estadoMenuAvanzado = IDLE_AVANZADO;
                estadoConfiguraciones = IDLE_CONFIGURACION;
                botonesTerminados[INDICE_ATRAS] = false;
            }

            // Si se aprieta menos se pasa al modo MODO_DIAGNOSTICO
            else if (botonesConfiguracion[INDICE_MENOS].estado == PRESIONADO && botonesTerminados[INDICE_MENOS] == true){
                estadoMenuAvanzado = MODO_DIAGNOSTICO;
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
    } else if (contador == 100){
        estadoMostrarParametros = MOSTRAR_SP2;
    } else if (contador == 200){
        estadoMostrarParametros = IDLE_MP;
        estadoConfiguraciones = IDLE_CONFIGURACION;
        contador = -2;
    }
    contador++;
}

void actualizarMostrarSensores(){
    static int contador = 0;
    if (contador == 0){
        estadoMostrarSensores = MOSTRAR_SENSOR2;
    } else if (contador == 500){
        estadoMostrarSensores = IDLE_MS;
        estadoConfiguraciones = IDLE_CONFIGURACION;
        contador = -1;
    }
    contador++;
}