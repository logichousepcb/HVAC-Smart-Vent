#define CURR_VERS              "0.2"
#define CURR_VERS_NAME         "-VENT"
#define MOTOR_RUN_TIME             10

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>

#include <Adafruit_NeoPixel.h> 
#include <PubSubClient.h>   //https://github.com/knolleary/pubsubclient
#include <WebConfig.h>
#include "config.h"
#include "HTML.h"
/********** DALLAS TEMP *********/
#include <OneWire.h> 
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 14 
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);

const byte interruptPin = 13;
char* switchState = "UNKNOWN";
int switchStateint = 0;

/*********** MQTT STUFF ************/
bool MQTT_connected = false;
bool boot = true;
bool freshboot = true;

byte willQoS = 0;
//const char* willTopic = "/checkIn";
const char* willMessage = "Offline";
boolean willRetain = true;
/***************************** for recieved msg ***************************/
char* mainTopic;
char* willTopic;
char* stateTopic = "/stat";
char* reportTopic = "/tele";
char* cmndTopic = "/cmnd"; 
String mainTopicS;
int maxTopicLength = 10;
char c[20];
String unique_id = "";
char unique_char;
/************************** UPTIME counter *********************************/
long Day=0;
int Hour =0;
int Minute=0;
int Second=0;
int HighMillis=0;
int Rollover=0;

/*********** Status LED Stuff ************/
#define RGB_PIN      12
#define NUMPIXELS 1
Adafruit_NeoPixel status_rgb(NUMPIXELS, RGB_PIN, NEO_GRB + NEO_KHZ800);



ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;

WebConfig conf;

boolean initWiFi() {
    boolean connected = false;
    // WiFi.mode(WIFI_STA);
    Serial.print("Verbindung zu ");
    Serial.print(conf.values[0]);
    Serial.println(" herstellen");
    if (conf.values[0] != "") {
      WiFi.begin(conf.values[0].c_str(),conf.values[1].c_str());
      uint8_t cnt = 0;
      while ((WiFi.status() != WL_CONNECTED) && (cnt<40)){
        delay(500);
        Serial.print(".");
        cnt++;
      }
 //     Serial.println();
    if (WiFi.status() == WL_CONNECTED) {
       Serial.print("IP-Adresse = ");
       Serial.println(WiFi.localIP());
        connected = true;
      }
    }
    if (!connected) {
          WiFi.mode(WIFI_AP);
          WiFi.softAP(conf.getApName(),"",1);  
    }
    return connected;
}




void setup() {
  Serial.begin(115200);
  Serial.println(params);
  conf.setDescription(params);
  conf.readConfig();
  mainTopicS = conf.getValue("mqtt-topic");           //load main topic from config file

  status_rgb.begin();
  status_rgb.clear();
  status_rgb.setBrightness(100);
  status_rgb.setPixelColor(0, status_rgb.Color(255, 0, 0  ));
  status_rgb.show();

  pinMode(interruptPin, INPUT_PULLUP);
  
 // attachInterrupt(digitalPinToInterrupt(interruptPin), buttonpushed, CHANGE);
  
  pinMode (4, OUTPUT);
  pinMode (5, OUTPUT);
  sensors.begin(); 
  
  initWiFi();
  char dns[30];

 httpUpdater.setup(&server);
 MDNS.addService("http", "tcp", 80);
 Serial.printf("HTTPUpdateServer ready! Open http://%s.local/update in your browser\n", dns);
  
  sprintf(dns,"%s.local",conf.getApName());
  if (MDNS.begin(dns)) {
    Serial.println("MDNS started");
  }

 

 

// WEB UI page assignments
  server.on("/",handleRoot);
  server.on("/config",handleConfig);
  server.on("/opencontrol",handleOpenControl);
  server.on("/closecontrol",handleCloseControl);
  server.on("/restart",handleRestart);

// START Web UI server  
   server.begin(80);
}

void show_status() {
status_rgb.clear();
status_rgb.setBrightness(250);
status_rgb.setPixelColor(0, status_rgb.Color(55, 155, 0));
status_rgb.show();
delay (2000);
status_rgb.show();
status_rgb.setPixelColor(0, status_rgb.Color(155, 55, 66));
status_rgb.clear();
status_rgb.show(); 
}

void loop() {
  // put your main code here, to run repeatedly:
 // show_status();
  if (digitalRead(interruptPin) == 0)  { buttonpushed(); 
  };
  server.handleClient();
  MDNS.update();
}

void buttonpushed() {

 Serial.println("Button pressed");
 status_rgb.clear();
 status_rgb.setBrightness(250);
 status_rgb.setPixelColor(0, status_rgb.Color(0, 255, 255));
 status_rgb.show();
 
 switch (switchStateint) {
    case 0:
      handleCloseControl();
      break;
    case 1:
      handleOpenControl();
      break;
    default:
      break;
 }
}
