#include "extra_lib.h"
#include <Arduino.h>
#include <ArduinoJson.h>

Extra::Extra() {
  // BUZZER pinini çıxış olaraq təyin edirik
  pinMode(BUZZER, OUTPUT);
}

void Extra::wifi_setup() {
  // Wi-Fi bağlantısı qurularkən istifadəçi məlumatı göstərmək üçün hazırlıq
  char dot = '.';                      // Wi-Fi axtarılması üçün nöqtə simvolu
  String message = "Wi-Fi axtarılır";  // Göstəriləcək mesaj
  String temp = "";                    // Nöqtələrin əlavə ediləcəyi boş string
  int counter = 0;                     // Nöqtələrin sayını izləyəcək sayğac

  while (true) {
    // Wi-Fi əlaqəsi qurulmadıqda, 3 dəfə cəhd edilir
    if (!wifi.init()) {
      if (counter == 3) {
        temp = "";  // Nöqtə sayğacını sıfırlayırıq
        counter = 0;
      }
      // Wi-Fi axtarılır və hər 3 cəhddən sonra nöqtə əlavə edilir
      temp += String(dot);
      display.printMessage(message + temp, 1, { 20, 30 }, true);  // LCD ekranda mesajı göstəririk
      counter++;
    } else {
      break;  // Wi-Fi bağlantısı qurularsa dövrü dayandırırıq
    }
  }

  // Wi-Fi bağlantısı qurulduqdan sonra mesaj göstəririk
  display.printMessage("Wi-Fi bağlandı", 1, { 20, 20 }, true);
}

void Extra::beep() {
  // BUZZER üzərindən ton ifa edilir
  tone(BUZZER, 1000);  // 1000 Hz frekansında ton
  delay(100);          // 100 millisecond gözləyir
  noTone(BUZZER);      // Ton dayandırılır
}

StaticJsonDocument<6000> Extra::jsonExtract(String data, int limit_data) {
  StaticJsonDocument<6000> doc;  // 6000 byte-lıq JSON sənədi üçün yer ayırırıq

  // Əgər verilən məlumatın uzunluğu limitdən çoxdursa, JSON deserialization həyata keçiririk
  if (data.length() > limit_data) {
    DeserializationError error = deserializeJson(doc, data);  // JSON-u parse edirik

    // Hər hansı bir xəta baş verərsə, xətanı serial monitorunda göstəririk
    if (error) {
      Serial.println(error.f_str());
      // Xəta baş verərsə boş bir StaticJsonDocument qaytarırıq
      return StaticJsonDocument<6000>();
    }

    // JSON deserialization uğurlu oldusa, JSON sənədini geri qaytarırıq
    return doc;
  }

  // Məlumat uzunluğu limitdən kiçikdirsə, boş bir JSON sənədi qaytarırıq
  return StaticJsonDocument<6000>();
}

void Extra::memoryInfo() {
  // RAM haqqında məlumatı çap edirik
  Serial.println("=== RAM Məlumatları ===");
  Serial.print("Ümumi heap: ");
  Serial.println(ESP.getHeapSize());  // Ümumi RAM ölçüsünü çap edirik
  Serial.print("İstifadə olunan heap: ");
  Serial.println(ESP.getHeapSize() - ESP.getFreeHeap());  // İstifadə olunan RAM miqdarı
  Serial.print("Boş heap: ");
  Serial.println(ESP.getFreeHeap());  // Boş RAM miqdarı

  // PSRAM (ESP32-də varsa) haqqında məlumat
  if (ESP.getPsramSize() > 0) {
    Serial.println("\n=== PSRAM Məlumatları ===");
    Serial.print("Ümumi PSRAM: ");
    Serial.println(ESP.getPsramSize());  // Ümumi PSRAM ölçüsü
    Serial.print("İstifadə olunan PSRAM: ");
    Serial.println(ESP.getPsramSize() - ESP.getFreePsram());  // İstifadə olunan PSRAM
    Serial.print("Boş PSRAM: ");
    Serial.println(ESP.getFreePsram());  // Boş PSRAM
  }

  // Flaş yaddaş haqqında məlumat
  Serial.println("\n=== Flaş Yaddaş Məlumatları ===");
  Serial.print("Flaş ölçüsü: ");
  Serial.println(ESP.getFlashChipSize());  // Ümumi flaş yaddaş ölçüsü
  Serial.print("İstifadə oluna bilən flaş: ");
  Serial.println(ESP.getFreeSketchSpace());  // Boş flaş yaddaş sahəsi
  Serial.print("Yüklənmiş sketch ölçüsü: ");
  Serial.println(ESP.getSketchSize());  // Yüklü proqramın ölçüsü
}
