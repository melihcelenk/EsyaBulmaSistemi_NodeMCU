#ifndef KALICIHAFIZA_h
#define KALICIHAFIZA_h

#include <Arduino.h>
#include <EEPROM.h>


namespace KALICIHAFIZA{
	void onAnahtarYaz(String sekizKarakterliOnAnahtar);
	void nodeIdYuksekYaz(byte NodeIdHigh);
	void nodeIdDusukYaz(byte NodeIdLow);
	void MACYaz(char MACAddr[6]);
	void pinYaz(String dortHaneliPin);
	void etiketYaz(String etiket20Karakter);
	void kaydet();
	void kurulum(char MACAddr[6]);
	void MACGetir(char* mac);
	void onAnahtarGetir(char* onAnh);
	byte nodeIdYuksekGetir();
	byte nodeIdDusukGetir();
	void pinGetir(char* pin);
}

#endif
