#include <Wire.h>
#include <SPI.h>
#include <mcp2515.h>
#include <AX12A.h>
#define DirectionPin  (12u)
#define BaudRate    (1000000ul)
#define ID    (5u)
#define ID2    (7u)
#define ID3   (11u)
#define switchCom(DirPin,Mode)		(digitalWrite(DirPin,Mode)) 

MCP2515 mcp2515(10); // Pin CS del MCP2515

const uint32_t FILTER_ID1 = 0x121; // ID específico de la primera parte del mensaje
const uint32_t FILTER_ID2 = 0x122; // ID específico de la segunda parte del mensaje

char receivedData[17]; // Buffer para almacenar el mensaje completo de 16 bytes
char option;

void setup() {
  Serial.begin(1000000); 
  pinMode(LED_BUILTIN, OUTPUT);
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ); // Configurar la velocidad de comunicación y la frecuencia del cristal
  mcp2515.setNormalMode(); 
}

void loop() {
  struct can_frame canMsg;

  // Chequear si hay un nuevo mensaje CAN
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
    // Verificar si el ID del mensaje coincide con alguno de los IDs específicos que queremos recibir
    if (canMsg.can_id == FILTER_ID1) {
      // Copiar los primeros 8 bytes del mensaje recibido
      for (int i = 0; i < canMsg.can_dlc; i++) {
        receivedData[i] = (char)canMsg.data[i];
      }
      Serial.println("Primera parte recibida");
    } else if (canMsg.can_id == FILTER_ID2) {
      // Copiar los siguientes 8 bytes del mensaje recibido
      for (int i = 0; i < canMsg.can_dlc; i++) {
        receivedData[i + 8] = (char)canMsg.data[i];
      }
      receivedData[16] = '\0'; // Null-terminate the string

      Serial.println("Segunda parte recibida");

      // Mostrar el mensaje completo recibido
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
        switchCom(12u, TX_MODE);
  ax12a.begin(BaudRate, DirectionPin, &Serial);
  ax12a.setEndless(ID, OFF);
  ax12a.setEndless(ID2, OFF);
  ax12a.setEndless(ID3, OFF);

  switchCom(12u, TX_MODE); 
    //Dynamixel
    int g1 = values[0] - 62;
    int g2 = values[1];
    int g3 = values[2];

    ax12a.move(ID, g1);
    ax12a.move(ID2, g2);
    ax12a.move(ID3, g3);
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(1000);                      // wait for a second
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
    delay(1000); 
    switchCom(12u, RX_MODE); 
    delay(100);
   }
    }
  }

 
 
