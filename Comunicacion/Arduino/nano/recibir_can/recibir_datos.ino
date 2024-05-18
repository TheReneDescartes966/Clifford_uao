#include <Wire.h>
#include <SPI.h>
#include <mcp2515.h>

MCP2515 mcp2515(10); // Pin CS del MCP2515

const uint32_t FILTER_ID = 0x121; // ID específico que queremos recibir

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
    // Verificar si el ID del mensaje coincide con el ID específico que queremos recibir
    if (canMsg.can_id == FILTER_ID) {
      // Mostrar la ID del mensaje y el número de bytes recibidos
      Serial.print("ID: ");
      Serial.print(canMsg.can_id, HEX);
      Serial.print(" DLC: ");
      Serial.println(canMsg.can_dlc);

      // Mostrar los datos recibidos como caracteres
      char receivedData[9];
      for (int i = 0; i < canMsg.can_dlc; i++) {
        receivedData[i] = (char)canMsg.data[i];
      }
      receivedData[canMsg.can_dlc] = '\0'; // Null-terminate the string

      Serial.print("Data: ");
      Serial.println(receivedData);

      // Extraer los valores numéricos del formato "[1023,1023,1023]"
      int values[3] = {0, 0, 0};
      sscanf(receivedData, "[%d,%d,%d]", &values[0], &values[1], &values[2]);

      // Mostrar los valores extraídos
      Serial.print("Value 1: ");
      Serial.println(values[0]);
      Serial.print("Value 2: ");
      Serial.println(values[1]);
      Serial.print("Value 3: ");
      Serial.println(values[2]);
    }
  }

  delay(100);
}
