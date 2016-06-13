/**
 * @file CustomJsonGCM.ino
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
#include <GCloudMessage.h>
#include <ArduinoJson.h>

ESP8266WiFiMulti WiFiMulti;
StaticJsonBuffer<200> jsonBuffer;
const String to = "----- device id -----";
const String api_key = "---- your key ----";
void setup() {
    Serial.begin(115200);
    Serial.println();

    for(uint8_t t = 4; t > 0; t--) {
        Serial.printf("[SETUP] WAIT %d...\n", t);
        Serial.flush();
        delay(1000);
    }
    WiFiMulti.addAP("-----ssid----", "---password---");
}

void loop() {    
    if((WiFiMulti.run() == WL_CONNECTED)) {
        JsonObject& root = jsonBuffer.createObject();
        root["to"] = to;
        JsonObject& data = root.createNestedObject("data");
        data["sensor"] = "temp";
        data["value"] = 25.5;
        String json = "";
        root.printTo(json);
        root.printTo(Serial);
        GCloudMessage gcm(api_key);
        gcm.send(json);
    }
    delay(30000);
}