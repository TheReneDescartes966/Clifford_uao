#include <Wire.h>
#include <SPI.h>
#include <mcp2515.h>

MCP2515 mcp2515(10); // Pin CS del MCP2515

const uint16_t can_ids[8] = {0x121, 0x122, 0x123, 0x124, 0x125, 0x126, 0x127, 0x128}; // IDs para las tramas

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
  char receivedMessage[65] = {0}; // Para almacenar el mensaje completo de 64 caracteres

  // Contador para saber cuántas tramas hemos recibido
  int framesReceived = 0;

  // Loop para recibir las 8 tramas
  while (framesReceived < 8) {
    // Chequear si hay un nuevo mensaje CAN
    if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
      // Verificar si el ID del mensaje está en nuestro arreglo de IDs
      for (int i = 0; i < 8; i++) {
        if (canMsg.can_id == can_ids[i]) {
          // Copiar los datos recibidos al mensaje completo en la posición correcta
          for (int j = 0; j < 8; j++) {
            receivedMessage[i * 8 + j] = canMsg.data[j];
          }
          framesReceived++;
          break;
        }
      }
    }
    delay(10); // Pequeña demora para evitar sobrecarga del loop
  }

  // Mostrar el mensaje completo recibido
  Serial.print("Mensaje completo recibido: ");
  Serial.println(receivedMessage);
}
