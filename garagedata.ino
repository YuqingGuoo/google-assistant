/******************** Documentation Section *******************************
This program is for Smart garage Automation
Garage can be closed or Opened using Google Assistant. 
Garage door will open only when you are 150cm near it.
Else it will remain closed .

date : 20th Jan 2018

Author : Harshit Srivastava

 ****************************************************/
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "XXXXXXXX" Router's SSID/ NAME
#define WLAN_PASS       "XXXXXXXX" Router,s Password

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "XXXXXXX"               // Adafruit IO Username 
#define AIO_KEY         "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" // Adafruit IO Key
/*************************** Global Variables *****************************************/
#define trig_pin D1 // D1 of nodeMCU is connected to trig
#define echo_pin D0 // D0 of nodeMCU is connected to echo
#define Lightpin D3 // connected to LED

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/


// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish garage_dist = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/garage_dist");

// Setup a feed called 'Sensor_lock' for subscribing to changes.
Adafruit_MQTT_Subscribe Sensor_lock = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Sensor_lock");

/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();

void setup() {
  Serial.begin(115200); // Sets baud rate for serial Communication
  delay(10);
  pinMode(trig_pin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echo_pin, INPUT); // Sets the echoPin as an Input
  pinMode(Lightpin , OUTPUT); // Sets Lightpin as O/P
  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  
  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

    mqtt.subscribe(&Sensor_lock);
}

uint32_t x=0;
int state = 0; // 0 means off state
               // 1 means on state 
void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here
  
  
 Adafruit_MQTT_Subscribe *subscription;
 
  while ((subscription = mqtt.readSubscription(5000))) {
   
    if (subscription == &Sensor_lock ) {
      Serial.print(F("Got Sensor_lock: "));
      Serial.println((char *)Sensor_lock.lastread);
      int dist = MeasureDistance();
      Serial.print(dist);
      Serial.println("  cm");
      if ( ((char *)Sensor_lock.lastread)[1] == 'F' )
      {
         state = 0; 
      }
      
      else if ( ((char *)Sensor_lock.lastread)[1] == 'N' )
      {     
         state = 1;
      }    
    }
  } 

   
  if (state == 1 )
  {
    if ( dist<=150 )
    {
        digitalWrite(Lightpin,HIGH); // Switches on Relay
      }

    else if (dist >150)
    {
          // Now we can publish stuff!
          Serial.print(F("\nSending distance val "));
          
          Serial.print(dist );
          Serial.print(" cm");
          
          if (! garage_dist.publish(dist)) 
          {
            Serial.println(F("Failed"));
          }
           
          else 
          {
            Serial.println(F("OK!"));
          }
        
        delay(100);
      
     } 
   }

   else if (state == 0)
   {
      digitalWrite(lightPin , LOW ); // Switches off Relay 
    }
    
  // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds
  /*
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
  */
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) 
  { 
    // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(3000);  // wait 3 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}

int MeasureDistance () // Functions measures distance for between person & garage 
{
    int duration,distance;
    // Clears the trigPin
    digitalWrite(trig_pin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trig_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig_pin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echo_pin, HIGH);
    // Calculating the distance in cm
    distance= duration*0.034/2;
    return distance;
}
