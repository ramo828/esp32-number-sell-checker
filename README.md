# **Arduino Wi-Fi və JSON İnterfeysi Proyekti**

Bu layihə, ESP32 əsaslı Arduino cihazlarında Wi-Fi bağlantısını qurmaq, JSON məlumatlarını emal etmək, cihazın yaddaş istifadəsini monitorinq etmək və OLED ekranda məlumatları göstərmək üçün nəzərdə tutulmuşdur. Eyni zamanda, cihaz özəl bir API vasitəsilə nömrə məlumatlarını alır və bu məlumatların satış vəziyyətini yoxlayır. Bu layihə, **SSD1306** LCD ekranı və **ESP32 daxili Wi-Fi** kitabxanasından istifadə edir. 

## **Proyekti Quraşdırma və İstifadə**

### **Tələblər**
- **Arduino IDE** (ESP32 board dəstəyi ilə)
- **ESP32 mikroçipi**
- **ArduinoJson Kitabxanası**
- **SSD1306 LCD ekran** (OLED ekran)
- **ESP32 daxili Wi-Fi kitabxanası**

### **Quraşdırma**
1. **Arduino IDE yükləyin**: Əgər hələ yükləməmisinizsə, [Arduino IDE](https://www.arduino.cc/en/software) proqramını yükləyin.
2. **ESP32 Board Dəstəyini Yükləyin**:
   - Arduino IDE-nin `File` > `Preferences` bölməsindən "Additional Board Manager URLs" hissəsinə aşağıdakı URL-ni əlavə edin:
     ```
     https://dl.espressif.com/dl/package_esp32_index.json
     ```
   - Sonra `Tools` > `Board` > `Boards Manager`-a gedin və "ESP32" yazaraq board dəstəyini yükləyin.
3. **Kitabxanaları Yükləyin**:
   - **ArduinoJson** kitabxanasını Arduino Library Manager-dan yükləyin.
   - **SSD1306** LCD ekran kitabxanasını da Library Manager-dan yükləyin (OLED ekran üçün).
   
### **İstifadə**
1. **Wi-Fi Bağlantısı**:
   - `wifi_setup()` funksiyası Wi-Fi əlaqəsini qurur. Cihaz Wi-Fi şəbəkəsini tapmağa çalışacaq və taparsa "Wi-Fi bağlandı" mesajını göstərəcəkdir.
   
2. **Səsli Bildirişlər**:
   - `beep()` funksiyası BUZZER pinini istifadə edərək, cihazı 1000 Hz tezliklə 100 ms müddətinə səsli xəbərdar edir.
   
3. **JSON Məlumatlarının Emalı**:
   - `jsonExtract()` funksiyası, bir JSON məlumatı stringini deserialization edərək `StaticJsonDocument` formatına çevirir. Əgər məlumat ölçüsü verilən limitdən böyükdürsə, bu JSON obyektini geri qaytarır.

4. **Yaddaş Məlumatlarının Çapı**:
   - `memoryInfo()` funksiyası cihazın RAM, PSRAM və flaş yaddaş istifadəsini göstərir. Bu məlumat serial monitorunda çap olunur.

## **Kod Açıqlamaları**

### **Extra Sinifi**
`Extra` sinifi, müxtəlif əlavə funksionallıqları idarə etmək üçün yaradılmışdır. Bu sinifin məqsədi cihazın yaddaş vəziyyətini göstərmək, Wi-Fi bağlantısını idarə etmək və səsli bildirişlər verməkdir.

#### **Sinifdəki Funksiyalar**:
1. **`Extra()`**: Sinifin konstruktoru, cihazın BUZZER pinini çıxış olaraq təyin edir.
2. **`wifi_setup()`**: Wi-Fi bağlantısını qurmaq üçün bu funksiya işə düşür. Əgər Wi-Fi şəbəkəsi tapılarsa, cihaz "Wi-Fi bağlandı" mesajını göstərir.
3. **`beep()`**: BUZZER pinini istifadə edərək cihazı səsli xəbərdar edir. Bu, 1000 Hz tezlikdə 100 ms müddətinə çalınan bir tondur.
4. **`memoryInfo()`**: Cihazın RAM, PSRAM və flaş yaddaş məlumatlarını monitorinq edir və bu məlumatları serial monitorunda göstərir.
5. **`jsonExtract(String data, int limit_data=9)`**: JSON məlumatlarını deserialization edir və onu bir `StaticJsonDocument` obyektinə çevirir. Bu obyekt, daha sonra JSON məlumatlarını oxumaq üçün istifadə oluna bilər.

### **Kitabxanalar və Daxili İstifadə Edilən Funksiyalar**

1. **`ESP32 daxili Wi-Fi kitabxanası`**:
   - Bu layihə, Wi-Fi əlaqəsi qurmaq üçün **ESP32 mikroçipinin daxili Wi-Fi modulundan** istifadə edir. `WiFi` kitabxanası vasitəsilə Wi-Fi şəbəkəsinə qoşulmaq və əlaqəni yoxlamaq mümkündür.

2. **`SSD1306 LCD ekranı`**:
   - **SSD1306** OLED ekranı vasitəsilə cihazın interfeysini vizual olaraq təqdim edirik. `Display` sinifi istifadəçi məlumatlarını ekranda göstərmək üçün istifadə olunur. Bu LCD ekranın əsas xüsusiyyəti kiçik ölçüsü və yüksək kontrastıdır, bu da onu ESP32 kimi layihələr üçün ideal edir.

## **API İnteqrasiyası**
Bu layihə, **özəl bir API** istifadə edir. API, **nömrə məlumatlarını** **https://pink-baboon-0.telebit.io/data** URL-dən alır. API vasitəsilə aşağıdakı əməliyyatlar yerinə yetirilir:

1. **Nömrə Məlumatlarının Alınması**: Layihə, API-dən nömrə məlumatlarını alır.
2. **Operator Məlumatlarının Yoxlanması**: Nömrənin hansı operatora aid olduğu və bu nömrənin satışda olub olmadığı yoxlanılır. 
   - Bu məlumatları **NAR API**-sindən əldə edir.
3. **Satılma Vəziyyətinin Yenilənməsi**: 
   - Əgər nömrə **satılıbsa**, bu məlumat **satisda** olaraq API üzərindən yenilənir.
   - Əgər nömrə **satılmayıbsa**, bu məlumat **satilmadi** olaraq yenilənir.
   
Bütün bu məlumatlar, cihazın ekranında vizual olaraq göstərilir. Bu xüsusiyyət, istifadəçiyə nömrənin vəziyyətini anında görmək imkanı verir.

## **Proyektin Əlavə Xüsusiyyətləri**
- **Wi-Fi Bağlantısının Monitorinqi**: Wi-Fi şəbəkəsinin tapılması və qoşulması prosesində istifadəçiyə hər addımda məlumat verilir. Hər 3 cəhddən sonra, cihaz şəbəkəni yenidən axtarmağa başlayır.
- **Yaddaş İstifadəsinin Monitorinqi**: Cihazın yaddaş istifadəsi barədə ətraflı məlumat verilir. RAM və PSRAM istifadəsi ilə yanaşı, flaş yaddaşın istifadəsi də göstərilir.
- **İnteraktiv Bildirişlər**: BUZZER vasitəsilə istifadəçiyə səsli bildirişlər verilərək, sistemin işləmə vəziyyəti bildirilir.

## **Əlavə Qeydlər**
- **Dəstək və Əlavə Təkmilləşdirmələr**: Bu layihə inkişaf etdirilməyə açıqdır. Yeni funksiyalar əlavə etmək, məsələn, digər Wi-Fi şəbəkələrini dəstəkləyən metodlar, daha optimallaşdırılmış yaddaş idarəetməsi və daha çoxunu əlavə etmək mümkündür.
- **Testlər və Problemlər**: Bu layihə ESP32 mikroçipi ilə işləyir və bəzi funksiyalar yalnız ESP32-də mövcud olan xüsusiyyətləri istifadə edir.

## **Lisenziya**
Bu layihə **MIT Lisenziyası** altında yayımlanır. Əlavə məlumat üçün `LICENSE` faylını yoxlaya bilərsiniz.
# esp32-number-sell-checker
# esp32-number-sell-checker
