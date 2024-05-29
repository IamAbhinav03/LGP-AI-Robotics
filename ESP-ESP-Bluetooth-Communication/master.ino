#include "BluetoothSerial.h" // Include the BluetoothSerial library

// Define pin numbers for the ultrasonic sensor
int trigPin = 5; 
int echoPin = 18;

// Create an instance of BluetoothSerial for communication
BluetoothSerial SerialBT;

// MAC address of the receiver device
String MACadd = "CC:7B:5C:36:00:62"; 
uint8_t address[6] = {0xCC, 0x7B, 0x5C, 0x36, 0x00, 0x62}; // Same MAC address in hexadecimal format
bool connected; // Variable to store the connection status

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(115200);
  
  // Set the trigPin as OUTPUT and echoPin as INPUT for the ultrasonic sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // Initialize Bluetooth in master mode with the name "WHEELCHAIR"
  SerialBT.begin("WHEELCHAIR", true);
  Serial.println("The device started in master mode. Make sure the receiver device is on!");

  // Attempt to connect to the receiver device using its MAC address
  connected = SerialBT.connect(address);

  if (connected) {
    Serial.println("Connected Successfully!");
  } else {
    // If connection fails, keep trying to connect every 10 seconds
    while (!SerialBT.connected(10000)) {
      Serial.println("Failed to connect. Make sure remote device is available and in range, then restart the app.");
    }
  }
}

void loop() {
  // Get the distance measurement from the ultrasonic sensor
  long distance = getDistance();
  byte send_data[4];

  // Convert the long distance to a byte array for Bluetooth transmission
  // Refer to long_to_byte_array_conversion.txt for more information
  send_data[0] = (distance >> 24) & 0xFF; // Extract the highest byte
  send_data[1] = (distance >> 16) & 0xFF; // Extract the second highest byte
  send_data[2] = (distance >> 8) & 0xFF;  // Extract the third highest byte
  send_data[3] = distance & 0xFF;         // Extract the lowest byte
  
  // Send the byte array via Bluetooth
  SerialBT.write(send_data, 4);

  // Delay to allow the sensor to settle
  delay(20);
}

// Function to get the distance from the ultrasonic sensor
long getDistance() {
  long duration, distance;
  
  // Trigger the ultrasonic pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Measure the duration of the echo pulse
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance based on the duration of the pulse
  distance = (duration / 2) * 0.0343;

  return distance;
}

