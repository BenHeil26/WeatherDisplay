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

char* ssid = SECRET_SSID;
char* pass = SECRET_PASS;

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


void scroll_to_led(String text){
  Serial.begin(115200);
  matrix.begin();
  matrix.beginDraw();

  matrix.stroke(0xFFFFFF);
  matrix.textScrollSpeed(50);
  matrix.textFont(Font_5x7);
  matrix.beginText(0, 1, 0xFFFFFF);
  
  matrix.println("    " + text + "    ");
  matrix.endText(SCROLL_LEFT);

  matrix.endDraw();
}

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    if (status == WL_CONNECTED){
      scroll_to_led("connected");
    }
    // wait 10 seconds for connection:
    delay(10000);
  }
}

void loop() {

  deserializeJson(doc, SAMPLE_JSON);

  float temp = doc["properties"]["temperature"]["value"];
  String text = String((1.8*temp)+32); //convert C to F
  scroll_to_led("30*");
}
