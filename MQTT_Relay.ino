#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#define LED 2     // PIN D4

#define wifi_ssid "SSID"           // SSID
#define wifi_password "PASSWORD"   // Password WiFi

#define mqtt_server "IP_Address"   // Broker MQTT
//#define mqtt_user "user"
//#define mqtt_password "password"

int servo_pin = 16 ;  // PIN D0

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
   delay(100);
   // koneksi pada jaringan WIFI
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      Serial.print(".");
    }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Command from MQTT broker is : [");
  Serial.print(topic);
  Serial.print("   ");
  
  int p =(char)payload[0]-'0';
  // if MQTT=0, Relay mati di pin D1
  if(p==0)
  {
    digitalWrite(LED, LOW);
    digitalWrite(servo_pin, LOW);
    Serial.println(" Relay Mati! ");
  } 
  // if MQTT=1, Relay aktif di pin D1
  if(p==1)
  {
    digitalWrite(LED, HIGH);
    digitalWrite(servo_pin, HIGH);
    Serial.println(" Relay Aktif! ");
  }
    Serial.println();
}
//end callback

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    
    // Create Name ClientId
    String clientId = "ESP8266";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str()))
    {
      Serial.println("connected");
      client.subscribe("akuator/relay"); //Topik MQTT yang di Subscribe
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // delay 5 detik
      delay(5000);
    }
  }
} //end reconnect()

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode(LED,OUTPUT);
  pinMode(servo_pin, OUTPUT);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
