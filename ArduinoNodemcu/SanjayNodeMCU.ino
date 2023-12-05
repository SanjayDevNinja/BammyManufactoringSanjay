/*  ___   ___  ___  _   _  ___   ___   ____ ___  ____  
 * / _ \ /___)/ _ \| | | |/ _ \ / _ \ / ___) _ \|    \ 
 *| |_| |___ | |_| | |_| | |_| | |_| ( (__| |_| | | | |
 * \___/(___/ \___/ \__  |\___/ \___(_)____)___/|_|_|_|
 *                  (____/ 
 * This tutorial is about how to remote control a servo by MQTT protocol
 * Tutorial URL http://osoyoo.com/2017/05/18/nodemcu-lesson-17-servo-mqtt/
 * CopyRight www.osoyoo.com
 */
#include <ArduinoWebsockets.h>
#include <Servo.h>

#include <ESP8266WiFi.h>

// Define NodeMCU D2 pin  connect to Piezo Buzzer
#define BUZZER_PIN D2

//WIFI Settings
const char* ssid = "laptoplocal";
const char* password = "sanjay1234";
const char* websocket_server_host = "192.168.137.1";  //The webservers IP address
const uint16_t websocket_server_port = 8887;          //Port to identify this device

using namespace websockets;
WebsocketsClient client;

Servo myservo;  // create servo object to control a servo
const int trigPin = 12;
const int echoPin = 14;

//define sound velocity in cm/uS
#define SOUND_VELOCITY 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;
float distanceInch;


int count = 0;
int countTarget = 0;

void buzzTime(int time) {

  digitalWrite(BUZZER_PIN, HIGH);
  delay(time);
  digitalWrite(BUZZER_PIN, LOW);
}


void onMessageCallback(WebsocketsMessage message) {
  String data = message.data();
  Serial.println(data);

  countTarget = data.toInt();
  buzzTime(3000);
}



void setup() {
  WiFi.begin(ssid, password);
  Serial.begin(115200);

  pinMode(trigPin, OUTPUT);  // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);   // Sets the echoPin as an Input
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  buzzTime(1000);////////////////////////////////////////

  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting network!");
    delay(500);
  }
  Serial.println("Sanjay Connected network!");

  myservo.attach(D1);  // attaches the servo on pin D1 to the servo object
  myservo.write(100);  // tell servo to go to position in variable '(int)payload[i]'
  client.onMessage(onMessageCallback);
}

void loop() {

  client.poll();

  
  while (!client.connect(websocket_server_host, websocket_server_port, "/")) {
    Serial.println("Connecting client!");
    delay(500);
  }

  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculate the distance
  distanceCm = duration * SOUND_VELOCITY / 2;

  // Convert to inches
  distanceInch = distanceCm * CM_TO_INCH;

  // Prints the distance on the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  Serial.print("Distance (inch): ");
  Serial.println(distanceInch);

  delay(500);

  //count bammy
  if (distanceCm < 12) {
    for(int i=100;i<170;i=i+3){
      myservo.write(i);
      delay(10);
    }

    for(int i=170;i > 90;i=i-3){
      myservo.write(i);
      delay(10);
    }    
     
    count++;
    Serial.println(count);
    client.send(String(count));
    delay(1000);
  }



  
}