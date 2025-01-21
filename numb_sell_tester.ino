#include <vector>                  // Vektorları istifadə etmək üçün lazımlı kitabxana (dizilərin dinamik idarəsi)
#include "display_lib.h"           // OLED ekranla işləmək üçün kitabxana
#include "image.h"                 // Şəkillərlə işləmək üçün kitabxana (hal-hazırda istifadəsi görünmür)
#include "api.h"                   // API ilə əlaqə qurmaq üçün kitabxana
#include <Fonts/FreeMono12pt7b.h>  // OLED ekranda istifadə edilən yazı şriftinin kitabxanası
#include "custom_fonts.h"          // Xüsusi yazı şriftləri (hal-hazırda istifadəsi görünmür)
#include "extra_lib.h"             // Əlavə funksiyalar üçün kitabxana
#include <Arduino_JSON.h>          // JSON formatını emal etmək üçün kitabxana

// OLED ekranı idarə etmək üçün Display sinifindən bir obyekt yaradılır
Display d = Display();

// API ilə məlumat almaq və göndərmək üçün Api sinifindən bir obyekt yaradılır
Api api = Api();

// Əlavə funksiyalar üçün Extra sinifindən bir obyekt yaradılır
Extra extra = Extra();

// Sayğaclar üçün dəyişənlər (bu dəyişənlər proqramın vəziyyətini izləmək üçün istifadə edilir)
int sell_count = 0;    // Satışda olan nömrələrin sayı
int unsell_count = 0;  // Satılmış nömrələrin sayı
int count_page = 0;    // Hər API çağırışı üçün səhifə sayıcısı
int total_number = 0;  // Cəmi nömrələrin sayı

// API ilə əlaqənin olub-olmamasını izləmək üçün dəyişən
bool is_connected = false;

// API URL
String myApiUrl = "https://pink-baboon-0.telebit.io";

// JSON məlumatını əldə etmək üçün funksiyamız
StaticJsonDocument<6000> fetchJsonData(String url) {
  // API-dən məlumat alınır
  String response = api.fetchData(url, api.BAKCELL);
  // Əgər cavabda "Error" sözü yoxdursa, məlumatı JSON formatında emal edirik
  if (response.indexOf("Error") == -1) {
    return extra.jsonExtract(response);  // JSON-u emal edib geri qaytarırıq
  }
  // Əgər bir səhv varsa, boş bir JSON geri qaytarırıq
  return StaticJsonDocument<6000>();
}

// setup() funksiyası bir dəfə icra olunur və başlanğıc parametrlərini təyin edir
void setup() {
  d.init();            // OLED ekranı işə salır
  d.clear();           // Ekranı təmizləyir
  extra.wifi_setup();  // Wi-Fi bağlantısını qurur
  extra.memoryInfo();  // Yaddaş haqqında məlumatı ekranda göstərir
  sleep(2);            // 2 saniyə gözləyir (Wi-Fi qurulması üçün)
}

// loop() funksiyası sonsuz olaraq təkrar icra olunur (yəni, proqram işləyərkən davamlı olaraq təkrarlanır)
void loop() {
  d.clear();  // OLED ekranını təmizləyir (amma hər dövrədə təmizləməyə ehtiyac olmaya bilər)

  // API-dən məlumatı əldə edirik
  StaticJsonDocument<6000> totalDataDoc = fetchJsonData(myApiUrl + "/total-data?page=" + String(count_page));

  // Əgər məlumat doğru gəlirsə (yəni, API ilə əlaqə uğurludursa)
  if (!totalDataDoc.isNull() && totalDataDoc.containsKey("data")) {
    is_connected = true;  // Bağlantı aktivdir

    // 250 məlumat üçün dövr (loop) yaradılır, hər dəfə API-dən gələn yeni məlumatlar emal edilir
    for (int count = 0; count < 251; count++) {
      // Əgər məlumat "null" (boş) olsa, dövrü dayandırırıq
      if (totalDataDoc["data"][count].isNull()) break;

      // Məlumat JSON formatında emal edilir
      StaticJsonDocument<6000> data = extra.jsonExtract(totalDataDoc["data"][count]);

      // Ekranda səhifə, ümumi say və satışda olan/satılmış nömrələrin sayı göstərilir
      d.printMessage("Page: " + String(count_page), 1, { 0, 0 }, false);
      d.printMessage("Total: " + String(total_number), 1, { 50, 0 }, false);
      d.printMessage("Satildi: " + String(unsell_count), 1, { 10, 17 }, false);
      d.printMessage("Satisda: " + String(sell_count), 1, { 10, 27 }, false);

      // Nömrənin unikal ID-sini alırıq
      int id = data["id"].as<int>();
      // API-dən nömrə məlumatını alırıq
      String msisdn = extra.jsonExtract(api.fetchData(api.getNumbers(data["Number"].as<String>(), data["Prefix"].as<String>()), api.NAR))["data"][0]["msisdn"].as<String>();

      // Əgər nömrə "null" deyilsə, onu ekrana yazdırırıq və satışda olan nömrələrin sayını artırırıq
      if (msisdn != "null") {
        Serial.println(msisdn);                                                                           // Serial monitoruna nömrəni yazdırır
        api.sendPutRequest(myApiUrl + "/update-sale/" + String(id) + "?status=false", {}, api.STANDART);  // API-ya PUT sorğusu göndərir
        d.printMessage(msisdn, 1, { 10, 62 }, true, &FreeMono12pt7b);                                     // Nömrəni ekranda göstərir
        sell_count++;                                                                                     // Satışda olan nömrələrin sayını artırırıq
      } else {                                                                                            // Əgər nömrə "null" olsa, satışda olmadığına qərar veririk
        api.sendPutRequest(myApiUrl + "/update-sale/" + String(id) + "?status=true", {}, api.STANDART);   // API-ya PUT sorğusu göndərir
        d.printMessage("Satildi", 1, { 10, 62 }, true, &FreeMono12pt7b);                                  // Ekranda "Satıldı" mesajını göstərir
        unsell_count++;                                                                                   // Satılmış nömrələrin sayını artırırıq
      }
      total_number++;  // Cəmi nömrələrin sayını artırırıq
    }
  } else {                           // Əgər API ilə əlaqə qurulmadısa
    is_connected = false;            // Bağlantı yoxdur
    if (is_connected) count_page++;  // Bağlantı varsa, səhifəni artırırıq
  }
}
