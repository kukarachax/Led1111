String recv_Pass, recv_Ssid;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html> 
  <head> 
    <title>KX-Home Configure</title> 
    <meta name="viewport" content="width=device-width, initial-scale=1"> 
  </head> 
  <body> 
    <h1>Please enter the ssid and password for your WiFi network</h1> 
    <form action="/get" method="POST"> 
      WiFi SSID: <input type="text" name="ssid"> 
      <br> 
      WiFi Password: <input type="password" name="password"> 
      <br> 
      <input type="submit" value="Submit"> 
    </form> 
  </body> 
</html>)rawliteral";

const char SubmitPage_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html> 
  <head> 
    <title>KX-Home Configure</title> 
    <meta name="viewport" content="width=device-width, initial-scale=1"> 
  </head> 
  <body>
    <h1>OK, I'm sending data to the ESP-32</h1> 
    <h2>Wait for the ESP-32 to reboot and connect to WiFi</h2> 
  </body> 
</html>)rawliteral";


void setupCaptivePortalServer(){
  serverAP.onNotFound([](AsyncWebServerRequest *request){
     request->send_P(200, "text/html", index_html); 
  });
  serverAP.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html); 
  });
  serverAP.on("/get", HTTP_POST, [](AsyncWebServerRequest *request){
    if (request->hasParam("ssid", true)) {
      recv_Ssid = request->getParam("ssid", true)->value();
    }
    if (request->hasParam("password", true)) {
      recv_Pass = request->getParam("password", true)->value();
    }
    Serial.println("RECV SSID, PASS");
    Serial.println(recv_Ssid);
    Serial.println(recv_Pass);

    updEEPROM_WiFi(recv_Ssid, recv_Pass);

    request->send(200, "text/html", SubmitPage_html);

    tone(BUZZER_PIN, 2400, 700);
    delay(1000);
    
    ESP.restart();
  });

  serverAP.begin();
}

void WiFiConnectAP() {
  WiFi.softAPdisconnect();
  WiFi.disconnect();

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssidAP, passwordAP);

  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.setTTL(300);
  dnsServer.start(53, "*", WiFi.softAPIP());

  setupCaptivePortalServer();

  fillAll(CRGB::Blue);
}

void networkAPLoop() {
  dnsServer.processNextRequest();
}