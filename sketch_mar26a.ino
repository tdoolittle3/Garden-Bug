#include "DHT.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// GPIO where the DS18B20 is connected to
const int oneWireBus = 4;     

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

const int BUZZER_PIN = D1;

#define DPIN 2        //Pin to connect DHT sensor (GPIO number) D2
#define DTYPE DHT11   // Define DHT 11 or DHT22 sensor type

DHT dht(DPIN,DTYPE);

const int lightSensorPin = A0;

const char* ssid = "NETGEAR77";
const char* password = "freequail194";
ESP8266WebServer server(80); // Port 80 is commonly used for HTTP

/******* MQTT Broker Connection Details *******/
const char* mqttServer = "192.168.0.14";
const char* mqttUsername = "growroom";
const int mqttPort =1883;

WiFiClient espClient;
PubSubClient client(espClient);

float highTempF = 100.0;



void setup() {
  Serial.begin(9600);
  dht.begin();
  sensors.begin();
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Print local IP address
  Serial.print("Local IP address: ");
  Serial.println(WiFi.localIP());
  // Set up routes
  server.on("/", handleRoot);
  server.on("/set-variables", handleSetVariables);

  // Start the server
  server.begin();

  client.setServer(mqttServer, mqttPort);
  while (!client.connected()) {
    if (client.connect("arduinoClient")) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("Failed to connect to MQTT broker, rc=");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void loop() {
  delay(2000);
  
  float tc = dht.readTemperature(false);  //Read temperature in C
  float tf = dht.readTemperature(true);   //Read Temperature in F
  float hu = dht.readHumidity();          //Read Humidity

  Serial.print("Temp: ");
  Serial.print(tc);
  Serial.print(" C, ");
  Serial.print(tf);
  Serial.print(" F, Hum: ");
  Serial.print(hu);
  Serial.println("%");

  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  float temperatureF = sensors.getTempFByIndex(0);
  Serial.print(temperatureC);
  Serial.println("C");
  Serial.print(temperatureF);
  Serial.println("F");

  int curLightValue = analogRead(lightSensorPin);
  Serial.print("Light intensity ");
  Serial.println(curLightValue);

  server.handleClient();

  if (temperatureF > highTempF) {
    Serial.println("HIGH TEMP ALARM");
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }

  StaticJsonDocument<200> telemetryData;
  telemetryData["temperature"] = temperatureF;
  telemetryData["humidity"] = hu;
  telemetryData["lightintensity"] = curLightValue;

  char jsonString[256];
  serializeJson(telemetryData, jsonString);
  client.publish("sensordata", jsonString);
}

void handleRoot() {
  server.send(200, "text/html", "<!DOCTYPE html><html><body><h2>Set Script Variables</h2><form action='/set-variables' method='post'>Variable 1: <input type='text' name='var1'><br>High Temp (F): <input type='text' name='var2'><br><input type='submit' value='Submit'></form></body></html>");
}

void handleSetVariables() {
  // Retrieve values from POST request
  String var1Value = server.arg("var1");
  String var2Value = server.arg("var2");

  // Convert string values to desired data types and set script variables
  // Example:
  int var1 = var1Value.toInt();
  highTempF = var2Value.toFloat();

  // Optionally, you can send a response back to the client
  server.send(200, "text/plain", "Variables set successfully");
}

