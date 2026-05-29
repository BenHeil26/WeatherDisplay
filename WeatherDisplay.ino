/*
 * Weather Display
 * Author: Ben Heil
 * board: UNO R4 WiFi
 * 
 * Calls public weather APIs to load current temperature and display it using on board LED display
 *
 * Required: Create an arduino_secrets.h file with the following config values
 * #define SECRET_SSID <your_ssid>
 * #define SECRET_PASS <your_pass>
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

unsigned long last_time_stamp = 0;
const unsigned long poll_interval = 10 * 1000L; // how long (in milliseconds) to wait to poll

String temperature;
char *server = "https://api.weather.gov";

const size_t BUF_SIZE = 4096;
unsigned char response_buf[BUF_SIZE];

// const char* SAMPLE_JSON = 
// "{ \
//     \"properties\": { \
//         \"temperature\": { \
//             \"value\": 22.0, \
//         } \
//     } \
// }";

WiFiSSLClient client;
JsonDocument doc;
ArduinoLEDMatrix matrix;

void init_matrix(){
  Serial1.begin(115200);
  matrix.begin();
}

void scroll_to_led(String text){
  matrix.beginDraw();

  matrix.stroke(0xFFFFFF);
  matrix.textScrollSpeed(50);
  matrix.textFont(Font_5x7);
  matrix.beginText(0, 1, 0xFFFFFF);
  
  matrix.println("    " + text + "    ");
  matrix.endText(SCROLL_LEFT);

  matrix.endDraw();
}

void get_temperature(){
  // close any connection before send a new request.
  // This will free the socket on the NINA module
  client.stop();

  // if there's a successful connection:
  if (client.connect(server, 443)) {
    Serial.println("connecting...");
    // send the HTTP GET request:
    client.println("GET /stations/E0797/observations/latest HTTP/1.1");
    client.println("Host: api.weather.gov");
    client.println("User-Agent: ArduinoWiFi/1.1");
    client.println("Connection: close");
    client.println("Accept: application/json");
    client.println();
  } else {
    // if you couldn't make a connection:
    scroll_to_led("request failed");
  }

  if (client.available()){
    client.read(response_buf, BUF_SIZE);
    deserializeJson(doc, response_buf);
    float temp = doc["properties"]["temperature"]["value"];
    temperature = String((1.8*temp)+32); //convert C to F
  } else{
    scroll_to_led("no data");
  }
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
      init_matrix();
      scroll_to_led("connected");
    }
    // wait 10 seconds for connection:
    delay(10000);
  }
  get_temperature(); 
}

void loop() {
  scroll_to_led(temperature);
  if (millis() - last_time_stamp > poll_interval){
    get_temperature();
    last_time_stamp = millis();
  }
}
