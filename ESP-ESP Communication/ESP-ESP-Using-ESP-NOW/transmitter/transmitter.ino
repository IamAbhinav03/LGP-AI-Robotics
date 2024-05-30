#include <esp_now.h>
#include <WiFi.h>

#define LDR_PIN 15 // Pin where the LDR is connected

// MAC Address of the receiver ESP32
uint8_t receiverMacAddress[] = {0x10, 0x06, 0x1C, 0xB4, 0xE8, 0x80}; // Replace with your receiver's MAC address

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
    int ldrValue;
} struct_message;

struct_message myData;

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  // Register peer
  esp_now_peer_info_t peerInfo;
  memset(&peerInfo, 0, sizeof(peerInfo));
  memcpy(peerInfo.peer_addr, receiverMacAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  // Read LDR value
  int ldrValue = analogRead(36);
  delay(10);
  Serial.printf("ldrValue: %d\n", ldrValue);
  myData.ldrValue = ldrValue;

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(receiverMacAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  } else {
    Serial.println("Error sending the data");
  }

  // Wait for a second before sending the next data
  delay(1000);
}
