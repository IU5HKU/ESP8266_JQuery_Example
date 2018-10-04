# ESP8266_JQuery_Example
A simple (naive?) example to handle JQuery requests from a webpage in ESP8266

This example can be adapted to any boards, isn't so much esp8266 specifi, anyway i've wrote it for use in IoT project (i use it in MiniWX), and in substance handle the requests that a webpage submits with a JavaScritp to a Server, requesting data to update dynamically in the page.
In the example webpage the script requests every seconds two values, and the server send requested infos as an array of comma separated values, the script at least will place received values where they have to be.
The two foundamentals parts of this program are the javascript part...

```javascript
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
```

...and the Server side counterpart handler:

```javascript
//***********************************************************
//* JQUERY REPLIES from Server to WebPage                   *
//*********************************************************** 
void handleJQuery() {

  // sends data as an array, the values are comma separated
  server.send ( 200, "text/plain", String(WiFi.RSSI()) + "," + String(ESP.getVcc()));                                 
}
//*********************************************************** 
```

use & abuse this code as you wont!
