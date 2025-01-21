#include "display_lib.h"       // Ekranla işləmək üçün xüsusi kitabxana
#include <Arduino.h>           // Arduino əsas funksiyalar üçün
#include <Wire.h>              // I2C əlaqəsi üçün
#include <Adafruit_GFX.h>      // Qrafik işləmələr üçün
#include <Adafruit_SSD1306.h>  // OLED ekran idarəedicisi kitabxanası
#include <vector>              // Dinamik massivlər üçün
#include <pgmspace.h>          // Flash yaddaşdan istifadə etmək üçün

// OLED ekranın ölçüləri
#define SCREEN_WIDTH 128     // Ekranın eni
#define SCREEN_HEIGHT 64     // Ekranın hündürlüyü
#define OLED_RESET -1        // Reset pini (-1 istifadə olunmur)
#define SCREEN_ADDRESS 0x3C  // OLED ekranın I2C ünvanı

// SDA və SCL pinlərinin təyini
#define SDA_PIN 10  // SDA pin (data)
#define SCL_PIN 11  // SCL pin (saat)

// Xüsusi I2C interfeysi üçün TwoWire obyektini yaradırıq
TwoWire myWire = TwoWire(0);  // 0 - I2C port nömrəsi

// OLED ekran üçün Adafruit_SSD1306 obyektini yaradırıq
Adafruit_SSD1306 oled_display(SCREEN_WIDTH, SCREEN_HEIGHT, &myWire, OLED_RESET);

// Display sinifinin konstrukturu
Display::Display() {
  Serial.begin(115200);  // Serial əlaqəni başladırıq
  Serial.println("Display obyekt yaradılır...");
  oled_display.clearDisplay();  // Ekranı təmizləyirik
  oled_display.display();
  delay(2000);                  // Kiçik gözləmə
  oled_display.clearDisplay();  // Yenidən təmizləyirik
  Serial.println("Display obyekt yaradıldı.");
}

// OLED ekranı başlatmaq üçün funksiyanı optimizə edirik
bool Display::init() {
  // I2C interfeysini xüsusi SDA və SCL pinləri ilə başlat
  myWire.begin(SDA_PIN, SCL_PIN);

  // OLED ekranını işə salırıq
  if (!oled_display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("OLED ekran işə salınmadı. Yenidən başlatmağa çalışılır...");
    delay(1000);   // Hata mesajı üçün gecikmə
    return false;  // Uğursuzluq halında false qaytarır
  }
  return true;  // Uğurlu başlatma
}

// OLED ekrana mesaj yazdırmaq funksiyası
void Display::printMessage(String message, int sizeText, std::vector<int> cursorPosition, bool clearStatus, const GFXfont *f) {
  // Əgər koordinatlar səhvdirsə, standart koordinatları istifadə edirik
  if (cursorPosition.size() != 2) {
    Serial.println("CursorPosition ölçüsü səhvdir. Standart dəyərlərdən istifadə edilir.");
    cursorPosition = { 0, 0 };  // Standart koordinatlar
  }
  if (clearStatus) {
    oled_display.clearDisplay();  // Ekranı təmizləyirik
  }
  oled_display.setTextSize(sizeText);                            // Mətnin ölçüsü
  oled_display.setFont(f);                                       // Mətnin fontu
  oled_display.setTextColor(SSD1306_WHITE);                      // Mətn rəngi
  oled_display.setCursor(cursorPosition[0], cursorPosition[1]);  // Mətnin mövqeyi
  oled_display.println(message);                                 // Mətnin özü
  oled_display.display();                                        // Ekranı yeniləyirik
}

// OLED ekranda şəkil çəkmək üçün funksiyanı optimizə edirik
void Display::drawBitmap(int x, int y, const uint8_t *bitmap, int w, int h) {
  oled_display.drawBitmap(x, y, bitmap, w, h, SSD1306_WHITE);  // Bitmap çəkirik
  oled_display.display();                                      // Yeniləyirik
}

// OLED ekranı təmizləmək funksiyası
void Display::clear() {
  oled_display.clearDisplay();  // Ekranı tamamilə təmizləyirik
}

// Kontrast səviyyəsini təyin etmək funksiyası
void Display::contrast(int light) {
  oled_display.ssd1306_command(SSD1306_SETCONTRAST);  // Kontrast komandasını göndər
  oled_display.ssd1306_command(light);                // Səviyyəni təyin et (0-255)
}

// Ekranı işıqlandırmaq və ya söndürmək üçün funksiyanı optimizə edirik
void Display::displayLightSwitch(bool status) {
  if (status) {
    oled_display.ssd1306_command(SSD1306_DISPLAYON);  // Ekranı yandır
  } else {
    oled_display.ssd1306_command(SSD1306_DISPLAYOFF);  // Ekranı söndür
  }
}
