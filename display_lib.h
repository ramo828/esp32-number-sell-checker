#ifndef DISPLAY_LIB_HPP
#define DISPLAY_LIB_HPP

#include <Arduino.h>       // String sinfi və əsas Arduino funksiyaları üçün
#include <vector>          // Dinamik massiv (std::vector) istifadəsi üçün
#include <Adafruit_GFX.h>  // Qrafik funksiyalar üçün

/**
 * @class Display
 * OLED ekranın idarə olunması üçün bir sinifdir.
 * Ekranda mətn yazmaq, şəkil çəkmək və ekran xüsusiyyətlərini idarə etmək funksiyalarını təqdim edir.
 */
class Display {
public:
  /**
   * @brief Display sinfinin konstrukturu.
   * Sinifin obyektini yaradarkən ilkin tənzimləmələr həyata keçirilir.
   */
  Display();

  /**
   * @brief OLED ekranında mətn göstərmək üçün istifadə olunur.
   * @param message Yazılacaq mətn.
   * @param sizeText Mətnin ölçüsü (standart olaraq 2).
   * @param cursorPosition Mətnin ekranda başlanğıc mövqeyi (standart olaraq {0, 0}).
   * @param clearStatus Əvvəlki ekran görüntüsünün təmizlənməsi (standart olaraq true).
   * @param f Mətn fontu (standart olaraq NULL).
   */
  void printMessage(String message, int sizeText = 2, std::vector<int> cursorPosition = { 0, 0 }, bool clearStatus = true, const GFXfont *f = NULL);

  /**
   * @brief OLED ekranını başlatmaq üçün istifadə olunur.
   * @return Uğurlu başlatma üçün true, əks halda false qaytarır.
   */
  bool init();

  /**
   * @brief OLED ekranını tamamilə təmizləyir.
   */
  void clear();

  /**
   * @brief OLED ekranın kontrast səviyyəsini təyin edir.
   * @param light Kontrast səviyyəsi (0-255 arası).
   */
  void contrast(int light);

  /**
   * @brief Ekranı işıqlandırmaq və ya söndürmək üçün istifadə olunur.
   * @param status true - ekranı yandır, false - ekranı söndür.
   */
  void displayLightSwitch(bool status);

  /**
   * @brief Bitmap şəkli ekranda göstərmək üçün istifadə olunur.
   * @param x Şəkilin başlanğıc x mövqeyi.
   * @param y Şəkilin başlanğıc y mövqeyi.
   * @param bitmap Şəkilin məlumatları.
   * @param w Şəkilin eni.
   * @param h Şəkilin hündürlüyü.
   */
  void drawBitmap(int x, int y, const uint8_t *bitmap, int w, int h);

private:
  // Gələcəkdə lazım ola biləcək daxili dəyişənlər burada saxlanıla bilər
};

#endif  // DISPLAY_LIB_HPP
