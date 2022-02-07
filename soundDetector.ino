#include <FirebaseArduino.h> 
#include <ESP8266WiFi.h>


#define FIREBASE_HOST "sound-bbcb8-default-rtdb.europe-west1.firebasedatabase.app"
#define FIREBASE_AUTH "wmEhpVeUumTeM0V9fgQek2jXJfAC71iHZ4ZM8RGA"
#define WIFI_SSID "iPhone od: Dime"
#define WIFI_PASSWORD "klotjevac1"


// analog pin used to connect the potentiometer
int val; 
int ldrPin = A0;
int alarmOn = false;
int alarmPin = 14;
int ledPin = 16;

bool sensorMode = false;
bool lightOn;
bool canTurnLightOn = true;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);   
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  pinMode(ldrPin, INPUT);
  pinMode(alarmPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

} 

int lastRefreshTime = 0;
int refreshTime = 2000;
void loop() {
 
  int ldrValue = analogRead(ldrPin);
  if(millis() -lastRefreshTime >= refreshTime) {
      lastRefreshTime += refreshTime;
      Serial.println("getaj podatke");
      alarmOn = Firebase.getBool("alarmon"); 
      sensorMode = Firebase.getBool("sensormode");
  }
  
  if(ldrValue > 230) {
    Serial.print(ldrValue);
    if(!sensorMode) {
      alarmOn = true;
      Firebase.setBool("alarmon", true);
          Serial.println("oglasavaj alarm");
         } else {
      if (alarmOn) {
        Serial.println("upali sijalicu");
        digitalWrite(ledPin, !digitalRead(ledPin));
        delay(1000);
    } else {
       digitalWrite(ledPin, LOW);
      }
    }
  }

  Buzz();      

  if(!alarmOn && sensorMode) {
     digitalWrite(ledPin, LOW);
  }
  delay(50);
}

void Buzz() { 
  if(alarmOn && !sensorMode) {
    delay(500);
    digitalWrite(alarmPin, HIGH);
    delay(500);
    digitalWrite(alarmPin, LOW);
  } else {
    digitalWrite(alarmPin, LOW);
  }
}
