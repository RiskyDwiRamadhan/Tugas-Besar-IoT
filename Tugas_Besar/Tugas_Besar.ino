#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define Lampu D1 //lampu
#define PUTIH D2 //led warnah biru
#define Biru D3 //led warnah biru

const char *ssid = "Risky";//silakan disesuaikan sendiri
const char *password = "risky0712";//silakan disesuaikan sendiri
const char *mqtt_server = "broker.hivemq.com";
WiFiClient espClient22;
PubSubClient client(espClient22);

void setup_wifi(){
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageInfo;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageInfo += (char)message[i];
  }
  Serial.println();

  // If a message is received on the topic room/lamp, you check if the message is either on or off. Turns the lamp GPIO according to the message
  if(topic=="kel8/light"){
      Serial.print("Changing Room Light to ");
      if(messageInfo == "Lampu 1 On"){
        digitalWrite(Lampu, LOW);
        Serial.print("Lampu 1 Menyala");
      }
      else if(messageInfo == "Lampu 1 Off"){
        digitalWrite(Lampu, HIGH);
        Serial.print("Lampu 1 Mati");
      }
      else if(messageInfo == "Lampu 2 On"){
        digitalWrite(PUTIH, HIGH);
        Serial.print("Lampu 2 Menyala");
      }
      else if(messageInfo == "Lampu 2 Off"){
        digitalWrite(PUTIH, LOW);
        Serial.print("Lampu 2 Mati");
      }
      else if(messageInfo == "Lampu 3 On"){
        digitalWrite(Biru, HIGH);
        Serial.print("Lampu 3 Menyala");
      }
      else if(messageInfo == "Lampu 3 Off"){
        digitalWrite(Biru, LOW);
        Serial.print("Lampu 3 Mati");
      }
      else if(messageInfo == "Semua Lampu On"){
        digitalWrite(Lampu, LOW);
        digitalWrite(PUTIH, HIGH);
        digitalWrite(Biru, HIGH);
        Serial.print("Lampu 3 Menyala");
      }
      else if(messageInfo == "Semua Lampu Off"){
        digitalWrite(Lampu, HIGH);
        digitalWrite(PUTIH, LOW);
        digitalWrite(Biru, LOW);
        Serial.print("Lampu 3 Mati");
      }
  }
  Serial.println();
}

void reconnect()
{
  while(!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client22"))
    {
      Serial.println("connected");
      client.subscribe("kel8/light");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup()
{
  pinMode(Biru, OUTPUT);
  pinMode(PUTIH, OUTPUT);
  pinMode(Lampu, OUTPUT);
  digitalWrite(Lampu, HIGH);
  
  Serial.begin(115200);
  Serial.println("Mqtt Node-RED");
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  if(!client.loop())
    client.connect("ESP8266Client22");

  }
