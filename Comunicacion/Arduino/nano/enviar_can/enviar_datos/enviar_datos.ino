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
    // Verificar que el mensaje tiene la longitud esperada (16 caracteres)
    if (message.length() == 16) {
      sendHelloWorld(message.c_str());
    } else {
      Serial.println("Error: Mensaje inválido, debe ser de 16 caracteres");
    }
  }
}

void sendHelloWorld(const char* message) {
  struct can_frame canMsg1;
  struct can_frame canMsg2;

  canMsg1.can_id = 0x121; // ID del mensaje CAN para la primera parte
  canMsg2.can_id = 0x122; // ID del mensaje CAN para la segunda parte

  canMsg1.can_dlc = 8; // Número de bytes en la primera parte del mensaje
  canMsg2.can_dlc = 8; // Número de bytes en la segunda parte del mensaje

  // Copiar los primeros 8 bytes del mensaje en el array de datos de la primera trama
  for (int i = 0; i < 8; i++) {
    canMsg1.data[i] = message[i];
  }

  // Copiar los siguientes 8 bytes del mensaje en el array de datos de la segunda trama
  for (int i = 0; i < 8; i++) {
    canMsg2.data[i] = message[i + 8];
  }

  // Enviar las dos partes del mensaje
  mcp2515.sendMessage(&canMsg1);
  mcp2515.sendMessage(&canMsg2);

  Serial.println("Mensaje enviado en dos tramas CAN");
}
