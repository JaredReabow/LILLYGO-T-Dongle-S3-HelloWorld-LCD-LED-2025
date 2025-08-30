
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <OneButton.h>
#include <FastLED.h>
#include <SPI.h>
#include "pin_config.h"

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS_PIN, TFT_DC_PIN, TFT_RES_PIN);
CRGB leds;
OneButton button(BTN_PIN, true);

// ------------------------------------------------------------------------------------

void button_task(void *param) {
  while (1) {
    button.tick();
    delay(50);
  }
}

void button_pressed() {
  FastLED.showColor(CRGB::Yellow);
  tft.fillScreen(ST77XX_YELLOW);
  tft.setTextColor(ST77XX_BLACK);
  tft.setCursor(20, 40);
  tft.setTextSize(2);
  tft.print("YELLOW");
  Serial.println("Yellow");
}

// ------------------------------------------------------------------------------------

void setup() {
  Serial.begin(115200);
  
  // Initialize SPI
  SPI.begin(TFT_SCL_PIN, -1, TFT_SDA_PIN, TFT_CS_PIN);
  
  // Initialize the display
  tft.initR(INITR_MINI160x80);  // Init ST7735S mini display
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);
  
  // Write initial diagnose to serial port
  Serial.println("");
  Serial.println("Hello, this is T-Dongle-S3.");
  Serial.println("I'm alive and well");
  Serial.println("");

  // Turn off LCD backlight
  pinMode(TFT_LEDA_PIN, OUTPUT);  
  digitalWrite(TFT_LEDA_PIN, 0);  

  // Attach LED
  FastLED.addLeds<APA102, LED_DI_PIN, LED_CI_PIN, BGR>(&leds, 1);  

  // Attach handler to the button
  button.attachClick(button_pressed);
  button.attachLongPressStart(button_pressed);

  // Create a separate task for the button to be checked. Task is pinned to core #0
  // Note that the main loop runs on core #1. Priority of the task is set to 1, which is above the iddle task (0)
  xTaskCreatePinnedToCore(button_task, "button_task", 1024, NULL, 1, NULL, 0);
}

// ------------------------------------------------------------------------------------

void loop() {
  FastLED.showColor(CRGB::Red);  
  tft.fillScreen(ST77XX_RED);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(30, 40);
  tft.setTextSize(2);
  tft.print("RED");
  Serial.println("Red");
  delay(1000);

  FastLED.showColor(CRGB::Green);
  tft.fillScreen(ST77XX_GREEN);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(20, 40);
  tft.setTextSize(2);
  tft.print("GREEN");
  Serial.println("Green");
  delay(1000);
  
  FastLED.showColor(CRGB::Blue);
  tft.fillScreen(ST77XX_BLUE);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(25, 40);
  tft.setTextSize(2);
  tft.print("BLUE");
  Serial.println("Blue");
  delay(1000);
}
