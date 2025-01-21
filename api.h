#ifndef API_H
#define API_H
#include <HTTPClient.h>

class Api {
public:
  // HTTP modlarını təmsil edən enum (sınıf daxilində istifadə olunacaq müxtəlif rejimlər)
  enum ApiMode {
    STANDART,  // Standart başlıqlar üçün rejim
    NAR,       // NAR API başlıqları üçün rejim (NAR üçün xüsusi başlıqlar)
    BAKCELL    // Gələcəkdə Bakcell API başlıqları əlavə edilə bilər (hələlik boşdur)
  };

  // fetchData: API'dən məlumat çəkmək üçün funksiya
  // apiUrl: sorğunun göndəriləcəyi URL
  // mode: istifadə ediləcək API rejimi (STANDART, NAR, BAKCELL)
  String fetchData(String apiUrl, ApiMode mode);

  // getNumbers: İstifadəçinin müəyyən bir nömrə və prefiks üçün URL yaratmaq üçün funksiyası
  // number: istifadəçinin daxil etdiyi nömrə
  // prefix: nömrənin prefiksi
  // geri qaytarır: düzgün URL stringi
  String getNumbers(String number, String prefix);

  // sendPutRequest: PUT sorğusu göndərmək üçün funksiyası
  // apiUrl: PUT sorğusunun göndəriləcəyi URL
  // payload: PUT sorğusunda göndəriləcək məlumat (JSON formatında)
  // mode: istifadə ediləcək API rejimi (STANDART, NAR, BAKCELL)
  // geri qaytarır: PUT sorğusunun nəticəsi
  String sendPutRequest(String apiUrl, String payload, ApiMode mode);

private:
  // addHeaders: HTTPClient obyektinə lazımlı başlıqları əlavə etmək üçün funksiya
  // http: HTTPClient obyektini qəbul edir
  // mode: istifadə ediləcək API rejimi (STANDART, NAR, BAKCELL)
  void addHeaders(HTTPClient& http, ApiMode mode);
};

#endif
