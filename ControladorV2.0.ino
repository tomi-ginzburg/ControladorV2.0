#include "contador.h"
#include "control.h"
#include "buzzer.h"
#include "botones.h"
#include "configuraciones.h"
#include "interfaz_usuario.h"


#define TIEMPO_CONTADOR_MS  1
#define DELAY_CONTROL_MS    1000
#define DELAY_IU_MS         50
#define DELAY_LOOP_MS       10 // Tiene que tener sentido con DELAY_CONTROL_MS y CANTIDAD_MUESTRAS en el modulo de sensores


void setup() {
  Serial.begin(115200);
  inicializarSensores();
  inicializarReles();
  inicializarBuzzer();
  inicializarBotones();
  inicializarConfiguraciones();
  inicializarControles();
  inicializarInterfazUsuario();
  inicializarContador(TIEMPO_CONTADOR_MS);

}

void loop() {
  
  if (contadorIgualA(DELAY_CONTROL_MS, CONTROL)){
    actualizarSensores();
    actualizarControles();
    actualizarReles();
  }
  if (contadorIgualA(DELAY_LOOP_MS, LOOP)){
    leerDatosSensores();
    actualizarBotones();
    actualizarBuzzer();
    actualizarConfiguraciones();
  }
  if (contadorIgualA(DELAY_IU_MS, IU)){
    actualizarInterfazUsuario();
  }
}
