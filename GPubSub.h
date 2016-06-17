/**
 * @file GPubSub.h
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
 
#ifndef GPUBSUB_H_
#define GPUBSUB_H_

#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include "FS.h"
#include "OAuth2.h"
#include "Helper.h"
#include <base64.h>

#define DEBUG(...) Serial.printf( __VA_ARGS__ )

class GPubSub
{
public:    
    GPubSub(void);
    ~GPubSub(void);
    bool begin(String client_id, String client_secret,String project_id,bool renew_token=false);    
    bool loadConfig();
    bool writeConfig(); 

    bool createTopic(String topic);
    //String[] listTopic(); TODO : implement this
    //bool deleteTopic(); TODO : implement this
    bool publish(String topic,String data);
    String getLastAckId();
    bool run();
    bool subscribe();

private:    
    
    OAuth2 oauth;    
    const String _finger_pubsub ="3d 49 7f d9 3e 17 0e 2a 0f d8 ac 1f 83 63 8b 29 ed 27 44 2f";   
    const String _scope = "https://www.googleapis.com/auth/pubsub,https://www.googleapis.com/auth/cloud-platform";    
    String _token = "";
    String _refresh_token = "";
    String _project_id = "";
};

#endif /* GPUBSUB_H_ */

