#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Wi-Fi credentials
const char* ssid = "Vishnu";
const char* password = "sainath2001";

// MQTT broker details
const char* mqtt_broker = "34.131.184.52"; // Replace with your server's IP
const int mqtt_port = 1883;
const char* dataTopic = "water/a1";

// IR sensor pin
const int pir = 2; // Use GPIO pin (e.g., D1 corresponds to GPIO5 on ESP8266)
int sensorState = LOW;

// WiFi and MQTT clients
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  pinMode(pir, INPUT);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");

  // Connect to MQTT broker
  client.setServer(mqtt_broker, mqtt_port);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT broker...");
    if (client.connect("ESP8266Sender")) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("Failed, state: ");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void loop() {
  // Publish IR sensor state
  int currentState = digitalRead(pir);
  if (currentState != sensorState) {
    sensorState = currentState;
    String data = (sensorState == HIGH) ? "1" : "0";
    client.publish(dataTopic, data.c_str());
    Serial.println("Published data: " + data);
  }

  // Keep the MQTT connection alive
  client.loop();
  delay(1000); // Adjust the delay as necessary
}
