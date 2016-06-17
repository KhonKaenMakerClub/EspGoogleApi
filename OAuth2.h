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
#include "HTTPBypass.h"
#include "FS.h"
#include "Helper.h"

#define DEBUG(...) Serial.printf( __VA_ARGS__ )

class OAuth2
{
public:
    OAuth2(void);
    ~OAuth2(void);
    bool init(String client_id, String client_secret, String scope);
    bool init(String client_id, String client_secret, String scope, String token, String refresh_token);
    bool oauth();
    bool refreshToken();
    String getToken();
    String getRefreshToken();
    bool removeToken();
    void setToken(String token);
<<<<<<< HEAD
private:
<<<<<<< HEAD
    const String _finger = "3D:49:7F:D9:3E:17:0E:2A:0F:D8:AC:1F:83:63:8B:29:ED:27:44:2F";    
    const String _finger_ggac = "AF:1D:99:E2:D3:E3:DE:73:4E:0A:60:E5:37:4C:F4:8C:4D:D0:E6:D4";
=======
    void setRefreshToken(String refresh_token);
private:    
    const String _finger = "00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
    const String _finger_ggac = "00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";    
    
>>>>>>> refs/remotes/origin/bypass
=======
    const String _finger = "f4 26 6b 8f df 92 5b 24 8b 01 80 59 6b 43 07 e5 62 bb 36 a9";    
    const String _finger_ggac = "4B 9A AA 36 1F 1A DC 64 FB 42 0F E6 02 4C EE A3 F8 05 AA 91";
>>>>>>> parent of 2a6ef4e... Update cert
    String _client_id = "";
    String _client_secret = "";
    String _scope = "";
    String _token = "";
    String _refresh_token = "";
    String _token_type = "";    
    int _token_time = 0;
};

#endif /* OAUTH2_H_ */

