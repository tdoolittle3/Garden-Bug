# ESP8266 Temperature & Humidity Sensor with MQTT

This repository contains an Arduino sketch for an ESP8266 microcontroller that reads temperature and humidity data and broadcasts it via MQTT to be consumed by subscribers.

## Features
- 📡 Reads temperature and humidity data
- 📶 Publishes sensor data to an MQTT broker
- 🔄 Supports periodic data updates
- 🔐 Configurable WiFi and MQTT settings

## Hardware Requirements
- ESP8266-based board (e.g., NodeMCU, Wemos D1 Mini)
- DHT11 or DHT22 temperature and humidity sensor
- 4.7kΩ pull-up resistor (for DHT data pin)
- Power source (USB or regulated 3.3V supply)

## Software Requirements
- [Arduino IDE](https://www.arduino.cc/en/software)
- ESP8266 board support package
- Required libraries:
  - `ESP8266WiFi` (built-in)
  - `PubSubClient` (for MQTT)
  - `DHT` (for sensor readings)

## Installation & Setup
1. Clone this repository:
2. Open the `.ino` file in the Arduino IDE.
3. Install necessary libraries via the Arduino Library Manager.
4. Configure WiFi and MQTT settings in the sketch:
5. Connect the hardware.
6. Upload the sketch to the ESP8266.
7. Monitor serial output for debugging.

## MQTT Payload Format
The sensor data is published in JSON format:

## Example MQTT Subscription
To subscribe to the topic using `mosquitto_sub`:
```sh
mosquitto_sub -h your-mqtt-broker-ip -t sensor/temperature_humidity
```

## Troubleshooting
- Ensure WiFi credentials are correct.
- Verify MQTT broker is running and accessible.
- Check sensor connections and power supply.
- Use the Arduino Serial Monitor for debugging.

## License
This project is licensed under the [MIT License](LICENSE).

---

**Author:** Thomas Doolittle  
**Website:** [doolittle-development.vercel.app](https://doolittle-development.vercel.app)

