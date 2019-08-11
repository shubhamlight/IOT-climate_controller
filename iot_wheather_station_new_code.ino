
#include <ESP8266WiFi.h>


// www.arduinesp.com
//
// Plot DTH11 data on thingspeak.com using an ESP8266
// April 11 2015
// Author: Jeroen Beemster
// Website: www.arduinesp.com

#include <DHT.h>

float sound;

// replace with your channel’s thingspeak API key,
String apiKey = "RI5288HWE5CFESMS";
const char* ssid = "TP-LINK_30A26C";
const char* password = "Ayush123@";

const char* server = "api.thingspeak.com";
#define DHTPIN 2 // what pin we’re connected to

DHT dht(DHTPIN, DHT11);
WiFiClient client;

void setup() {
Serial.begin(115200);
delay(10);
dht.begin();

WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("WiFi connected");

}

void loop() {
  if(WiFi.status() != WL_CONNECTED){
    WiFi.begin(ssid,password);
    
  }
  

float h = dht.readHumidity();
float t = dht.readTemperature();
float hic = dht.computeHeatIndex(t,h, false);
if (isnan(h) || isnan(t)) {
Serial.println("Failed to read from DHT sensor!");
return;
}
sound = 331.3 + (0.606*t);
// code for relay ,to swith on the appliance
 if ( t <= 34 )
{digitalWrite (11, HIGH) };
else (t >= 34 )
{digitalWrite (11, LOW) };
//code ends here for relay

if (client.connect(server,80)) { // "184.106.153.149" or api.thingspeak.com
String postStr = apiKey;
postStr +="&field1=";
postStr += String(t);
postStr +="&field2=";
postStr += String(h);
postStr +="&field3=";
postStr += String(hic);
postStr +="&field4=";
postStr += String(sound);
postStr += "\r\n\r\n";

client.print("POST /update HTTP/1.1\n");
client.print("Host: api.thingspeak.com\n");
client.print("Connection: close\n");
client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
client.print("Content-Type: application/x-www-form-urlencoded\n");
client.print("Content-Length: ");
client.print(postStr.length());
client.print("\n\n");
client.print(postStr);

Serial.print("Temperature: ");
Serial.print(t);
Serial.print(" degrees Celcius Humidity: ");
Serial.print(h);
Serial.print("Heat Index");
Serial.print(hic);
Serial.print("speed of sound :");
Serial.print(sound);
Serial.println("% send to Thingspeak");
}
client.stop();

Serial.println("Waiting…");
// thingspeak needs minimum 15 sec delay between updates
delay(15000);
}
