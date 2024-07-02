#include "contador.h"
#include "control.h"
#include "botones.h"
#include "configuraciones.h"
#include "interfaz_usuario.h"


#define TIEMPO_CONTADOR_MS 1
#define DELAY_CONTROL_MS 1000
#define DELAY_SENSORES_MS 100
#define DELAY_LOOP_MS 10


void setup() {
  Serial.begin(115200);
  inicializarSensores();
  inicializarReles();
  inicializarBotones();
  inicializarConfiguraciones();
  inicializarControles();
  inicializarInterfazUsuario();
  inicializarContador(TIEMPO_CONTADOR_MS);

}

void loop() {
  if (contadorIgualA(DELAY_SENSORES_MS, SENSORES)){
    leerDatosSensores();
  }
  if (contadorIgualA(DELAY_CONTROL_MS, CONTROL)){
    actualizarSensores();
    actualizarControles();
    actualizarReles();
  }
  if (contadorIgualA(DELAY_LOOP_MS, LOOP)){
    actualizarBotones();
    actualizarConfiguraciones();
    actualizarInterfazUsuario();
  }
}
