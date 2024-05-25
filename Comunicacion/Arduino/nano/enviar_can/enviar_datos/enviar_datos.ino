#include <Wire.h>
#include <SPI.h>
#include <mcp2515.h>

MCP2515 mcp2515(10); // Pin CS del MCP2515

void setup() {
  Serial.begin(115200);

  // Inicialización del MCP2515
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ); // Configurar la velocidad de comunicación y la frecuencia del cristal
  mcp2515.setNormalMode(); // Modo normal de operación

  Serial.println("MCP2515 Initialized");
}

void loop() {
  // Chequear si hay datos disponibles en el puerto serial
  if (Serial.available()) {
    // Leer el mensaje del puerto serial
    String message = Serial.readStringUntil('\n');
    // Verificar que el mensaje tiene la longitud esperada (64 caracteres)
    if (message.length() == 64) {
      sendMessageInFrames(message.c_str());
    } else {
      Serial.println("Error: Mensaje inválido, debe ser de 64 caracteres");
    }
  }
}

void sendMessageInFrames(const char* message) {
  struct can_frame canMsg[8]; // Arreglo para las 8 tramas CAN
  uint16_t can_ids[8] = {0x121, 0x122, 0x123, 0x124, 0x125, 0x126, 0x127, 0x128}; // IDs para las tramas

  for (int i = 0; i < 8; i++) {
    canMsg[i].can_id = can_ids[i]; // Asignar el ID correspondiente
    canMsg[i].can_dlc = 8; // Cada trama tiene 8 bytes

    // Copiar los 8 bytes correspondientes del mensaje en el array de datos de la trama
    for (int j = 0; j < 8; j++) {
      canMsg[i].data[j] = message[i * 8 + j];
    }

    // Enviar la trama
    mcp2515.sendMessage(&canMsg[i]);

    // Imprimir mensaje de confirmación
    Serial.print("Mensaje enviado en trama CAN ");
    Serial.println(i + 1);
  }
}
