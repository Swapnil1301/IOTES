#include <DHT.h>
#include <SoftwareSerial.h>
#include <dht.h>   
#define DHTTYPE DHT11   
#define DHTPIN  A2 
DHT dht(DHTPIN, DHTTYPE);
#define MQ2pin (1)
float sensorValue;
#define MQ7pin (0)
float mqValue; 
#define MQ135pin (4)
float sensorValuee;
#define MQ4pin (3)
float mq4Value;
#define soundpin (5)
float soundValue;
#define RX 2
#define TX 3
String AP = "Sdg";       // AP NAME
String PASS = "Password"; // AP PASSWORD
String API = "6FURLLNKTXE95C0C";   // Write API KEY
String HOST = "api.thingspeak.com";
String PORT = "80";
String field = "field1";
int countTrueCommand;
int countTimeCommand;
boolean found = false;
int valSensor = 1;
SoftwareSerial esp8266(RX, TX);
void setup() {
  Serial.begin(9600);
  esp8266.begin(115200);
  sendCommand("AT", 5, "OK");
  sendCommand("AT+CWMODE=1", 5, "OK");
  sendCommand("AT+CWJAP=\"" + AP + "\",\"" + PASS + "\"", 20, "OK");
  dht.begin();
  Serial.println("Gas sensor is giving you a warning now");
  delay(50); //Allows the MQ2 Sensor some time to react
  Serial.begin(9600); // sets the serial port to 9600
}

void loop() {
  delay(100);
  // put your main code here, to run repeatedly:
  String getData = "GET /update?api_key=" + API + "&field1=" + getTemperaturevalue() + "&field2=" + getHumidvalue() + "&field3=" + getsmokevalue() + "&field4=" + getcovalue() + "&field5=" + getnaturalgasvalue() + "&field6=" + getairpollutionvalue() + "&field7=" + getsoundvalue();
  sendCommand("AT+CIPMUX=1", 5, "OK");
  sendCommand("AT+CIPSTART=0,\"TCP\",\"" + HOST + "\"," + PORT, 15, "OK");
  sendCommand("AT+CIPSEND=0," + String(getData.length() + 4), 4, ">");
  esp8266.println(getData); delay(150); countTrueCommand++;
  sendCommand("AT+CIPCLOSE=0", 5, "OK");
}
String getTemperaturevalue() {

  // dht.read(DHTPIN);
  Serial.print("Temperature(C)=");
  float t = dht.readTemperature();
  Serial.println(t);
  delay(50);
  Serial.println("----------------------------------------------------------------");
  return String(t);

}
String getHumidvalue()
{
  //dht.read(DHTPIN);
  Serial.print("Humidity in%=");
  float h = dht.readHumidity();
  Serial.println(h);
  delay(50);
  return String(h);
}
String getsmokevalue() {
   sensorValue = analogRead(MQ2pin); // read analog input pin 0
  
  Serial.print("Smoke Value: ");
  Serial.print(sensorValue);
  
  if(sensorValue > 280)
  {
    Serial.print(" | Smoke detected!");
  }
  
  Serial.println("");
  delay(50);
  return String(sensorValue);
}
String getcovalue() {
   mqValue= analogRead(MQ7pin); // read analog input pin 0
  Serial.print("CO Value: ");
  Serial.print(mqValue);
  Serial.println("");
  delay(50);
  return String(mqValue);

}

String getairpollutionvalue() {
  sensorValuee = analogRead(MQ135pin); // read analog input pin 0
  Serial.print("Air Quality Value: ");
  Serial.print(sensorValuee);
  Serial.println("");
  delay(50);
  return String(sensorValuee);
}
String getnaturalgasvalue() {
  mq4Value = analogRead(MQ4pin); // read analog input pin 0
  Serial.print("Methane Value: ");
  Serial.print(mq4Value);
  Serial.println("");
  delay(50);
  return String(mq4Value);

}
String getsoundvalue() {
soundValue = analogRead(soundpin); // read analog input pin 0
  Serial.print("Sound Sensor Value: ");
  Serial.print(soundValue);
  Serial.println(""); 
  delay (50);
  return String(soundValue);
}



void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while (countTimeCommand < (maxTime * 1))
  {
    esp8266.println(command);//at+cipsend
    if (esp8266.find(readReplay)) //ok
    {
      found = true;
      break;
    }

    countTimeCommand++;
  }

  if (found == true)
  {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }

  if (found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }

  found = false;
}