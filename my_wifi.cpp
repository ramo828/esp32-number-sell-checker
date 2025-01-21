#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <Arduino.h>
#include "my_wifi.h"

// Wi-Fi şəbəkə adı (SSID) və parol üçün göstəricilər
char *ssid = "";
char *password = "";

/**
 * @brief MyWifi sinfinin konstrukturu.
 * Wi-Fi şəbəkə məlumatlarını təyin edir və seriya portunu başlatır.
 * 
 * @param SSID Wi-Fi şəbəkə adı (SSID).
 * @param PASSWORD Wi-Fi şəbəkə parolu.
 */
MyWifi::MyWifi(char *SSID, char *PASSWORD) {
  ssid = SSID;
  password = PASSWORD;
  Serial.begin(115200);  // Seriya portu başlat
}

/**
 * @brief Wi-Fi bağlantısını başlatmaq üçün istifadə olunur.
 * 
 * @return true Əgər bağlantı uğurlu olarsa.
 * @return false Əgər bağlantı zamanı aşımı baş verərsə.
 */
bool MyWifi::init() {
  WiFi.begin(ssid, password);  // Wi-Fi şəbəkəsinə qoşulmağa başlayır

  // Qoşulma üçün maksimum gözləmə müddəti (20 saniyə)
  const unsigned long connectionTimeout = 20000;
  unsigned long startAttemptTime = millis();

  // Qoşulma prosesini yoxlayan dövr
  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - startAttemptTime >= connectionTimeout) {
      Serial.println("Wi-Fi bağlantısı zaman aşımına uğradı!");
      return false;  // Zaman aşımı baş verdikdə false döndər
    }
    delay(1000);  // Qoşulma cəhdləri arasında 1 saniyə gözləyir
    Serial.println("Wi-Fi şəbəkəsinə qoşulmağa çalışılır...");
  }

  Serial.println("Wi-Fi şəbəkəsinə uğurla qoşuldu!");
  return true;
}
