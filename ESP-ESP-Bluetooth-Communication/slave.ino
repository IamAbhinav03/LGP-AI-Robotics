#include <BluetoothSerial.h> // Include the BluetoothSerial library

BluetoothSerial SerialBT; // Create an instance of BluetoothSerial for communication

const int buzzerPin = 15; // Define the pin number for the buzzer
const int thresholdDistance = 20;  // Define the threshold distance in centimeters

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(115200);
  
  // Set the buzzerPin as OUTPUT and initially turn it off
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  // Initialize Bluetooth with the name "ESP32_BT_Receiver"
  SerialBT.begin("ESP32_BT_Receiver");
  Serial.println("Bluetooth Receiver Started");
}

void loop() {
  // Check if there is any data available to read from Bluetooth
  if (SerialBT.available()) {
    byte received_data[4]; // Array to store the received byte data
    SerialBT.readBytes(received_data, 4); // Read 4 bytes from Bluetooth

    // Convert the received byte array back to a long integer
    // Refer to the long_to_byte_array_conversion.txt for more information
    long distance = (received_data[0] << 24) | (received_data[1] << 16) | (received_data[2] << 8) | received_data[3];

    // Print the received distance to the serial monitor
    Serial.println("Distance received: " + String(distance));

    // Check if the received distance is less than the threshold distance
    if (distance < thresholdDistance) {
      digitalWrite(buzzerPin, HIGH); // Turn on the buzzer if the distance is below the threshold
    } else {
      digitalWrite(buzzerPin, LOW); // Turn off the buzzer if the distance is above the threshold
    }
  }
}

