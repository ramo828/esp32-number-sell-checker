#ifndef MY_WIFI_H
#define MY_WIFI_H

/**
 * @brief MyWifi sinfi.
 * Wi-Fi bağlantısı üçün istifadə olunur.
 */
class MyWifi {
public:
  /**
   * @brief MyWifi sinfinin konstrukturu.
   * Default olaraq SSID və parol təyin edilmişdir.
   * 
   * @param SSID Wi-Fi şəbəkə adı (default olaraq "HUAWEI-2.4G-j9u6").
   * @param PASSWORD Wi-Fi şəbəkə parolu (default olaraq "20402040").
   */
  MyWifi(char *SSID = "HUAWEI-2.4G-j9u6", char *PASSWORD = "20402040");

  /**
   * @brief Wi-Fi bağlantısını başlatmaq üçün istifadə olunur.
   * 
   * @return true Əgər bağlantı uğurlu olarsa.
   * @return false Əgər bağlantı zamanı aşımı baş verərsə.
   */
  bool init();

private:
  // Əgər əlavə özəl dəyişənlər və ya funksiyalar tələb olunarsa, burada təyin edilə bilər.
};

#endif  // MY_WIFI_H
