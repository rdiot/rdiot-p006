/* UVC Camera Movie Monitoring [P006] : http://rdiot.tistory.com/325 [RDIoT Demo] */

#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>
//https://www.arduino.cc/en/Tutorial/Bridge
#include <Servo.h> 
 
YunServer server;

Servo myservo1;
Servo myservo2;

int servoPin_leftright = 9;
int servoPin_updown = 10;

int pos_updown = 0;
int pos_leftright = 30;

void setup() {
  //Serial.begin(9600);
  pinMode(13,OUTPUT);
  
  digitalWrite(13, LOW);
  Bridge.begin();
  digitalWrite(13, HIGH);
  
  server.listenOnLocalhost();
  server.begin();

  myservo1.attach(servoPin_leftright); 
  myservo2.attach(servoPin_updown); 
}
 
void loop() {
  YunClient client = server.accept();
 
  if (client) {
    process(client);
    client.stop();
  }

  delay(50); 
}
 
void process(YunClient client) {
  String command = client.readStringUntil('/');
 
  if (command == "digital") {
    digitalCommand(client);
  }
  if (command == "analog") {
    analogCommand(client);
  }
  if (command == "mode") {
    modeCommand(client);
  }
  if (command == "control") {
    controlCommand(client); 
  }
}
 
void digitalCommand(YunClient client) {
  int pin, value;
pin = client.parseInt();
 
  if (client.read() == '/') {
    value = client.parseInt();
    digitalWrite(pin, value);
  } 
  else {
    value = digitalRead(pin);
  }
  client.print(F("Pin D"));
  client.print(pin);
  client.print(F(" set to "));
  client.println(value);
 
  String key = "D";
  key += pin;
  Bridge.put(key, String(value));
}
 
void analogCommand(YunClient client) {
  int pin, value;
 
  pin = client.parseInt();
 
  if (client.read() == '/') {
    value = client.parseInt();
    analogWrite(pin, value);
 
    // Send feedback to client
    client.print(F("Pin D"));
    client.print(pin);
    client.print(F(" set to analog "));
    client.println(value);
 
    String key = "D";
    key += pin;
    Bridge.put(key, String(value));

  }
  else {
    value = analogRead(pin);
 
    client.print(F("Pin A"));
    client.print(pin);
    client.print(F(" reads analog "));
    client.println(value);
 
    String key = "A";
    key += pin;
    Bridge.put(key, String(value));
  }
}
 
void modeCommand(YunClient client) {
  int pin;
  pin = client.parseInt();
if (client.read() != '/') {
    client.println(F("error"));
    return;
  }
 
String mode = client.readStringUntil('\r');
 
  if (mode == "input") {
    pinMode(pin, INPUT);
    // Send feedback to client
    client.print(F("Pin D"));
    client.print(pin);
    client.print(F(" configured as INPUT!"));
    return;
  }
 
  if (mode == "output") {
    pinMode(pin, OUTPUT);
    // Send feedback to client
    client.print(F("Pin D"));
    client.print(pin);
    client.print(F(" configured as OUTPUT!"));
    return;
  }
 
  client.print(F("error: invalid mode "));
  client.print(mode);
}

void controlCommand(YunClient client) {
  
  if (client.read() != '/') {
    client.println(F("error"));
    return;
  }
 
String control = client.readStringUntil('\r');
 
  if (control == "left") {    
    
    client.print(F("Current Position : "));
    client.print(pos_leftright);      
    if(pos_leftright != 0)
    {
      pos_leftright = pos_leftright-1; 
      myservo1.write(pos_leftright);
    }
    
    return;
  }

  client.print(F("error: invalid contol "));
  client.print(control);
}
