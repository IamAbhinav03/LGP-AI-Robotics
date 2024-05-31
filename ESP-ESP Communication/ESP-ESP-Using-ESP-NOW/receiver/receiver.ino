#include <esp_now.h>
#include <WiFi.h>

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    // int ldrValue;
    char *text
} struct_message;

struct_message incomingData;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  // Cast incoming data to our struct
  struct_message myData;
  memcpy(&myData, incomingData, sizeof(myData));
  
  // Print received data
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("LDR Value: ");
  // Serial.println(myData.ldrValue);
  Serial.println(myData.text);
  int data = map(myData.ldrValue, 0, 4095, 800, 2000);
  tone(4, data);
}

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

  // Register callback function
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // Nothing to do here
}
