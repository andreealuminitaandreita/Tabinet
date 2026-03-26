//---------------------------------------------------------------------------

#ifndef CarteH
#define CarteH
#include "EntitateJoc.h"
#include "InformatiiSuplimentare.h"
#include <string>

 class Carte : public EntitateJoc {
private:
    int valoareNumerica;
    std::string simbol;
    std::string suita;
    InformatiiSuplimentare infoPuncte;

public:
    Carte();
    Carte(int valoare, const std::string &simbolCarte, const std::string &suitaCarte);
    int GetValoare() const;
    const std::string& GetSimbol() const;
    const std::string& GetSuita() const;
    bool EsteAs() const;
    bool EsteCarteaCuPuncte() const;
    int GetPuncteCartea() const;
    std::string ConvertesteLaText() const;
    void AfiseazaInformatii() const override;
};
#endif
