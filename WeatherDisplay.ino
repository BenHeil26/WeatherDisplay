/*
 * Weather Display
 * Author: Ben Heil
 * board: UNO R4 WiFi
 * 
 * Calls public weather APIs to load current temperature and display it using on board LED display
 *
 */

#include "WiFiS3.h"

#include "arduino_secrets.h"
#include "lib.h"

char* SSID = SECRET_SSID;
char* PASS = SECRET_PASS;

int status = WL_IDLE_STATUS;

WiFiClient client;

void setup() {
}

void loop() {
}
