/**
 * @file Helper.h
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
#ifndef HELPER_H_
#define HELPER_H_
#include <Arduino.h>

inline String strip(String haystack,String st,String ed)
{
  int stind = haystack.indexOf(st)+st.length();
  return haystack.substring(stind,haystack.indexOf(ed,stind+1));  
}

inline bool stripStream(String terminate,Stream *s,int len)
{
  bool found = false;
  int ix = 0;
  int rcount = 0;
  int l = terminate.length();
  bool f = false;  
  unsigned long st = millis() + (3*1000);
  while(found == false && millis() < st){
    while(s->read() != terminate[ix]){
      if(len != -1 && rcount > len) return false;
      rcount++;
      delay(0);
    }
    ix++;
    while(s->read() == terminate[ix]){
      ix++;
      rcount++;
      if(ix == l){
        found = true;
        break;
      }
      delay(0);
    }    
    ix = 0;
    delay(0);
  }
  return found;
}
#endif /* HELPER_H_ */