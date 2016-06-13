/**
 * @file GoogleDriveUpdate.ino
 * @date 12-Jun-2016
 * @author Comdet Phaudphut
 * @facebook fb.com/comdet
 * 
 * Comdet (c) by Comdet Phaudphut
 * this file is licensed under a
 * Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.
 * You should have received a copy of the license along with this
 * work.  If not, see <http://creativecommons.org/licenses/by-nc-sa/3.0/>.
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include "GDriveUpdate.h"
//------ Config -------//
const String client_id = "------- client id -------";
const String client_secret = "------- secret ------";
const String folder_id = "----- google drive folder id -----";
GDriveUpdate gUpdate;
ESP8266WiFiMulti WiFiMulti;
void setup(){
    Serial.begin(115200);
    Serial.println();
    for(uint8_t t = 5; t > 0; t--) {
        Serial.printf("[SETUP] WAIT %d...\n", t);
        delay(1000);
    }
    WiFiMulti.addAP("---ssid---", "--- password  ---");
    gUpdate.begin(client_id,client_secret,folder_id);
    gUpdate.setLedDebug(true);
}
void loop() {
  if((WiFiMulti.run() == WL_CONNECTED)) {
    gUpdate.run();
  }
  delay(100);
}
