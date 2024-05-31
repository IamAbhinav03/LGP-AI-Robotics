#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "";          // Replace with your WiFi SSID
const char* password = "";  // Replace with your WiFi password

const char* apiKey = "924056f39b342eee5396ef1be85547c0";     // Replace with your OpenWeather API key
const char* location = "Kerala";      // Replace with your desired location

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("connected");

  // Fetch and print weather data
  fetchWeatherData();
}

void loop() {
  // You can call fetchWeatherData() periodically to get updated data
  // fetchWeatherData();
  // delay(60000);  // Update every 60 seconds

  // Don't do api calls inside loop unless you are sure it won't go beyond your api limit. You can call it inside loop periodically like ever hour or every day.
}

void fetchWeatherData() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "http://api.openweathermap.org/data/2.5/weather?q=" + String(location) + "&appid=" + String(apiKey);
    
    http.begin(url);
    int httpCode = http.GET();

    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println("Received payload:");
      Serial.println(payload);

      // Parse JSON
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, payload);

      float temperature = doc["main"]["temp"];
      int humidity = doc["main"]["humidity"];
      const char* weatherDescription = doc["weather"][0]["description"];

      // Convert temperature from Kelvin to Celsius
      temperature = temperature - 273.15;

      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.println(" °C");

      Serial.print("Humidity: ");
      Serial.print(humidity);
      Serial.println(" %");

      Serial.print("Weather: ");
      Serial.println(weatherDescription);
    } else {
      Serial.println("Error on HTTP request");
    }

    http.end();
  } else {
    Serial.println("WiFi not connected");
  }
}
