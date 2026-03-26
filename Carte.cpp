//---------------------------------------------------------------------------

#pragma hdrstop

#include "Carte.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#include <iostream>


Carte::Carte() : valoareNumerica(0), simbol(""), suita(""), infoPuncte("", "") {}

Carte::Carte(int valoare, const std::string &simbolCarte, const std::string &suitaCarte)
	: valoareNumerica(valoare), simbol(simbolCarte), suita(suitaCarte), infoPuncte(simbolCarte, suitaCarte) {}

int Carte::GetValoare() const { return valoareNumerica; }
const std::string& Carte::GetSimbol() const { return simbol; }
const std::string& Carte::GetSuita() const { return suita; }
bool Carte::EsteAs() const { return simbol == "A"; }
bool Carte::EsteCarteaCuPuncte() const { return infoPuncte.EsteCuPuncte(); }
int Carte::GetPuncteCartea() const { return infoPuncte.GetValoarePuncte(); }
std::string Carte::ConvertesteLaText() const { return simbol + " de " + suita; }
void Carte::AfiseazaInformatii() const { std::cout << ConvertesteLaText() << "\n"; }
