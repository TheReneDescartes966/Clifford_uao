#include <SPI.h>
#include <mcp2515.h>

// Configuración del nodo
const uint8_t NODE_ID = 4; // Cambiar a 2, 3 o 4 para los demás nodos
const uint16_t BASE_CAN_ID = 0x100; // Base para IDs
const uint16_t NODE_CAN_ID = BASE_CAN_ID + (NODE_ID << 4); // ID único del nodo

MCP2515 mcp2515(10); // Pin CS del MCP2515
String receivedMessage = "";
unsigned long lastMessageTime = 0;
const unsigned long timeout = 1000; // Tiempo de espera para mensajes incompletos

void setup() {
    Serial.begin(115200);

    // Inicialización del MCP2515
    mcp2515.reset();
    mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ); // Configura la velocidad CAN a 500 kbps
    mcp2515.setNormalMode();

    // Configuración de filtro y máscara
    struct can_frame filterMask;
    filterMask.can_id = NODE_CAN_ID & 0x7F0; // Filtro para aceptar solo mensajes de este rango
    filterMask.can_dlc = 0x7F0; // Máscara para IDs en el rango

    Serial.println("MCP2515 Initialized - Multi-node Mode");
}

void loop() {
    checkForIncomingCAN();
    checkForSerialInput();
}

void checkForIncomingCAN() {
    struct can_frame canMsg;

    // Leer mensajes CAN entrantes
    if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
        processIncomingCANMessage(canMsg);
    }
}

void processIncomingCANMessage(struct can_frame &canMsg) {
    if (millis() - lastMessageTime > timeout && receivedMessage.length() > 0) {
        Serial.println("Timeout, mensaje incompleto descartado.");
        resetReception();
    }

    lastMessageTime = millis();

    // Mostrar contenido de la trama recibida
    Serial.print("Trama CAN ID: 0x");
    Serial.print(canMsg.can_id, HEX);
    Serial.print(" | Data: ");
    for (int i = 0; i < canMsg.can_dlc; i++) {
        Serial.print((char)canMsg.data[i]);
    }
    Serial.println();

    // Verificar si el mensaje pertenece a este nodo
    if ((canMsg.can_id & 0x7F0) == NODE_CAN_ID) {
        for (int i = 0; i < canMsg.can_dlc; i++) {
            if (canMsg.data[i] != 0) {
                receivedMessage += (char)canMsg.data[i];
            }
        }
    } else {
        Serial.println("Mensaje no dirigido a este nodo.");
    }

    // Si se recibe el final del mensaje, procesarlo
    if (receivedMessage.endsWith("\n")) {
        Serial.print("Mensaje completo recibido: ");
        Serial.println(receivedMessage);
        resetReception();
    }
}

void resetReception() {
    receivedMessage = "";
}

void checkForSerialInput() {
    if (Serial.available()) {
        String input = Serial.readStringUntil('\n');
        int delimiterIndex = input.indexOf(':');

        if (delimiterIndex > 0) {
            // Extraer NODE_ID y mensaje
            int targetNodeID = input.substring(0, delimiterIndex).toInt();
            String message = input.substring(delimiterIndex + 1);

            if (targetNodeID >= 1 && targetNodeID <= 4) {
                sendMessageToNode(targetNodeID, message.c_str());
            } else {
                Serial.println("Error: NODE_ID fuera de rango (1-4).");
            }
        } else {
            Serial.println("Error: Formato inválido. Use <NODE_ID>:<MENSAJE>.");
        }
    }
}

void sendMessageToNode(uint8_t targetNodeID, const char* message) {
    struct can_frame canMsg;
    uint16_t targetCanID = BASE_CAN_ID + (targetNodeID << 4);
    size_t len = strlen(message);
    size_t frames = (len + 7) / 8;

    for (size_t i = 0; i < frames; i++) {
        canMsg.can_id = targetCanID + i;
        canMsg.can_dlc = 8;

        for (size_t j = 0; j < 8; j++) {
            size_t index = i * 8 + j;
            canMsg.data[j] = (index < len) ? message[index] : 0;
        }

        if (mcp2515.sendMessage(&canMsg) == MCP2515::ERROR_OK) {
            Serial.print("Trama CAN al nodo ");
            Serial.print(targetNodeID);
            Serial.print(" enviada: ");
            Serial.println(i + 1);
        } else {
            Serial.print("Error al enviar la trama al nodo ");
            Serial.println(targetNodeID);
        }
    }
}
