#include <FirebaseArduino.h>
#include <ESP8266WiFi.h> 

 

#define FIREBASE_HOST "heartratetest-b6c92.firebaseio.com"
#define FIREBASE_AUTH "C1aMOzXpolzdAEAkX1qRaiNV08M6uJqxwN5gA33a"
#define WIFI_SSID "SRM-Event"
#define WIFI_PASSWORD "SRMist#3333"


int Signal; 

void setup () {
  Serial.begin (9600);
  pinMode(A0, INPUT);  
  delay (1000);
  WiFi.begin (WIFI_SSID, WIFI_PASSWORD); 
  Serial.print ("Connecting to");
  Serial.print (WIFI_SSID);
  
  while (WiFi.status()!= WL_CONNECTED) {
    Serial.print (".");
    delay (500);
  }
  Serial.println ();
  Serial.print ("Connected to");
  Serial.print (" ");
  Serial.println (WIFI_SSID);
  Serial.print ("IP Address is:");
  Serial.println (WiFi.localIP ()); 
  Firebase.begin (FIREBASE_HOST, FIREBASE_AUTH);  
}
void loop () 
{
 Signal = analogRead(A0); 
  float bpm= Signal*(3.3/2400.0)*100;    //3v3 2400  //Vin 3480  //1023 actual
  Serial.print("\t");
  Serial.print(bpm);
  Serial.print("\t");
  Serial.println(Signal);
  String firebaseBPM = String(bpm);
  delay(1000);

  Firebase.pushString("BPM", firebaseBPM);
  
 
} 
