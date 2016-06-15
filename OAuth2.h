/**
 * @file OAuth2.cpp
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
#ifndef OAUTH2_H_
#define OAUTH2_H_

#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include "FS.h"
#include "Helper.h"

#define DEBUG(...) //Serial.printf( __VA_ARGS__ )

class OAuth2
{
public:
    OAuth2(void);
    ~OAuth2(void);
    bool oauth(String client_id, String client_secret,String scope);
    bool refreshToken();
    String getToken();
    String getRefreshToken();
    bool removeToken();
    void setToken(String token);
private:
    const String _finger = "f4 26 6b 8f df 92 5b 24 8b 01 80 59 6b 43 07 e5 62 bb 36 a9";    
    const String _finger_ggac = "4B 9A AA 36 1F 1A DC 64 FB 42 0F E6 02 4C EE A3 F8 05 AA 91";
    String _client_id = "";
    String _client_secret = "";
    
    String _token = "";
    String _refresh_token = "";
    String _token_type = "";    
    int _token_time = 0;
};

#endif /* OAUTH2_H_ */

