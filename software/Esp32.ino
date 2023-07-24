/*

  _   _ ___ _     ____  _  _____ _     _____
 | \ | |_ _| |   / ___|| |/ /_ _| |   |__  /
 |  \| || || |   \___ \| ' / | || |     / / 
 | |\  || || |___ ___) | . \ | || |___ / /_ 
 |_| \_|___|_____|____/|_|\_\___|_____/____|
                                            
  
  Skirmos Laser Tag system

  - to create Sound files use `xxd -i ak47_clipin.wav ak47_clipin.h`
*/

#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ezButton.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

const uint16_t kIrLed = 14;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).

IRsend irsend(kIrLed);  // Set the GPIO to be used to sending the message.

#include "TFT_Setup.h"

#include "Fonts/Orbitron_Medium_12.h"
#include "Fonts/Orbitron_Medium_16.h"
#include "Fonts/Orbitron_Medium_36.h"

#include "Sounds/ak47-1.h"
#include "Sounds/ak47_clipout.h"
#include "Sounds/ak47_clipin.h"
#include "Sounds/ak47_boltpull.h"
#include "XT_DAC_Audio.h"

// Audio Class
XT_DAC_Audio_Class DacAudio(25,0);


// ******************
// Gun SFX
// ******************

XT_Wav_Class Ak47(ak47_1_wav); 
XT_Wav_Class Ak47_clipout(ak47_clipout_wav); 
XT_Wav_Class Ak47_clipin(ak47_clipin_wav); 
XT_Wav_Class Ak47_boltpull(ak47_boltpull_wav); 
                                      

// ******************
// Wifi Settings
// ******************
const char* ssid = "PidgeonsNest";
const char* password = "3b5794e3e9";


// ******************
// TFT Settings
// ******************
  
TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

#define TFT_GREY 0x5AEB
#define GFXFF 1
#define LOOP_PERIOD 35 // Display updates every 35 ms

/*
  TO GENERATE ADDITIONAL FONTS GO TO https://oleddisplay.squix.ch/
*/
#define CF_OL24 &Orbitron_Light_24
#define CF_OL32 &Orbitron_Light_32
#define CF_OM12 &Orbitron_Medium_12
#define CF_OM16 &Orbitron_Medium_16
#define CF_OM36 &Orbitron_Medium_36

uint32_t updateTime = 0;       // time for next update



// ******************
// Buttons & Trigger
// ******************

ezButton trigger(16);





// ******************
// Game Variables
// ******************

int ammo = 10;
int health = 100;
int armour = 100;

enum ReloadState {
  none,
  start,
  clipout,
  clipin,
  bolt,
  finished
};

bool ammoNeedsUpdate = false;


// Keep the state of reloading
ReloadState reloadState = none;

int nextTick = 0;

// ******************
// Program Setup
// ******************

void setupTft() {
  Serial.println("Setting up TFT");
  tft.init();
  tft.setRotation(0);
  tft.setFreeFont(CF_OM12); 
  Serial.begin(57600); // For debug
  tft.fillScreen(TFT_BLACK);
}

void setupWifi() {
  Serial.println("Setting up Wifi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setupOTA() {
  Serial.println("Setting up OTA");
  ArduinoOTA.setHostname("Tagger-01");
  
  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
      digitalWrite(25, LOW);
      plotOtaUpdate(progress);
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
}

void setup(void) {

  Serial.println("Running Setup");

  setupTft();
  setupWifi();
  setupOTA();
  drawLabels();
  
  trigger.setDebounceTime(50);

  updateTime = millis(); // Next update time

  plotAmmo();
  plotHealth();
  plotArmour();

//irsend.begin();

  Serial.println("Setup Finished");
  Serial.println(TFT_MISO); // 19
  Serial.println(TFT_MOSI); // 23
  Serial.println(TFT_SCLK); // 18
  Serial.println(TFT_CS);   // 5  
  Serial.println(TFT_DC);   // 21 
  Serial.println(TFT_RST);  // 17

//  pinMode(14, OUTPUT);
  
}


// ******************
// Main Loop
// ******************

void loop() {

  ArduinoOTA.handle();

  trigger.loop();
 
  DacAudio.FillBuffer();

  if(trigger.isPressed()) {
    Serial.println("Trigger");
    if (reloadState == none) {
      fireWeapon();
    }
  }

  if (updateTime <= millis()) {
    updateTime = millis() + LOOP_PERIOD;  
    if (ammoNeedsUpdate) {
      plotAmmo();
      plotHealth();
      plotArmour();
    }
  }

//  digitalWrite(14, HIGH);

  if (reloadState != none) {
    switch(reloadState) {
      case start:
        if (millis() > nextTick)
          ejectClip();
        break;
      case clipout: 
        if (millis() > nextTick)
          loadClip();
        break;
      case clipin: 
        if (millis() > nextTick)
          cockGun();
        break;
    }
  }
}


// ******************
// Game Functions
// ******************

  void fireWeapon() {
  Serial.println("Fire");
//  irsend.sendNEC(0x00FFE01FUL);
  DacAudio.Play(&Ak47);
  ammo -= 1;
  ammoNeedsUpdate = true;

  if (ammo <= 0 && reloadState == none) {
    reloadState = start;
    nextTick = millis() + 2000;
  }
}

void ejectClip() {
  DacAudio.Play(&Ak47_clipout); 
  nextTick = millis() + 1000;
  reloadState = clipout;
}

void loadClip() {
  DacAudio.Play(&Ak47_clipin); 
  nextTick = millis() + 1000;
  reloadState = clipin;
}

void cockGun() {
  DacAudio.Play(&Ak47_boltpull); 
  nextTick = 0;
  reloadState = none;
  ammo = 10;
  ammoNeedsUpdate = true;
}


// ******************
// TFT Drawing
// ******************

void drawLabels() {
  tft.setFreeFont(CF_OL24);
  tft.setTextColor(TFT_WHITE);  // Text colour
  tft.drawString("/ 10", 130, 140, 1);

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

  if (ammo <= 0) {
    tft.setFreeFont(CF_OM16);
    tft.drawCentreString("Reloading...", 120, 190, 1);
  } else if (ammo < 4) {
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

void plotOtaUpdate(unsigned int progress) {
  tft.fillRect(90, 265, 140, 12, TFT_BLACK);
  tft.setFreeFont(CF_OM16);
  tft.drawCentreString("Updating...", 120, 190, 1);
}
