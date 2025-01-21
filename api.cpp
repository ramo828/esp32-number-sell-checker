#include <WiFi.h>        // Wi-Fi bağlantısı üçün kitabxana
#include <HTTPClient.h>  // HTTP sorğuları üçün kitabxana
#include "api.h"         // API sinifinin başlıq faylı

// Funksiya: URL yaratmaq
// Məqsəd: Göstərilən nömrə və prefiksə əsasən düzgün URL qaytarır
String Api::getNumbers(String number, String prefix) {
  String myApiUrl = String("https://esim.nar.az/api/number-discovery/stock") + "?page=0" + "&number=" + number + "&prefix=" + prefix + "&size=50&providerId=1";
  return myApiUrl;
}

// Funksiya: HTTP başlıqları əlavə etmək
// Məqsəd: Sorğu üçün lazımlı başlıqları əlavə etmək
void Api::addHeaders(HTTPClient &http, ApiMode mode) {
  // Bütün sorğular üçün ümumi başlıq
  http.addHeader("User-Agent", "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/131.0.0.0 Safari/537.36 v2");

  // Mode uyğun başlıqlar əlavə edilir
  switch (mode) {
    case NAR:
      http.addHeader("Accept", "application/json, text/plain, */*");
      http.addHeader("Host", "esim.nar.az");
      http.addHeader("X-API-Key", "f14b1e9f-ddaa-4537-ad81-6bc910927caa");  // Xüsusi API açarı
      http.addHeader("X-API-Version", "1.0.0");
      http.addHeader("Accept-Encoding", "gzip, deflate, br, zstd");
      http.addHeader("Accept-Language", "tr-TR,tr;q=0.9,en-US;q=0.8,en;q=0.7");
      http.addHeader("Connection", "keep-alive");
      http.addHeader("Content-Language", "en");
      http.addHeader("Cookie", "_gcl_au=1.1.1851166897.1730213888; _fbp=fb.1.1730213888652.268995889615495814; _ga=GA1.1.1579418950.1730213889; _hjSessionUser_3133044=eyJpZCI6IjA1MzFkYmMyLTlkYjctNTU4Ni1hYzZkLWNmYjY4N2U3Njg4OCIsImNyZWF0ZWQiOjE3MzAyMTM4ODg1MDEsImV4aXN0aW5nIjp0cnVlfQ==; _hjSession_3133044=eyJpZCI6IjgwZWFmYmQyLTAxNmUtNDMwNi1iZjRkLWQxYjBjNTcyMTQxOSIsImMiOjE3MzAzNjU1MzE3ODMsInMiOjAsInIiOjAsInNiIjowLCJzciI6MCwic2UiOjAsImZzIjowLCJzcCI6MH0=; _gcl_aw=GCL.1730365565.Cj0KCQjw1Yy5BhD-ARIsAI0RbXbyfCrUDtQVmTJC-TGsgnEi81c6maKZQgrwtSQa8K0sAj7TptcKP58aAsbpEALw_wcB; _gcl_gs=2.1.k1$i1730365564$u154879660; _ga_LPT2H142SY=GS1.1.1730365535.3.1.1730365565.0.0.0; _ga_91DQNBWTTX=GS1.1.1730365534.3.1.1730365565.29.0.0");
      http.addHeader("Provider-Id", "1");
      http.addHeader("Referer", "https://esim.nar.az/en?gad_source=1&gclid=Cj0KCQjw1Yy5BhD-ARIsAI0RbXbyfCrUDtQVmTJC-TGsgnEi81c6maKZQgrwtSQa8K0sAj7TptcKP58aAsbpEALw_wcB");
      http.addHeader("Referrer", "1");
      http.addHeader("Sec-CH-UA", "\"Chromium\";v=\"130\", \"Google Chrome\";v=\"130\", \"Not?A_Brand\";v=\"99\"");
      http.addHeader("Sec-CH-UA-Mobile", "?0");
      http.addHeader("Sec-CH-UA-Platform", "\"Linux\"");
      http.addHeader("Sec-Fetch-Dest", "empty");
      http.addHeader("Sec-Fetch-Mode", "cors");
      http.addHeader("Sec-Fetch-Site", "same-origin");
      http.addHeader("X-App-Id", "b6920d9083c8e76685bcc8db34b8c9bb");
      http.addHeader("X-Session-Id", "1ff32023-dec8-49e1-8b39-fac28b0e3224");
      break;
    case STANDART:
      http.addHeader("Accept", "*/*");
      break;
    case BAKCELL:
      // Bakcell üçün xüsusi başlıqlar əlavə etmək mümkündür
      break;
  }
}


