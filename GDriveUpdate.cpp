/**
 * @file GDriveUpdate.cpp
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
 
#include "GDriveUpdate.h"

GDriveUpdate::GDriveUpdate(void)
{
}
GDriveUpdate::~GDriveUpdate(void)
{
}
bool GDriveUpdate::begin(String client_id, String client_secret,String folder_id,bool renew_token)
{
  oauth.begin();
  _folder_id = folder_id;  
  if(oauth.getToken() == "" || renew_token){
    if(!oauth.oauth(client_id, client_secret,_scope)){
      DEBUG("[GDRIVEUPDATE] Cannot OAuth ...\n");
      return false;
    }
  }
  oauth.getToken();
  loadConfig();
  _last_time = millis();
  return true;
}
bool GDriveUpdate::run()
{
  if(millis() - _last_time > (_refresh_rate*1000))
  {    
    bool res = updateCheck(); 
    _last_time = millis();
    return res;
  }
}
bool GDriveUpdate::renewToken()
{
  return oauth.removeToken();
}
void GDriveUpdate::setLedDebug(bool enable)
{
   _led_debug = enable;
}
void GDriveUpdate::setRefreshRate(int rate)
{
  _refresh_rate = rate;
}
bool GDriveUpdate::updateCheck()
{
  HTTPClient https;
  String urls ="https://www.googleapis.com/drive/v2/files?corpus=DOMAIN&orderBy=createdDate+desc&maxResults=1&";
  urls += "q=%27"+_folder_id+"%27+in+parents+and+trashed%3Dfalse&access_token="+oauth.getToken();
  DEBUG("[GDRIVEUPDATE] URL:%s\n",urls.c_str());  
  https.begin(urls,_finger);
  https.setUserAgent("Mozilla/5.0 (Windows NT 6.3; WOW64)");
  https.addHeader("Accept","text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
  int httpCode = https.GET();
  if(httpCode <= 0) {
    DEBUG("[GDRIVEUPDATE] ... failed, error: %s\n", https.errorToString(httpCode).c_str());
    return false;
  }
  if(httpCode != HTTP_CODE_OK) {
    DEBUG("[GDRIVEUPDATE] Error... code: %d\n", httpCode);
    if(httpCode == 401 || httpCode == 403){
      https.end();
      oauth.refreshToken();
      return false;
    }    
  }
  WiFiClient * stream = https.getStreamPtr();
  stripStream("id\": \"",stream,-1);
  String id = stream->readStringUntil('\"');
  stripStream("createdDate\": \"",stream,-1);
  String create_date = stream->readStringUntil('\"');  
  String firmware_url = "https://www.googleapis.com/drive/v2/files/";
  firmware_url += id+"?alt=media&access_token="+oauth.getToken();
  DEBUG("[GDRIVEUPDATE] id : %s\n", id.c_str());
  DEBUG("[GDRIVEUPDATE] createdDate : %s\n", create_date.c_str());
  DEBUG("[GDRIVEUPDATE] download : %s\n", firmware_url.c_str());
  if(id != _last_firmware_id){
    https.end();
    firmware_url = getDownloadUrl(firmware_url);
    DEBUG("[GDRIVEUPDATE] download redirect to : %s\n", firmware_url.c_str());
    DEBUG("!!!![Update Found ]!!!!\n");       
    updateFirmware(id,firmware_url);
  }
  return true;
}

bool GDriveUpdate::updateFirmware(String id,String url)
{
  if(_led_debug)
  {
    pinMode(D0,OUTPUT);
    digitalWrite(D0,HIGH); delay(200); digitalWrite(D0,LOW); delay(200);
    digitalWrite(D0,HIGH); delay(200); digitalWrite(D0,LOW); delay(200);
    digitalWrite(D0,HIGH); delay(200); digitalWrite(D0,LOW); delay(200);
    led_ticker.attach(0.1,ledBlink);
  }
  ESPhttpUpdate.rebootOnUpdate(false);
  t_httpUpdate_return  ret = ESPhttpUpdate.update(url,"0",_finger_file);  
  switch(ret) {
    case HTTP_UPDATE_FAILED:
      DEBUG("[GDRIVEUPDATE] HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
      break;
    case HTTP_UPDATE_NO_UPDATES:
      DEBUG("[GDRIVEUPDATE] HTTP_UPDATE_NO_UPDATES");
      break;
    case HTTP_UPDATE_OK:
      _last_firmware_id = id;
      writeConfig();
      Serial.println("[GDRIVEUPDATE] HTTP_UPDATE_OK");
      ESP.restart();
      break;
  }
  return true;
}
bool GDriveUpdate::loadConfig()
{
  File f = SPIFFS.open("/gdriveupdate.config", "r+");
  if (!f) {
      DEBUG("[GDRIVEUPDATE] file open failed\n");
      return false;
  }
  _last_firmware_id = f.readStringUntil(';');    
  DEBUG("[GDRIVEUPDATE]Load Config firmware id : %s\n",_last_firmware_id.c_str());  
  f.close();
  if(_last_firmware_id != ""){
    return true;
  }
  return false;
}
bool GDriveUpdate::writeConfig()
{
  DEBUG("[GDRIVEUPDATE] Save Config firmware id : %s\n",_last_firmware_id.c_str());  
  File f = SPIFFS.open("/gdriveupdate.config", "w+");
  if (!f) {
      DEBUG("[GDRIVEUPDATE] file open failed");
      return false;
  }
  f.print(_last_firmware_id+";");  
  f.close();
  return true;
}

String GDriveUpdate::getDownloadUrl(String url)
{
  HTTPClient http;
  http.begin(url,_finger);  
  const char * headerkeys[] = { "Location" };
  size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);
  http.collectHeaders(headerkeys, headerkeyssize);
  http.setUserAgent("Mozilla/5.0 (Windows NT 6.3; WOW64)");
  http.addHeader("Accept","text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
  int httpCode = http.GET();
  if(http.hasHeader("Location")){
    http.end();
    return http.header("Location");  
  }
  http.end();
  return url;  
}
