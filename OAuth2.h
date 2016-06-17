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

#define DEBUG(...) //Serial.printf( __VA_ARGS__ )

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
    void setRefreshToken(String refresh_token);
private:    
    const String _finger = "00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
    const String _finger_ggac = "00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";    
    
    String _client_id = "";
    String _client_secret = "";
    String _scope = "";
    String _token = "";
    String _refresh_token = "";
    String _token_type = "";    
    int _token_time = 0;
};

#endif /* OAUTH2_H_ */