// PUT sorğusu göndərən funksiya
// Məqsəd: API'ye PUT sorğusu göndərmək
String Api::sendPutRequest(String apiUrl, String payload, ApiMode mode) {
  HTTPClient http;

  // Wi-Fi bağlantısını kontrol et
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi bağlantısı mövcud deyil. Lütfən Wi-Fi bağlantısını yoxlayın.");
    return "Xəta: Wi-Fi bağlantısı yoxdur";
  }

  // Zaman aşımını ayarla
  http.setTimeout(30000);  // 30 saniyə

  // URL başlatma
  if (!http.begin(apiUrl)) {
    Serial.println("HTTP müştərisi başlatıla bilmədi. URL formatını yoxlayın.");
    return "Xəta: HTTP müştəri başlatma uğursuz oldu";
  }
  addHeaders(http, mode);

  // JSON məzmunu üçün Content-Type əlavə et
  http.addHeader("Content-Type", "application/json");

  // PUT sorğusunu göndər
  int httpResponseCode = http.PUT(payload);
  Serial.print("HTTP Cavab Kodu (PUT): ");
  Serial.println(httpResponseCode);

  // Cavabı işləmə
  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("API Cavabı (PUT):");
    Serial.println(response);
    http.end();
    return response;
  } else {
    Serial.println("Xəta: PUT sorğusu uğursuz oldu.");
    Serial.print("Xəta Kodu: ");
    Serial.println(httpResponseCode);
    http.end();
    return "Xəta: PUT sorğusu uğursuz oldu, cavab kodu: " + String(httpResponseCode);
  }

  http.end();
  return "Xəta: Tanınmayan xəta baş verdi";
}


// API'dən məlumatları çəkmək üçün funksiya
// Məqsəd: GET sorğusu ilə API'dən məlumat çəkmək
String Api::fetchData(String apiUrl, ApiMode mode) {
  HTTPClient http;

  // Wi-Fi bağlantısını kontrol et
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi bağlantısı mövcud deyil. Lütfən Wi-Fi bağlantısını yoxlayın.");
    return "Xəta: Wi-Fi bağlantısı yoxdur";
  }

  // Zaman aşımını ayarla
  http.setTimeout(30000);  // 30 saniyə

  // URL başlatma
  if (!http.begin(apiUrl)) {
    Serial.println("HTTP müştərisi başlatıla bilmədi. URL formatını yoxlayın.");
    return "Xəta: HTTP müştəri başlatma uğursuz oldu";
  }
  addHeaders(http, mode);

  // GET sorğusunu göndər
  int httpResponseCode = http.GET();
  Serial.print("HTTP Cavab Kodu: ");
  Serial.println(httpResponseCode);

  // Cavab kodlarına görə xəta idarəsi
  if (httpResponseCode > 0) {
    if (httpResponseCode == 200) {
      String response = http.getString();
      Serial.println("API Cavabı:");
      // Serial.println(response);
      http.end();
      return response;
    } else if (httpResponseCode == 401) {
      Serial.println("Xəta: Yetkiləndirmə xətası (401). API açarınızı yoxlayın.");
      http.end();
      return "Xəta: İcazəsiz (401)";
    } else if (httpResponseCode == 403) {
      Serial.println("Xəta: Giriş qadağası (403). API girişinizi yoxlayın.");
      http.end();
      return "Xəta: Qadağan olunub (403)";
    } else if (httpResponseCode == 404) {
      Serial.println("Xəta: İstifadə olunan resurs tapılmadı (404). URL'yi yoxlayın.");
      http.end();
      return "Xəta: Tapılmadı (404)";
    } else if (httpResponseCode == 500) {
      Serial.println("Xəta: Server xətası (500). API təminatçısı ilə əlaqə saxlayın.");
      http.end();
      return "Xəta: Daxili Server Xətası (500)";
    } else {
      Serial.println("Xəta: Gözlənilməz bir HTTP cavab kodu alındı.");
      http.end();
      return "Xəta: Gözlənilməz HTTP cavabı (" + String(httpResponseCode) + ")";
    }
  } else {
    // HTTP sorğusu uğursuz olduqda
    switch (httpResponseCode) {
      case HTTPC_ERROR_CONNECTION_REFUSED:
        Serial.println("Xəta: Bağlantı rədd edildi. Server əlçatan olubmu?");
        break;
      case HTTPC_ERROR_SEND_HEADER_FAILED:
        Serial.println("Xəta: HTTP başlıqları göndərilə bilmədi. Başlıq quruluşunu yoxlayın.");
        break;
      case HTTPC_ERROR_SEND_PAYLOAD_FAILED:
        Serial.println("Xəta: HTTP məlumatı göndərilə bilmədi. Payload-u yoxlayın.");
        break;
      case HTTPC_ERROR_NOT_CONNECTED:
        Serial.println("Xəta: Serverə bağlantı qurulmadı. URL və ya Wi-Fi bağlantısını yoxlayın.");
        break;
      case HTTPC_ERROR_CONNECTION_LOST:
        Serial.println("Xəta: Bağlantı zamanı kəsilmə baş verdi. İnternet bağlantınızı yoxlayın.");
        break;
      case HTTPC_ERROR_NO_HTTP_SERVER:
        Serial.println("Xəta: HTTP serveri tapılmadı. URL'yi yoxlayın.");
        break;
      default:
        Serial.println("Xəta: HTTP bağlantısı zamanı bilinməyən bir xəta baş verdi.");
        break;
    }
    http.end();
    return "Xəta: HTTP sorğusu uğursuz oldu, cavab kodu: " + String(httpResponseCode);
  }

  http.end();  // HTTP bağlantısını bağla
  return "Xəta: Tanınmayan xəta baş verdi";
}
