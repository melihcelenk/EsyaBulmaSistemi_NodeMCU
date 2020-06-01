#include  <ESP8266WiFi.h>
#include  <DNSServer.h>
#include  <ESP8266WebServer.h>
#include  <WiFiManager.h>
#include "KALICIHAFIZA.h"
using namespace KALICIHAFIZA;


int sinyalDurum; // Anlık olarak sinyalin verilip verilmeyeceğine karar verir
// delay() kullanımı asenkron çalışmaya imkan vermediğinden zaman ölçülerek sinyal verilir
unsigned long currentMillis;
unsigned long blinkStartMillis;
const unsigned long blinkPeriod = 500;

const int ledPin = 13; // GPIO13 (NodeMCU üzerinde D7)

int value; // Led değeri
ESP8266WebServer server(80);

void setup() {
  
  Serial.begin(115200);
  delay(10);
  pinMode(0, INPUT_PULLUP); // Resetleme butonu aktifleştiriliyor

  sinyalDurum=0; // Başlangıçta sinyal
  
  AgKurulum();
  hafizaIlkKurulum();

  blinkStartMillis = millis();
}

void loop() {
  currentMillis = millis();
  sinyalKontrol();
  sunucuKontrol();
}

void hafizaIlkKurulum(){
      byte macAddressByteArray[6];
      WiFi.macAddress(macAddressByteArray);
      
      char macAddressCharArray[6];
      for(int i = 0; i < 6; i++){
        macAddressCharArray[i] = macAddressByteArray[i];
      }
      kurulum(macAddressCharArray);
}

void sunucuKontrol() {
  server.handleClient();
  value = LOW;
}

void AgKurulum() {
  
  WiFiManager  wifiManager;
  // Bağlanılmış bir ağdan çıkmak için nodeMCU üzerindeki butona tıklanmalı
  int buttonState = digitalRead(0);
   Serial.print("\nbuttonState:");
   Serial.println(buttonState);
  if (buttonState == 0) {
    Serial.println("Agdan cikiliyor...");
    WiFi.disconnect();
  }
  
  delay(500);

  //wifiManager.setSTAStaticIPConfig(IPAddress(192,168,137,153), WiFi.gatewayIP(), WiFi.subnetMask());
  //wifiManager.setSTAStaticIPConfig(IPAddress(192,168,137,153), IPAddress(192,168,137,1), IPAddress(255,255,255,0));

  // Cihaz konfigüre edilmek üzere AP olarak yayın yapıyor
  wifiManager.autoConnect ("KurulumAP");
  Serial.print ("Kablosuz Aga Baglaniliyor: ");

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Bağlanana kadar bekle
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println (WiFi.SSID());
  Serial.println("Kablosuz aga baglanildi");

  // Servera gelecek istekler aşağıda aynı isimle kotarılacaktır
  server.on("/", handleRoot);
  server.on("/text", handleText);
  server.on("/me", handleMe);
  server.on("/konfigurasyon", handleKonfigurasyon);
  server.on("/sinyalGonder", handleSinyalGonder);
  server.on("/LED=ON", handleLedOn);
  server.on("/LED=OFF", handleLedOff);
  server.onNotFound(handleNotFound);
  
  // HTTP server başlatılıyor
  server.begin();
  Serial.println("TCP server baslatildi");

  // IP adresi yazdırılıyor (Seri porta yazdırmak sadece kontrol içindir, IP'nin bulunması işlemi Android cihaz tarafından yapılacaktır)
  Serial.print("Baglanmak icin bu URL'yi kullanin: ");
  Serial.print("http://");
  Serial.println(WiFi.localIP());
  //Serial.print(" Hostname:");
  //Serial.print(WiFi.hostname());
}

