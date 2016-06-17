/**
 * @file GSheets.cpp
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
 
#include "GSheets.h"

GSheets::GSheets(void)
{

}
GSheets::~GSheets(void)
{

}
bool GSheets::begin(String client_id, String client_secret,String sheet_id,bool renew_token)
{
  SPIFFS.begin();
  _sheet_id = sheet_id;
  oauth.init(client_id,client_secret,_scope);
  if(!loadConfig() || renew_token){
    int retry = 5;
    while(!oauth.oauth()){
      DEBUG("[GSHEET] Cannot OAuth retry %d ...\n",retry);
      if(retry < 0){
        DEBUG("[GSHEET] Cannot OAuth timeout...\n");
        return false;
      }
      retry--;
    }
    _token = oauth.getToken();
    _refresh_token = oauth.getRefreshToken();
    writeConfig();
  }
  oauth.setToken(_token);
  oauth.setRefreshToken(_refresh_token);
  return true;
}
bool GSheets::renewToken()
{
  bool res = oauth.removeToken();
  if(res){
    _token = oauth.getToken();
    _refresh_token = oauth.getRefreshToken();
    writeConfig();
  }
  return res;
}
bool GSheets::loadConfig()
{
  File f = SPIFFS.open("/gsheet.config", "r+");
  if (!f) {
      DEBUG("[GSHEET] file open failed\n");
      return false;
  }  
  _token = f.readStringUntil(';');
  _refresh_token = f.readStringUntil(';');
  DEBUG("[GSHEET]Load Config token : %s\n",_token.c_str());  
  DEBUG("[GSHEET]Load Config refreshToken : %s\n",_refresh_token.c_str());  
  f.close();
  if(/*_last_firmware_id != "" && */ _token != "" && _refresh_token != ""){
    return true;
  }
  return false;
}
bool GSheets::writeConfig()
{  
  DEBUG("[GSHEET] Save Config token : %s\n",_token.c_str());  
  DEBUG("[GSHEET] Save Config refreshToken : %s\n",_refresh_token.c_str());  
  File f = SPIFFS.open("/gsheet.config", "w+");
  if (!f) {
      DEBUG("[GSHEET] file open failed\n");
      return false;
  }
  f.print(_token+";");
  f.print(_refresh_token+";");
  f.close();
  return true;
}
void GSheets::addColumn(String columnName,String value)
{
  columnName.toLowerCase();
  _atom_content += "<g:"+columnName+">"+value+"</g:"+columnName+">";
}
bool GSheets::insertRow()
{
  HTTPBypass http;
  String url ="https://spreadsheets.google.com/feeds/list/"+_sheet_id+"/od6/private/full?access_token="+_token;  
  DEBUG("[GSHEET] URL:%s\n",url.c_str());
  http.begin(url,_finger);
  http.setUserAgent("Mozilla/5.0 (Windows NT 6.3; WOW64)");  
  http.addHeader("Accept","text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
  http.addHeader("Content-Type","application/atom+xml");
  String payload = "<entry xmlns=\"http://www.w3.org/2005/Atom\" xmlns:g=\"http://schemas.google.com/spreadsheets/2006/extended\">";
  payload += _atom_content + "</entry>";
  DEBUG("[GSHEET] Payload : %s\n",payload.c_str());
  int httpCode = http.POST(payload);
  _atom_content = "";
  if(httpCode <= 0) {
    DEBUG("[GSHEET] ... failed, error: %s\n", http.errorToString(httpCode).c_str());
    return false;
  }
  if(httpCode != 201) {
    DEBUG("[GSHEET] Error... code: %d\n", httpCode);
    if(httpCode == 401 || httpCode == 403){
      http.end();      
      oauth.refreshToken();
      _token = oauth.getToken();
      _refresh_token = oauth.getRefreshToken();
      writeConfig();
      return false;
    }
  }

  if(httpCode == 201){
    http.end();
    DEBUG("[GSHEET] Insert successful ... code: %d\n", httpCode);    
    return true;
  }
  http.end();
  return false;
}
bool GSheets::deleteRow()
{
  //TODO : implement delete row
  return false;
}
bool GSheets::updateRow()
{
  //TODO : implement update row
  return false;
}