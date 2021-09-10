# Ses Tanıma İle Eşya Bulma Sistemi

NodeMCU kartlar için tasarlanmıştır. Android uygulamasıyla birlikte kullanılır. (Önerilen minimum NodeMCU cihaz adedi: 3)

KALICIHAFIZA klasörü Arduino/libraries dizinine eklenmelidir.

Detaylı bilgi ve Android tarafı için: https://github.com/melihcelenk/EsyaBulmaSistemi_Android

## Kodun Kullanımı

- Arduino IDE yükleyin.
- ESP8266	WiFi	modülüne sahip kartları (bu projede NodeMCU kullanılmıştır) tanımlı kartlar arasına eklemek için Arduino IDE üzerinde
Dosya	>	Tercihler sekmesindeki	ekranda	“Ek	Devre	Kartları Yöneticisi	URLleri”	bölümüne	aşağıda	verilen	linki	ekleyin.
http://arduino.esp8266.com/stable/package_esp8266com_index.json
- Ardunio	IDE’de	Araçlar	>	Kart >	Kart	Yöneticisi	ekranından	"ESP8266" aramasını yapıp kurun.
- Wi-Fi modülünün hotspot olarak kullanılmasını ve ağa bir ara yüz ile bağlanılmasını sağlamak için 
Taslak >	library ekle > libraryleri	düzenle üzerinden "WiFiManager" araması yaparak tzapu tarafından geliştirilen versiyon 0.14.0'ı kurun.
- KALICIHAFIZA kütüphanesini bilgisayarınızdaki Arduino/libraries dizinine ekleyin.
- Kodu derleyip USB ile bağladığınız NodeMCU kartına Arduino IDE üzerinden yükleyin
- Cihaz çalıştığında cihazın oluşturduğu ağa bağlanarak kendi ağınızın SSID ve parolasını girin, cihaz ağa bağlanacaktır
- Bütün NodeMCU cihazlarını ağa bağladıktan sonra Android cihazınızı da ağa bağlayın ve Android uygulaması üzerinden cihazların kurulumunu yapın


