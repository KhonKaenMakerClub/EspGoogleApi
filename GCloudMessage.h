/**
 * @file GCloudMessage.h
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
 
#ifndef GCLOUDMESSAGE_H_
#define GCLOUDMESSAGE_H_

#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include "Helper.h"

#define DEBUG(...) //Serial.printf( __VA_ARGS__ )

class GCloudMessage
{
public:    
    GCloudMessage(String key);
    ~GCloudMessage(void);
    String notification(String to, String title,String message);
    String send(String);    
private:
    const String _finger = "f4 26 6b 8f df 92 5b 24 8b 01 80 59 6b 43 07 e5 62 bb 36 a9";
    const String _endpoint = "https://gcm-http.googleapis.com/gcm/send";
    String _token = "";   
};

#endif /* GCLOUDMESSAGE_H_ */

