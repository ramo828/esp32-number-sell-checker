#ifndef EXTRA_LIB
#define EXTRA_LIB

// Müxtəlif kitabxanaları daxil edirik
#include "my_wifi.h"      // Wi-Fi ilə əlaqə qurulması üçün kitabxana
#include "display_lib.h"  // LCD ekranda məlumat göstərmək üçün kitabxana
#include <ArduinoJson.h>  // JSON məlumatlarını emal etmək üçün kitabxana
#include <Arduino.h>      // Arduino-nun əsas funksionallığı

// Extra sinifi tərtib edirik
class Extra {
public:
  // Konstruktor, sinifin yaradılması zamanı işə düşəcək
  Extra();

  // Wi-Fi bağlantısını qurmaq üçün funksiya
  void wifi_setup();

  // BUZZER ilə səs çıxarmaq üçün funksiya
  void beep();

  // RAM, PSRAM və flaş yaddaş məlumatlarını çap edən funksiya
  void memoryInfo();

  // JSON məlumatlarını deserialization edərək geri döndərən funksiya
  StaticJsonDocument<6000> jsonExtract(String data, int limit_data = 9);

private:
  // Display (LCD) obyektini yaratmaq
  Display display = Display();

  // MyWifi obyektini yaratmaq (Wi-Fi əlaqəsi üçün)
  MyWifi wifi = MyWifi();

// BUZZER pininin təyini (pin 7)
#define BUZZER 7
};

#endif
