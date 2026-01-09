#include <Arduino.h>
#include <FastLED.h>
#include <Matter.h>
#include <WiFi.h>
#include <Preferences.h>

MatterColorLight ColorLight;
Preferences matterPref;
const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;
const char *onOffPrefKey = "OnOff";
const char *hsvColorPrefKey = "HSV";

#define BRIGHTNESS  100
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];

void updatePhysicalLeds() {
  if (ColorLight.getOnOff()) {
    espHsvColor_t matterHsv = ColorLight.getColorHSV();
    fill_solid(leds, NUM_LEDS, CHSV(matterHsv.h, matterHsv.s, matterHsv.v));
    FastLED.setBrightness(BRIGHTNESS); 
  } else {
    fill_solid(leds, NUM_LEDS, CRGB::Black);
  }
  FastLED.show();
}

bool setLightState(bool state, espHsvColor_t colorHSV) {
  updatePhysicalLeds();
  matterPref.putBool(onOffPrefKey, state);
  matterPref.putUInt(hsvColorPrefKey, colorHSV.h << 16 | colorHSV.s << 8 | colorHSV.v);
  return true;
}

void setup() {
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 2000);
  Serial.begin(115200);
  delay(2000);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\r\nWiFi connected");

  matterPref.begin("MatterPrefs", false);
  bool lastOnOffState = matterPref.getBool(onOffPrefKey, true);
  uint32_t prefHsvColor = matterPref.getUInt(hsvColorPrefKey, 169 << 16 | 254 << 8 | 254);
  espHsvColor_t lastHsvColor = {uint8_t(prefHsvColor >> 16), uint8_t(prefHsvColor >> 8), uint8_t(prefHsvColor)};
  
  ColorLight.begin(lastOnOffState, lastHsvColor);
  ColorLight.onChange(setLightState);

  ColorLight.onChangeOnOff([](bool state) {
    updatePhysicalLeds();
    return true;
  });

  ColorLight.onChangeColorHSV([](HsvColor_t hsvColor) {
    updatePhysicalLeds();
    return true;
  });

  Matter.begin();
  
  if (Matter.isDeviceCommissioned()) {
     updatePhysicalLeds();
  }
}

void loop() {
  if (!Matter.isDeviceCommissioned()) {
    Serial.printf("Manual pairing code: %s\r\n", Matter.getManualPairingCode().c_str());
    Serial.printf("QR code URL: %s\r\n", Matter.getOnboardingQRCodeUrl().c_str());
    
    while (!Matter.isDeviceCommissioned()) {
      delay(5000);
      Serial.println("Waiting for commissioning...");
    }
    
    ColorLight.updateAccessory();
    updatePhysicalLeds();
  }
}