// Base ESP8266
#include <ESP8266WiFi.h>
WiFiClient WIFI_CLIENT;

// MQTT
#include <PubSubClient.h>
PubSubClient MQTT_CLIENT;

//for HTTP protocol
#include <ESP8266WebServer.h>
ESP8266WebServer server(80); //Server on port 80

#include "index.h" //Our HTML webpage contents with javascripts

#define LED 2

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void handleRoot() {
 Serial.println("You called root page");
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}
 
void handleLEDon() { 
 Serial.println("LED on page");
 digitalWrite(LED,LOW); //LED is connected in reverse
 server.send(200, "text/html", "ON"); //Send ADC value only to client ajax request
}
 
void handleLEDoff() { 
 Serial.println("LED off page");
 digitalWrite(LED,HIGH); //LED off
 server.send(200, "text/html", "OFF"); //Send ADC value only to client ajax request
}
//==============================================================
//                  SETUP
//==============================================================
void setup() {
  // Initialize the serial port
  Serial.begin(115200);

 //Onboard LED port Direction output
  pinMode(LED,OUTPUT); 
  //Power on LED state off
  digitalWrite(LED,HIGH);

  // Attempt to connect to a specific access point
  WiFi.begin("OneNetworkToRuleThemAll(IoT)", "arduino3245");

  // Keep checking the connection status until it is connected
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
  }


  // Print the IP address of your module
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);      //Which routine to handle at root location. This is display page
  server.on("/ledOn", handleLEDon); //as Per  <a href="ledOn">, Subroutine to be called
  server.on("/ledOff", handleLEDoff);
 
  server.begin();                  //Start server

  Serial.println("HTTP server started");
}

// This function handles received messages
void myMessageArrived(char* topic, byte* payload, unsigned int length) {
  // Convert the message payload from bytes to a string
  String message = "";
  for (unsigned int i=0; i< length; i++) {
    message = message + (char)payload[i];
  }
   
  // Print the message to the serial port
  Serial.println(message);
}

// This function connects to the MQTT broker
void reconnect() {
  // Set our MQTT broker address and port
  MQTT_CLIENT.setServer("broker.hivemq.com", 1883);
  MQTT_CLIENT.setClient(WIFI_CLIENT);

  // Loop until we're reconnected
  while (!MQTT_CLIENT.connected()) {
    // Attempt to connect
    Serial.println("Attempt to connect to MQTT broker");
    MQTT_CLIENT.connect("Narya123");

    // Wait some time to space out connection requests
    delay(3000);
  }

  Serial.println("MQTT connected");

  // Subscribe to the topic where our web page is publishing messages
  MQTT_CLIENT.subscribe("LOTR/Narya");

  // Set the message received callback
  MQTT_CLIENT.setCallback(myMessageArrived);
}

// This function runs over and over again in a continuous loop

//==============================================================
//                     LOOP
//==============================================================
void loop() {
    server.handleClient(); 
    
  if (!MQTT_CLIENT.connected()) {
    // If we're not, attempt to reconnect
    reconnect();
  }

  // Publish a message to a topic
 //MQTT_CLIENT.publish("LOTR/fromNarya", "Hello world!");

  // Check for incoming MQTT messages
  MQTT_CLIENT.loop();
}
