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
  
  // Enviar el mensaje "Hola Mundo"
 
}

void loop() {
  // En el loop principal no hacemos nada
  sendHelloWorld();
}

void sendHelloWorld() {
  struct can_frame canMsg;
  
  canMsg.can_id = 0x121; // ID del mensaje CAN
  const char* message = "diego   ";
  canMsg.can_dlc = 8; // Número de bytes en el mensaje (máximo 8 bytes)
  
  // Copiar los primeros 8 bytes del mensaje en el array de datos
  for (int i = 0; i < 8; i++) {
    canMsg.data[i] = message[i];
  }

  // Enviar el mensaje
  mcp2515.sendMessage(&canMsg);

  Serial.println("Mensaje 'Hola Mundo' enviado");
}
