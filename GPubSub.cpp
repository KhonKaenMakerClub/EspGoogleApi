/**
 * @file GPubSub.cpp
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
 
#include "GPubSub.h"

GPubSub::GPubSub(void)
{

}
GPubSub::~GPubSub(void)
{

}
bool GPubSub::begin(String client_id, String client_secret,String project_id,bool renew_token);
{
  SPIFFS.begin();
  _project_id = project_id;
  if(!loadConfig() || renew_token){
    int retry = 5;
    while(!oauth.oauth(client_id, client_secret,_scope)){
      DEBUG("[GPUBSUB] Cannot OAuth retry %d ...\n",retry);
      if(retry < 0){
        DEBUG("[GPUBSUB] Cannot OAuth timeout...\n");
        return false;
      }
      retry--;
    }
    _token = oauth.getToken();
    _refresh_token = oauth.getRefreshToken();
    writeConfig();
  }
  //------ get last ack id  
}
bool GPubSub::createTopic(String topic)
{
  HTTPClient http;
  if(topic[0]!='/'){
    DEBUG("[GPUBSUB] topic must start with '/'\n");
    return false;
  }
  String url = "https://pubsub.googleapis.com/v1/projects/"+_project_id+"/topics/"+topic+"?access_token="+_token;  
  DEBUG("[GPUBSUB] URL:%s\n",url.c_str());
  http.begin(url,_finger_pubsub);
  http.setUserAgent("Mozilla/5.0 (Windows NT 6.3; WOW64)");  
  http.addHeader("Accept","text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
  int httpCode = http.sendRequest("PUT");  
  if(httpCode <= 0) {    
    DEBUG("[GPUBSUB] ... failed, error: %s\n", http.errorToString(httpCode).c_str());
    return false;
  }
  if(httpCode != 200) {
    DEBUG("[GPUBSUB] Error... code: %d\n", httpCode);
    if(httpCode == 401 || httpCode == 403){
      http.end();
      oauth.refreshToken();
      _token = oauth.getToken();
      _refresh_token = oauth.getRefreshToken();
      writeConfig();
      return false;
    }
  }
  if(httpCode == 200){
    http.end();
    DEBUG("[GPUBSUB] Create topic successful ... code: %d\n", httpCode);    
    return true;
  }
  http.end();
  return false;
}
bool GPubSub::createSubscription(String subName,String topicName,int ackDeadline)
{
https://pubsub.googleapis.com/v1/projects/esponlineupdate-1342/subscriptions/testtest?key={YOUR_API_KEY}

{
 "ackDeadlineSeconds": 600,
 "topic": "projects/esponlineupdate-1342/topics/test2",
 "name": "projects/esponlineupdate-1342/subscriptions/testtest"
}
  topicName = "projects/"+_project_id+"/topics"+topicName;
  subName = "projects/"+_project_id+"/subscriptions"+subName;
  HTTPClient http;
  String url = "https://pubsub.googleapis.com/v1/"+subName+"?access_token="_token;
  
}
bool GPubSub::publish(String topic,String data)
{
  HTTPClient http;
  if(topic[0]!='/'){
    DEBUG("[GPUBSUB] topic must start with '/'\n");
    return false;
  }
  String url = "https://pubsub.googleapis.com/v1/projects/"+_project_id+"/topics/"+topic+":publish?access_token="+_token;  
  DEBUG("[GPUBSUB] URL:%s\n",url.c_str());
  http.begin(url,_finger_pubsub);
  http.setUserAgent("Mozilla/5.0 (Windows NT 6.3; WOW64)");  
  http.addHeader("Accept","text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
  String payload = "{\"messages\": [{\"data\": \""+base64::encode(data)+"\"}]}";
  int httpCode = http.POST(payload);  
  if(httpCode <= 0) {    
    DEBUG("[GPUBSUB] ... failed, error: %s\n", http.errorToString(httpCode).c_str());
    return false;
  }
  if(httpCode != 200) {
    DEBUG("[GPUBSUB] Error... code: %d\n", httpCode);
    if(httpCode == 401 || httpCode == 403){
      http.end();
      oauth.refreshToken();
      _token = oauth.getToken();
      _refresh_token = oauth.getRefreshToken();
      writeConfig();
      // recursive to retry again but it may cause of forever loop
      //return publish(topic,data);
      return false;
    }
  }
  if(httpCode == 200){
    String resp = http.getString();
    if(resp.indexOf("messageIds") != -1){
      DEBUG("[GPUBSUB] publish topic successful ... code: %d\n", httpCode);
      http.end();
      return true;
    }    
  }
  http.end();
  return false; 
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
