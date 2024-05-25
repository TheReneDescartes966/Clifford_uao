#include <Wire.h>
#include <SPI.h>
#include <mcp2515.h>

MCP2515 mcp2515(10); // Pin CS del MCP2515

const uint16_t can_ids[8] = {0x121, 0x122, 0x123, 0x124, 0x125, 0x126, 0x127, 0x128}; // IDs para las tramas
bool framesReceived[8] = {false}; // Bandera para saber si cada trama ha sido recibida
char receivedMessage[65] = {0}; // Para almacenar el mensaje completo de 64 caracteres

void setup() {
  Serial.begin(115200);

  // Inicialización del MCP2515
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ); // Configurar la velocidad de comunicación y la frecuencia del cristal
  mcp2515.setNormalMode(); // Modo normal de operación

  Serial.println("MCP2515 Initialized");
}

void loop() {
  struct can_frame canMsg;

  // Chequear si hay un nuevo mensaje CAN
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
    // Verificar si el ID del mensaje está en nuestro arreglo de IDs
    for (int i = 0; i < 8; i++) {
      if (canMsg.can_id == can_ids[i]) {
        // Copiar los datos recibidos al mensaje completo en la posición correcta
        for (int j = 0; j < 8; j++) {
          receivedMessage[i * 8 + j] = canMsg.data[j];
        }
        framesReceived[i] = true; // Marcar la trama como recibida
        break;
      }
    }

    // Verificar si todas las tramas han sido recibidas
    bool allReceived = true;
    for (int i = 0; i < 8; i++) {
      if (!framesReceived[i]) {
        allReceived = false;
        break;
      }
    }

    if (allReceived) {
      // Mostrar el mensaje completo recibido
      Serial.print("Mensaje completo recibido: ");
      Serial.println(receivedMessage);

      // Reiniciar las banderas y limpiar el mensaje para recibir un nuevo mensaje
      for (int i = 0; i < 8; i++) {
        framesReceived[i] = false;
      }
      memset(receivedMessage, 0, sizeof(receivedMessage));
    }
  }

  delay(10); // Pequeña demora para evitar sobrecarga del loop
}
