/*
 * Weather Display
 * Author: Ben Heil
 * board: UNO R4 WiFi
 * 
 * Calls public weather APIs to load current temperature and display it using on board LED display
 *
 */

#include "WiFiS3.h"
#include <ArduinoJson.h>
#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"

#include "arduino_secrets.h"
#include "lib.h"

char* SSID = SECRET_SSID;
char* PASS = SECRET_PASS;

int status = WL_IDLE_STATUS;

const char* SAMPLE_JSON = 
"{ \
    \"properties\": { \
        \"temperature\": { \
            \"value\": 1.0, \
        } \
    } \
}";

WiFiClient client;
JsonDocument doc;
ArduinoLEDMatrix matrix;


void setup() {
  deserializeJson(doc, SAMPLE_JSON);
  Serial.begin(115200);
  matrix.begin();

  matrix.beginDraw();
  matrix.stroke(0xFFFFFFFF);
  float temp = doc["properties"]["temperature"]["value"];
  String text = String((1.8*temp)+32); //convert C to F
  matrix.textFont(Font_5x7);
  matrix.beginText(0, 1, 0xFFFFFF);
  matrix.println(text);
  matrix.endText();

  matrix.endDraw();
}

void loop() {
}
