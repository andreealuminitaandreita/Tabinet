//---------------------------------------------------------------------------

#pragma hdrstop

#include "InformatiiSuplimentare.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
InformatiiSuplimentare::InformatiiSuplimentare(const std::string& simbolCarte, const std::string& suitaCarte) {
    valoarePuncte = 0;
    esteCarteaCuPuncte = false;

	// Doar 10 de Romb si 2 de Trefla sunt "cãrti cu puncte speciale"
    if (simbolCarte == "10" && suitaCarte == "Romb") {
        esteCarteaCuPuncte = true;  // Carte specialã
        valoarePuncte = 2;
    } else if (simbolCarte == "2" && suitaCarte == "Trefla") {
        esteCarteaCuPuncte = true;  // Carte specialã
        valoarePuncte = 1;
    }
	// Asii, Valetii, Damele, Regii si Zecarii aduc puncte la scor, DAR nu sunt "cãrti cu puncte speciale"
    else if (simbolCarte == "A" || simbolCarte == "J" || simbolCarte == "D" || simbolCarte == "K") {
		esteCarteaCuPuncte = false;  // NU sunt cãrti speciale
        valoarePuncte = 1;           // DAR aduc 1 punct la scor
    } else if (simbolCarte == "10") {
        esteCarteaCuPuncte = false;  // NU sunt cãrti speciale
        valoarePuncte = 1;           // DAR aduc 1 punct la scor
    }
}
bool InformatiiSuplimentare::EsteCuPuncte() const { return esteCarteaCuPuncte; }
int InformatiiSuplimentare::GetValoarePuncte() const { return valoarePuncte; }
