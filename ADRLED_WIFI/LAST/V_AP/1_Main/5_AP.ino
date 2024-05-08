
const char* PARAM_INPUT_1 = "input1";

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>WIFI</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <p>Usage: [Name],[Password]</p>
  <p></p>
  <form action="/get">
    <input type="text" name="input1">
    <input type="submit" value="Submit">
  </form><br>
</body></html>)rawliteral";

void APactivate() {
    tone(BUZZER_PIN, 1000, 100);
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssidAP, passwordAP);
    IPAddress IP = WiFi.softAPIP();
    Serial.println(WiFi.softAPIP());

    serverAP.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", index_html);
    });

    serverAP.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
        String inputRAM;
        String inputSSID;
        String inputPASS;

        if (request->hasParam(PARAM_INPUT_1)) {
            inputRAM = request->getParam(PARAM_INPUT_1)->value();

            inputSSID = CutSSID(inputRAM);
            inputPASS = CutPASS(inputRAM);

            EepromPutNew(inputSSID, inputPASS);
            request->send(200, "text/html", "<h1>OK</h1>");
            serverAP.end();
            WiFi.softAPdisconnect(true);
            delay(500);
            wificonnect();
            return;
        }
    });
    serverAP.begin();
}

String CutPASS(String passNew) {
    passNew.remove(0, passNew.indexOf(",")+1);
    return passNew;
}

String CutSSID(String ssidNew) {
    ssidNew.remove(ssidNew.indexOf(","), ssidNew.length());
    return ssidNew;
}

void EepromPutNew(String inSSID, String inPASS) {
    EEPROM.put(100, inSSID.length());
    EEPROM.put(104, inPASS.length());

    Serial.println(inSSID);
    Serial.println(inPASS);
    Serial.println();
    Serial.println();

    for (int i=0;i<inSSID.length();i++) {
        EEPROM.put(110+i, inSSID[i]);
        Serial.print(inSSID[i]);
    }

    for (int i=0;i<inPASS.length();i++) {
        EEPROM.put(150+i, inPASS[i]);
        Serial.print(inPASS[i]);
    }

    Serial.println();
    EEPROM.commit();
}
