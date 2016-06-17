/**
 * @file GDriveUpdate.h
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
 
#ifndef GDRIVEUPDATE_H_
#define GDRIVEUPDATE_H_

#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <Ticker.h>
#include "FS.h"
#include "OAuth2.h"
#include "Helper.h"

#define DEBUG(...) //Serial.printf( __VA_ARGS__ )

class GDriveUpdate
{
public:
    static void ledBlink()
    {
      static int led_stage = 0;
      led_stage = !led_stage;
      digitalWrite(BUILTIN_LED,led_stage);
    }
    GDriveUpdate(void);
    ~GDriveUpdate(void);
    bool begin(String client_id, String client_secret,String folder_id,bool renew_token=false);
    bool run();
    bool updateCheck();
    bool updateFirmware(String id,String url);
    bool loadConfig();
    bool writeConfig();
    void setRefreshRate(int rate);
    void setLedDebug(bool enable);
    bool renewToken();
private:    
    
    OAuth2 oauth;
    const String _finger = "3D:49:7F:D9:3E:17:0E:2A:0F:D8:AC:1F:83:63:8B:29:ED:27:44:2F";
    const String _finger_file = "A3:6A:B9:04:E1:7D:0B:80:AC:E8:4E:AB:1A:17:0A:C4:99:BA:0C:3F"; //usercontents   
    
    String _scope = "https://docs.google.com/feeds/,https://www.googleapis.com/auth/drive,https://www.googleapis.com/auth/drive.file";
    String _folder_id = "";
    String _last_firmware_id = "";
    String _token = "";
    String _refresh_token = "";

    String getDownloadUrl(String url);
    int _refresh_rate = 10; //10 sec per request
    unsigned long _last_time = 0;    
    bool _led_debug = false;

    Ticker led_ticker;
};

#endif /* GDRIVEUPDATE_H_ */

