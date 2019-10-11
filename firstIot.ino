int Signal;
long double startTime=0;
int value;

void setup() 
{
  pinMode(A0, INPUT);          //heart rate sensor     
  pinMode(D2, OUTPUT);         //vibrating motor
  pinMode(D0, INPUT);          //button switch
  Serial.begin(9600);
  
}

void loop() 
{
  Signal = analogRead(A0); 
  float bpm= Signal*(3.3/2400.0)*100;    //3v3 2400  //Vin 3480  //1023 actual
  Serial.print("\t");
  Serial.print(bpm);
  Serial.print("\t");
  Serial.println(Signal);
  value = digitalRead(D0);
  Serial.println(value);
  if(Signal>600)    //threshold for Vibrator to start.
  {   startTime=millis();
      digitalWrite(D2, HIGH);
  }
  
  if(millis()-startTime>=15000) //10 seconds run out
  {   digitalWrite(D2, LOW);    //message to be sent  
      //void track();
  }
  else if(value==0)
  {   digitalWrite(D2,LOW);   //OFF for FALSE alarm.
  }
    
  delay(3000);
}                                
       
