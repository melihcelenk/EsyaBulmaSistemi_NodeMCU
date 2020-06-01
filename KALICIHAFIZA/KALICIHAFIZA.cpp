#include "KALICIHAFIZA.h"

// BU KÜTÜPHANE EEPROM'UN [0-39] VE [269-511] ARALIĞINDAKİ BYTE'LARINI KULLANMAKTADIR.
// ASCII DIŞI KARAKTER KULLANILMAMALIDIR. ASCII DIŞI KARAKTERLERLE KULLANMAK İÇİN İSTEDİĞİNİZ KARAKTERLERİ ASCII'DEKİ KULLANILMAYAN KARAKTERLERLE DEĞİŞTİRİNİZ.

namespace KALICIHAFIZA{
	// Doğru cihazla iletişimin başlangıcı için kontrol amaçlı bir anahtar
	void onAnahtarYaz(String sekizKarakterliOnAnahtar){
      char onAnahtar[8];
      if (sekizKarakterliOnAnahtar.length()!=8){
        
        Serial.println("Lütfen 8 karakterli (ASCII) bir metin giriniz.");
        Serial.println(sekizKarakterliOnAnahtar.length());
        onAnahtar[0] = 69;
        onAnahtar[1] = 108;
        onAnahtar[2] = 97;
        onAnahtar[3] = 122;
        onAnahtar[4] = 105;
        onAnahtar[5] = 287;
        onAnahtar[6] = 50;
        onAnahtar[7] = 51;
      }
      else{
        for (int i = 0; i < 8 ; i++) { 
          onAnahtar[i] = sekizKarakterliOnAnahtar[i]; 
        }
      }
      EEPROM.begin(512);
      EEPROM.put(0, onAnahtar);
      kaydet();
	}
	// EEPROM'a yazım byte byte olduğu için yüksek ve düşük byte'lar ayrı ayrı alınıyor
	 void nodeIdYuksekYaz(byte NodeIdHigh){
		EEPROM.begin(512);
		EEPROM.write(8, NodeIdHigh);
		kaydet();
	 }
	  void nodeIdDusukYaz(byte NodeIdLow){
		EEPROM.begin(512);
		EEPROM.write(9, NodeIdLow);
		kaydet();
	  }
	void MACYaz(char MACAddr[6]){
		char mac[6];
		for (int i = 0; i < 6; i++) { 
		mac[i] = MACAddr[i];
		}

		EEPROM.begin(512);
		EEPROM.put(10, mac);
		kaydet();
	}
	void pinYaz(String dortHaneliPin){
		EEPROM.begin(512);
		char pinKodu[4] = { 'E','L','2','3'};
		if (dortHaneliPin.length()!=4){
		Serial.print("Lütfen 4 karakterli (ASCII) bir pin giriniz. Pin kodu EL23 olarak tanımlandı.");
		}
		else{
		for (int i = 0; i < 4 ; i++) { 
		  pinKodu[i] = dortHaneliPin[i]; 
		}
		}
		EEPROM.put(16, pinKodu);
		kaydet();
	}
  void etiketYaz(String etiket20Karakter){
      EEPROM.begin(512);
      char etiket[20] = "OTURMA ODASI";
      if (etiket20Karakter.length()>20){
        Serial.print("Lütfen en fazla 20 (ASCII) karakterli bir metin giriniz. Etiket OTURMA ODASI olarak tanımlandı."); 
      }
      else{
        for (int i = 0; i < 20 ; i++) {
          etiket[i] = etiket20Karakter[i];
        }
      }
      EEPROM.put(20, etiket);
      kaydet();
  }
	void kaydet(){
		if (EEPROM.commit()) {
		Serial.println("EEPROM successfully committed");
		} else {
		Serial.println("ERROR! EEPROM commit failed");
		}
	}
	void kurulum(char MACAddr[6]){
		onAnahtarYaz("elazig23");
		Serial.print("\nmac:");
		Serial.println((int)*MACAddr);
		for (int i = 0; i < 6; i++) { 
			Serial.print("mac[");
			Serial.print(i);
			Serial.print("]:");
			Serial.println((int)MACAddr[i]);
		}
		MACYaz(MACAddr);
	}
	void MACGetir(char* mac){
		EEPROM.begin(512);
		for (int address = 10; address < 16; address++){
			byte value = EEPROM.read(address);
			mac[address-10] = value;
		}
	}
	void onAnahtarGetir(char* onAnh){
		EEPROM.begin(512);
		for (int address = 0; address < 8; address++){
			byte value = EEPROM.read(address);
			onAnh[address] = value;
		}
	}
	byte nodeIdYuksekGetir(){
		EEPROM.begin(512);
		byte value = EEPROM.read(8);
		return value;
	}
	byte nodeIdDusukGetir(){
		EEPROM.begin(512);
		byte value = EEPROM.read(9);
	}
	void pinGetir(char* pin){
		EEPROM.begin(512);
		for (int address = 16; address < 20; address++){
			byte value = EEPROM.read(address);
			pin[address-16] = value;
		}
	}



}