// Kök dizin
void handleRoot() {
  IPAddress ip = WiFi.localIP();
  String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]); 
  String s = "";
  s = "\r\n<!DOCTYPE HTML>\r\n<html>Merhaba, ben ESP8266. IP adresim: ";
  s += ipStr;
  s += "</html>\r\n";
  server.send(200, "text/html", s);
  Serial.print("Mesaj:");Serial.println(s);
}
// LED=ON isteği için
void handleLedOn() {
  digitalWrite(ledPin, HIGH);
  value = HIGH;
  
  String s = "";
  s = "\r\n<!DOCTYPE HTML>\r\n<html>Led pin durumu: ";
  if (value == HIGH) {
    s+="Acik";
  } else {
    s+="Kapali";
  }
  
  s += "</html>\r\n";
  server.send(200, "text/html", s);
  Serial.print("Mesaj:");Serial.println(s);
}
// LED=OFF isteği için
void handleLedOff() {
  digitalWrite(ledPin, LOW);
  value = LOW;
  
  String s = "";
  s = "\r\n<!DOCTYPE HTML>\r\n<html>Led pin durumu: ";
  if (value == HIGH) {
    s+="Acik";
  } else {
    s+="Kapali";
  }
  
  s += "</html>\r\n";
  server.send(200, "text/html", s);
  Serial.print("Mesaj:");Serial.println(s);
}
// Cihazın kendini tanıtması (Android tarafında ID'ler eşleşirse bağlantı sağlanacaktır. ID atanması için bkz handleKonfigurasyon)
void handleMe() {
  IPAddress ip = WiFi.localIP();
  String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]); 
  String s = "";
  s = "{\"nodeId\":"+String(nodeIdDusukGetir())+",\"ip\":" + ipStr + ",\"macAddress\":\"" + WiFi.macAddress() + "\"}";
  server.send(200, "application/json", s);
  Serial.print("Mesaj:");Serial.println(s);
}
void handleText() {
  String message = "";
  for (uint8_t i=0; i<server.args(); i++){
    message += server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(200, "text/plain", message);
  Serial.print("Mesaj:");Serial.println(message);
}
// Cihaza Android tarafından NodeId atanıyor
void handleKonfigurasyon() {
  String message = "";
  message += "{";
  for (uint8_t i=0; i<server.args(); i++){
    if (server.argName(i) == "setNodeId"){
      int nodeId = server.arg(i).toInt();
      nodeIdDusukYaz(nodeId);
      Serial.print("Node Id ");Serial.print(nodeId);Serial.println(" olarak atandi.");
    }
    message += "\"" + server.argName(i) + "\"" + ":" + server.arg(i) + "\n";
  }
  message += "}";
  server.send(200, "application/json", message);
  Serial.print("Mesaj:");Serial.println(message);
}

// sinyalGonder?id=X&durum=23 formatında bir http isteği için
void handleSinyalGonder() {
  int id=-1, durum=-1;
  String message = "";
  message += "{";
  for (uint8_t i=0; i<server.args(); i++){
    if (server.argName(i) == "id"){
      id = (int)server.arg(i).toInt();
      Serial.print("Sinyal Istegi, Id:"); Serial.println(id);
    }
    if (server.argName(i) == "durum"){
      durum = (int)server.arg(i).toInt();
      Serial.print("Sinyal Istegi, Durum:"); Serial.println(durum);
    }
    //message += "\"" + server.argName(i) + "\"" + ":" + server.arg(i);
    //if(i<server.args()-1) message+= ",";
  }
  int nodeId = nodeIdDusukGetir();
  // Eğer istemcinin sinyal göndermek istediği ID, bu cihazınkiyle eşleşiyorsa ve durum kodu doğru ise
  if ( id == nodeId && durum==23){
    durum = 200; // 200 ile istemciye cevap verilecek
    sinyalDurum=20; // 20'den geriye doğru sayılarak sinyal verilecek
    Serial.println("Sinyal verildi.");
  }
  else{
    String hataSny;
    if(id!=nodeId && durum==23) {durum = -44; hataSny="NodeId uyumsuz.";}
    if(id==nodeId && durum!=23) {durum = -55; hataSny="Durum uyumsuz.";}
    Serial.println("Sinyal isteği başarısız:"+hataSny);
  }
  
  message += ("\"id\":" + String(id));
  message += ",";
  message += ("\"durum\":" + String(durum));
  message += "}";
  
  server.send(200, "application/json", message);
  Serial.print("Mesaj:");Serial.println(message);
}

// Tanımlı olmayan bir isteğin gelme durumu
void handleNotFound(){
  server.send(404, "text/plain", "404: Not found");
}

// Sinyal zaman her döngüde periyodik olarak azaltılarak sinyal veriliyor
void sinyalKontrol(){
  if(sinyalDurum>1){
    if (currentMillis - blinkStartMillis >= blinkPeriod)
    {
      digitalWrite(ledPin, !digitalRead(ledPin));  // ledin durumunu değiştir
      blinkStartMillis = currentMillis;
      sinyalDurum--;  
    }
  }
  else if (sinyalDurum==1) {digitalWrite(ledPin, LOW); sinyalDurum--;}
}
