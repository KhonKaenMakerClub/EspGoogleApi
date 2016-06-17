/**
 * @file GSheets.h
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
 
#ifndef GSHEETS_H_
#define GSHEETS_H_

#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include "FS.h"
#include "OAuth2.h"
#include "Helper.h"
#define DEBUG(...) //Serial.printf( __VA_ARGS__ )

class GSheets
{
public:    
    GSheets(void);
    ~GSheets(void);
    bool begin(String client_id, String client_secret,String sheet_id,bool renew_token=false);    
    bool loadConfig();
    bool writeConfig();
    void addColumn(String columnName,String value);
    bool insertRow();
    bool deleteRow();
    bool updateRow();
    bool renewToken();

private:    
    
    OAuth2 oauth;
    const String _finger = "6B:55:4F:08:78:6D:81:1E:DC:21:B9:D1:5F:5B:64:6E:87:59:75:8B";
    const String _scope = "https://spreadsheets.google.com/feeds";    
    String _sheet_id = "";
    String _atom_content = "";
    String _token = "";
    String _refresh_token = "";    
};

#endif /* GSHEETS_H_ */

