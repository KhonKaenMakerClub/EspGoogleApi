/**
 * @file OAuth2.h
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
#include "OAuth2.h"

OAuth2::OAuth2(void)
{  
}

OAuth2::~OAuth2(void)
{
}
bool OAuth2::begin()
{
  if(!SPIFFS.begin()) {
    DEBUG("[OAuth2] Failed to mount file system");
    return false;
  } 
  loadConfig();
  return true;
}
bool OAuth2::removeToken()
{  
  _token = "";
  return SPIFFS.remove("/oauth.config");
}
bool OAuth2::oauth(String client_id, String client_secret,String scope)
{  
  _client_id = client_id;
  _client_secret = client_secret;
  
  HTTPClient http;
  DEBUG("[OAuth2] Get user code ...\n");        
  http.begin("https://accounts.google.com/o/oauth2/device/code",_finger_ggac);
  http.setUserAgent("Mozilla/5.0 (Windows NT 6.3; WOW64)");
  http.addHeader("Content-Type","application/x-www-form-urlencoded");
  http.addHeader("Accept","text/html");
  String payload = "client_id="+_client_id+"&scope="+scope;
  int httpCode = http.POST(payload);
  if(httpCode <= 0) {
    DEBUG("[OAuth2]... failed, error: %s\n", http.errorToString(httpCode).c_str());
    return false;
  }
  if(httpCode != HTTP_CODE_OK) {
    DEBUG("[OAuth2] Error... code: %d\n", httpCode);
    return false;
  }
  String resp = http.getString();
  String device_code = strip(resp,"device_code\" : \"","\"");
  String user_code = strip(resp,"user_code\" : \"","\"");
  String url = strip(resp,"verification_url\" : \"","\"");
  int expire = strip(resp,"expires_in\" : ",",").toInt();  
  int interval = strip(resp,"interval\" : ","\n").toInt();
  Serial.printf("\n[OAuth 2.0 Alert!]\n");
  Serial.printf("[=== Attension ===]\n");
  Serial.printf("[1. Open => \"%s\" \n",url.c_str());
  Serial.printf("[2. Fill this code : \"%s\" within %d minutes\n",user_code.c_str(),expire/60);
  Serial.printf("[=================]\n");
  //----- loop get verification -----//
  for(int i=expire;i>0;i-=interval){
    delay(interval * 1000);
    if(i%50 == 0) {DEBUG("\n");}
    
    http.begin("https://www.googleapis.com/oauth2/v4/token",_finger);
    http.setUserAgent("Mozilla/5.0 (Windows NT 6.3; WOW64)");
    http.addHeader("Content-Type","application/x-www-form-urlencoded");
    http.addHeader("Accept","text/html");
    payload = "client_id="+_client_id+"&client_secret="+_client_secret+"&code="+device_code+"&grant_type=http://oauth.net/grant_type/device/1.0";
    httpCode = http.POST(payload);
    if(httpCode <= 0) {
      DEBUG(".");      
      continue;
    }
    if(httpCode != HTTP_CODE_OK) {
      DEBUG(".");       
      continue;
    }
    String resp = http.getString();
    if(resp.indexOf("pending") > 0)
    {
        DEBUG("*"); 
        continue;
    }else if(resp.indexOf("slow_down") > 0)
    {
        DEBUG("!"); 
        continue;
    }
    _token = strip(resp,"access_token\": \"","\"");
    _refresh_token = strip(resp,"refresh_token\": \"","\"");
    _token_time = strip(resp,"expires_in\": ",",").toInt();
    _token_type = strip(resp,"token_type\": \"","\"");
    Serial.printf("\n[OAuth 2.0 Registered!]\n");
    Serial.printf("[Token : %s]\n",_token.c_str());
    Serial.printf("[Type : %s]\n",_token_type.c_str());
    Serial.printf("[Refresh_token : %s]\n",_refresh_token.c_str());
    Serial.printf("[Time : %d]\n",_token_time);  
    delay(100);
    //-------------------------//
    if(_token != "" && _refresh_token != "" && _token_time != 0 && _token_type != ""){
      writeConfig();
      return true;
    }
  }
  return false;
}
bool OAuth2::refreshToken()
{
  HTTPClient http;
  http.begin("https://www.googleapis.com/oauth2/v4/token",_finger);
  http.setUserAgent("Mozilla/5.0 (Windows NT 6.3; WOW64)");
  http.addHeader("Content-Type","application/x-www-form-urlencoded");
  http.addHeader("Accept","text/html");
  String payload = "client_id="+_client_id+"&client_secret="+_client_secret+"&refresh_token="+_refresh_token+"&grant_type=refresh_token";
  int httpCode = http.POST(payload);
  if(httpCode <= 0) {
    DEBUG("[OAuth2]... failed, error: %s\n", http.errorToString(httpCode).c_str());
    return false;
  }
  if(httpCode != HTTP_CODE_OK) {
    DEBUG("[OAuth2] Error... code: %d\n", httpCode);
    return false;
  }
  String resp = http.getString();
  _token = strip(resp,"access_token\": \"","\"");
  _token_time = strip(resp,"expires_in\": ",",").toInt();
  _token_type = strip(resp,"token_type\": \"","\"");
  //---------
  if(_token != "" && _token_time != 0 && _token_type != "")
  {
    writeConfig();
    return true;
  }
  return false;
}
bool OAuth2::loadConfig()
{
  File f = SPIFFS.open("/oauth.config", "r+");
  if (!f) {
      DEBUG("[OAuth2] file open failed\n");
      return false;
  }  
  _token = f.readStringUntil(';');
  _refresh_token = f.readStringUntil(';');
    
  DEBUG("[OAuth2]Load Config token: %s\n",_token.c_str());
  DEBUG("[OAuth2]Load Config refresh_token : %s\n",_refresh_token.c_str());
  f.close();
  if(_token != "" && _refresh_token != ""){
    return true;
  }
  return false;
}
bool OAuth2::writeConfig()
{
  DEBUG("[OAuth2] Save Config token : %s\n",_token.c_str());
  DEBUG("[OAuth2] Save Config refresh token : %s\n",_refresh_token.c_str());
  File f = SPIFFS.open("/oauth.config", "w+");
  if (!f) {
      DEBUG("[OAuth2] file open failed");
      return false;
  }
  f.print(_token+";");
  f.print(_refresh_token+";");
  f.close();
  return true;
}
String OAuth2::getToken()
{
  return _token;
}
void OAuth2::setToken(String token)
{
  _token = token;
  writeConfig();
}
