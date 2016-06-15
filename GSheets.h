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
    const String _finger = "f4 26 6b 8f df 92 5b 24 8b 01 80 59 6b 43 07 e5 62 bb 36 a9";
    const String _finger_sheet ="CA 13 6A C7 78 96 37 79 66 1A AE 43 A5 EE F0 67 59 6A E7 64";   
    const String _finger_sheet2 ="4B 28 4E 4C 36 AF 7C AF 51 49 C2 51 D3 D9 FC E5 62 47 E1 7A";  
                                //4B 28 4E 4C 36 AF 7C AF 51 49 C2 51 D3 D9 FC E5 62 47 E1 7A 
                                  //CA 13 6A C7 78 96 37 79 66 1A AE 43 A5 EE F0 67 59 6A E7 64
                                //4B 28 4E 4C 36 AF 7C AF 51 49 C2 51 D3 D9 FC E5 62 47 E1 7A
    String _selected_finger = "CA 13 6A C7 78 96 37 79 66 1A AE 43 A5 EE F0 67 59 6A E7 64";
    const String _scope = "https://spreadsheets.google.com/feeds";    
    String _sheet_id = "";
    String _atom_content = "";
    String _token = "";
    String _refresh_token = "";    
};

#endif /* GSHEETS_H_ */

