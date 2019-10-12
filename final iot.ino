#include<TinyGPS++.h>
#include<SoftwareSerial.h>
#include <FirebaseArduino.h>
#include <ESP8266WiFi.h> 



#define FIREBASE_HOST "heart-7e56b.firebaseio.com"
#define FIREBASE_AUTH "4tBg0bhkZHMQtMheJF1QlWK0aSJSFyFliFIq3KrM"
#define WIFI_SSID "SRM-Event"
#define WIFI_PASSWORD "SRMist#3333"


//variables
int Signal;                              //Raw value of the heartt rate sensor
long double startTime=0;  
int val;                                 //value of the button
int flag=0;                              //to prevent call back
static const uint32_t GPSBaud=9600;
float thresh;
       

TinyGPSPlus gps;    //Instance of the library.
SoftwareSerial ss(D6,D7);



void setup() 
{
  pinMode(A0, INPUT);          //heart rate sensor     
  pinMode(D2, OUTPUT);         //vibrating motor
  pinMode(D0, INPUT);          //button switch
  Serial.begin(9600);          //Serial Monitor
  ss.begin(GPSBaud);           //GPS begin.

   delay (1000);
  WiFi.begin (WIFI_SSID, WIFI_PASSWORD); 
  Serial.print ("Connecting to");
  Serial.print (WIFI_SSID);
  
  while (WiFi.status()!= WL_CONNECTED) 
  {
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



void loop() 
{
  Signal = analogRead(A0); 
  float bpm= Signal*(3.3/2400.0)*100;    
  Serial.print("\t");
  Serial.print(bpm);
  Serial.print("\t");
  Serial.print(Signal);
  Serial.print("\t");
  val = digitalRead(D0);
  Serial.print(flag);
  Serial.print("\t");
  Serial.println(val);
  String firebaseBPM = String(bpm);
  Firebase.setString("/heart_rt/BPM", firebaseBPM);

  //get data from data base
  thresh = Firebase.getFloat("/heart_rt/threshold");
  Serial.println(thresh);
  if(flag==0)
  { if(thresh==1)                                        //threshold for Vibrator to start will be taken from database and compared with bpm
    {   startTime=millis(); 
        digitalWrite(D2, HIGH);
        flag=1;
    }
  }

  if(flag==1)
  {
  if(millis()-startTime>=30000)                             //30 seconds time out
  {   digitalWrite(D2, LOW);    
      flag=0;
      Serial.println("Triggered");
      while (ss.available() > 0)
      {
        gps.encode(ss.read());
        if (gps.location.isUpdated())
        {
          Serial.print("Latitude= "); 
          Serial.print(gps.location.lat(), 6);
          Serial.print("Longitude= "); 
          Serial.println(gps.location.lng(), 6);
          String firebaseLAT = String(gps.location.lat());
          String firebaseLNG = String(gps.location.lng());

         //Firebase.pushString("location/lat", firebaseLAT);
         //Firebase.pushString("location/lng", firebaseLNG);
        }
       
      }
  
  }
  }
  if((flag==1)&&(val==1))
  {   digitalWrite(D2,LOW);   //OFF for FALSE alarm.
      flag=0;
  }

  //Manual override
  else if((flag==0)&&(val==1))
  {
     startTime = millis();
     digitalWrite(D2,HIGH);
     flag = 1;
  }
     
  delay(2000);
}                                

/*void Track()
{
  while (ss.available() > 0)
  {
    gps.encode(ss.read());
      if (gps.location.isUpdated())
      {
      Serial.print("Latitude= "); 
      Serial.print(gps.location.lat(), 6);
      Serial.print(" Longitude= "); 
      Serial.println(gps.location.lng(), 6);
      }
  }  
}*/
