## Weather Display

A current weather monitor for Arduino R4 WiFi

Demonstrates several Arduino features as follows:
* Connecting to WiFi networks
* Making SSL Encrypted API calls
* Deserializing json
* Writing to the LED Screen

## Setup

Via Arduino IDE, update to latest firmware before starting

Create a file called `arduino_secrets.h` and include it in your sketch.

The file should have the following values:
```cpp
#define SECRET_SSID "<your_ssid>"
#define SECRET_PASS "<your_password>"
```

Connection information can be displayed using the Arduino IDE Serial Monitor

## Sample Weather Request

```curl
curl -X 'GET' 'https://api.weather.gov/stations/E0797/observations/latest' \
          -H 'accept: application/geo+json'
```
