/*
 ESP8266 JQUERY DEMO
 Marco Campinoti
  
 This is free software. Use, modify and tinker with it however you like!
 LICENSED UNDER CC-BY-4.0 http://creativecommons.org/licenses/by/4.0/
 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

ADC_MODE(ADC_VCC);      //needed for ESP.getVcc()

const char RootPage_Header[] PROGMEM = "<!DOCTYPE html>\
<html>\
<head>\
<meta lang='en' http-equiv='Content-Type' content='text/html' charset='UTF-8'>\
<title>JQUERY DEMO</title>\
<style>\
  body { background-color: #ffffff; font-family: Arial, Helvetica, Sans-Serif; Color: black; font-size: 32px;}\
</style>\
</head>";


//******************************************************
//* JQUERY from the WebPage to the Server              *
//******************************************************
const char RootPage_JQuery_Script[] PROGMEM = "<script>\
setInterval(function() { getData();}, 1000);\
function getData() {\
  var xhttp = new XMLHttpRequest();\
  var reply = new Array;\
  xhttp.onreadystatechange = function() {\
     if (this.readyState == 4 && this.status == 200){\
      reply = this.responseText.split(',');\
      document.getElementById('rssi').innerHTML = reply[0];\
      document.getElementById('vcpu').innerHTML = reply[1];\
    }\
  };\
  xhttp.open('GET', 'jquery', true);\
  xhttp.send();\
}\
</script>";
//******************************************************

const char RootPage_Body[] PROGMEM = "<body>\
  <h3> RSSI (dbm) = <a id='rssi' style='padding-right:10px'> rssi </a></h3>\
  <h3> VCPU (mV)  = <a id='vcpu' style='padding-right:10px'> vcpu </a></h3>\
</body></html>";

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(74880);
  while (!Serial) {
    ; // wait for serial port to connect
  }
  
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(F("Connecting to "));
    Serial.println("YourSSID");
    
    WiFi.persistent(false);       // WiFi config isn't saved in flash
    WiFi.mode(WIFI_STA);          // use WIFI_AP_STA if you want an AP
    WiFi.hostname("JQUERY");    // must be called before wifi.begin()
    WiFi.begin("YourSSID", "YourPassword");
   
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(F("."));
    }
  }
     
  Serial.println();
  Serial.print(F("IP address: "));
  Serial.println(WiFi.localIP());  
  
  server.begin();
  server.on("/", handleRoot);
  server.on("/jquery", handleJQuery);
}

//***********************************************************
//* JQUERY REPLIES from Server to WebPage                   *
//*********************************************************** 
void handleJQuery() {

  // sends data as an array, the values are comma separated
  server.send ( 200, "text/plain", String(WiFi.RSSI()) + "," + String(ESP.getVcc()));                                 
}
//*********************************************************** 

void handleRoot(){
  WiFiClient client = server.client();
  
  if (client) {
    Serial.println("new client");
    
    //prepare the webpage
    String page = FPSTR(RootPage_Header);
    page += FPSTR(RootPage_JQuery_Script);
    page += FPSTR(RootPage_Body);
    
    // Serve page actually
    server.sendHeader(F("Content-Length"), String(page.length()));
    server.send ( 200, F("text/html"), page );
  }

  // give the web browser time to receive the data
  delay(1);

  // close the connection
  client.stop();
  Serial.println("client disconnected");
}

void loop() {
  // listen for incoming clients
  server.handleClient();
}
