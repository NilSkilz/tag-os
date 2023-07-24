
   /*
  Example animated analogue meters using a ILI9341 TFT LCD screen

  Needs Font 2 (also Font 4 if using large scale label)

  Make sure all the display driver and pin comnenctions are correct by
  editting the User_Setup.h file in the TFT_eSPI library folder.

  #########################################################################
  ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
  #########################################################################
*/

#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

const char* ssid = "PidgeonsNest";
const char* password = "3b5794e3e9";

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

#define TFT_GREY 0x5AEB

#define LOOP_PERIOD 100 // Display updates every 35 ms
#define CF_OM12 &Orbitron_Medium_12
#define CF_OM16 &Orbitron_Medium_16
#define CF_OM36 &Orbitron_Medium_36
#define CF_OL24 &Orbitron_Light_24
#define CF_OL32 &Orbitron_Light_32

#define GFXFF 1

int ammo = 70;
int health = 100;
int armour = 100;

float ltx = 0;    // Saved x coord of bottom of needle
uint16_t osx = 120, osy = 120; // Saved x & y coords
uint32_t updateTime = 0;       // time for next update

int old_analog =  -999; // Value last displayed
int old_digital = -999; // Value last displayed

int value[6] = {0, 0, 0, 0, 0, 0};
int old_value[6] = { -1, -1, -1, -1, -1, -1};
int d = 0;

void setup(void) {
  tft.init();
  tft.setRotation(0);
  tft.setFreeFont(CF_OM12); 
  Serial.begin(57600); // For debug
  tft.fillScreen(TFT_BLACK);

//  analogMeter(); // Draw analogue meter

  WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.println("Connection Failed! Rebooting...");
      delay(5000);
      ESP.restart();
    }

    // Hostname defaults to esp3232-[MAC]
  ArduinoOTA.setHostname("Tagger-01");
  
  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  drawLabels();

  updateTime = millis(); // Next update time
}


void loop() {
  ArduinoOTA.handle();
  
  if (updateTime <= millis()) {
    updateTime = millis() + LOOP_PERIOD;

    ammo -= 1; if (ammo <=0) ammo = 70;
    armour -= 2; if (armour <=0) armour = 100;
    health -= 1; if (health <=0) health = 100;

    plotAmmo();
    plotHealth();
    plotArmour();
  }
}

void drawLabels() {
  tft.setFreeFont(CF_OL24);
  tft.setTextColor(TFT_WHITE);  // Text colour
  tft.drawString("/ 70", 130, 140, 1);

  tft.setFreeFont(CF_OM12); 
  tft.setTextColor(TFT_WHITE);  // Text colour
  tft.drawString("Armour", 10, 265, 1);

  tft.setFreeFont(CF_OM12); 
  tft.setTextColor(TFT_WHITE);  // Text colour
  tft.drawString("Health", 10, 290, 1);

  tft.setFreeFont(CF_OL24); 
  tft.setTextColor(TFT_WHITE);  // Text colour
  tft.drawCentreString("SKIRMOS", 120, 20, 1);
}

void plotAmmo() {
  tft.setFreeFont(CF_OM36);
  tft.fillRect(10, 134, 120, 40, TFT_BLACK);
  tft.setTextColor(TFT_WHITE);  // Text colour
  tft.drawRightString(String(ammo), 120, 134, 1);


  // Show Reload label if ammo low
  tft.fillRect(10, 190, 220, 40, TFT_BLACK);
  tft.setTextColor(TFT_WHITE);  // Text colour
  
  if (ammo < 20) {
    tft.setFreeFont(CF_OM16);
    tft.drawCentreString("Reload!", 120, 190, 1);
  }
}

void plotHealth() {
  uint32_t colour = TFT_GREENYELLOW;
  if (health < 20) colour = TFT_RED;

  tft.fillRect(90, 290, 140, 12, TFT_BLACK);
  tft.fillRect(90, 290, 1.4*health, 12, colour);
}

void plotArmour() {
  tft.fillRect(90, 265, 140, 12, TFT_BLACK);
  tft.fillRect(90, 265, 1.4*armour, 12, TFT_CYAN);
}
