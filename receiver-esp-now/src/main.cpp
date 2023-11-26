#include <SPI.h>
#include <Wire.h>
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <esp_now.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
#define I2C_SDA 21
#define I2C_SCL 22

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len);

TwoWire i2cWire(0);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &i2cWire, OLED_RESET);


void setup() {
  Serial.begin(115200);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setTextWrap(true);
  display.setCursor(0, 0);
  display.println("Setting up device...");
  display.display();
  delay(500);
  WiFi.mode(WIFI_MODE_STA);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("MAC Addr:");
  display.setTextSize(1);
  display.println(WiFi.macAddress());
  display.display();

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  display.clearDisplay();
  display.setCursor(2, 1);
  display.println("Received data:");
  display.println((char*)incomingData);
  display.display();
}
