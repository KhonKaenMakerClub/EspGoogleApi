/**
 * @file GCloudMessage.cpp
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
 
#include "GCloudMessage.h"

GCloudMessage::GCloudMessage(String key)
{
	_token = key;
}
GCloudMessage::~GCloudMessage(void)
{
	
}
String GCloudMessage::notification(String to, String title,String message)
{
	String json = "{\"notification\": {\"title\": \""+title+"\",\"text\": \""+message+"\"},\"to\" : \""+to+"\"}";;
	return send(json);
}
String GCloudMessage::send(String json_string)
{
	HTTPClient http;
	String response = "";
    DEBUG("[GCloudMessage] begin...\n");
    http.begin(_endpoint,_finger); //HTTPS
    http.setUserAgent("Mozilla /5.0");
    http.addHeader("Content-Type","application/json");
    http.addHeader("Authorization","key="+_token);
    DEBUG("[GCloudMessage] POST...\n");
    int httpCode = http.POST(json_string);
	if(httpCode > 0) {
    	DEBUG("[GCloudMessage] POST ... code: %d\n", httpCode);
		if(httpCode == HTTP_CODE_OK) {
        	String resp = http.getString();
        	response = strip(resp,"multicast_id\": ",",");        
        }
    } else {
    	DEBUG("[GCloudMessage] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
	http.end();
	return response;
}