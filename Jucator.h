//---------------------------------------------------------------------------

#ifndef JucatorH
#define JucatorH
#include "EntitateJoc.h"
#include "Carte.h"
#include <vector>
#include <string>

class Jucator : public EntitateJoc {
private:
    std::string numeJucator;
    std::vector<Carte> cartiInMana;
    std::vector<Carte> cartiLuate;
    int punctajTabla;
    int punctajFinal;
    bool aLuatUltimaMana;

public:
    Jucator();
    Jucator(const std::string &nume);

    void SetNume(const std::string &n);
    const std::string& GetNume() const;

    void PrimesteCarte(const Carte &c);
    Carte JoacaCarte(size_t indexCarte);
    size_t NumarCartiInMana() const;
    const std::vector<Carte>& GetCartiInMana() const;

    void AdaugaCartiLuate(const std::vector<Carte> &c);
    void AdaugaInCastig(const Carte &c) { cartiLuate.push_back(c); }

    void AdaugaPunctTabla();
    void RidicaTabla();
    int GetNumarTable() const;

    const std::vector<Carte>& GetCartiLuate() const { return cartiLuate; }
    size_t NumarCartiLuate() const;

    void SeteazaUltimaMana(bool status);
    bool GetUltimaMana() const;

    void SetPunctajFinal(int p) { punctajFinal = p; }
    int GetPunctajFinal() const { return punctajFinal; }
    int CalculeazaPunctajFinal() const;

    void AfiseazaInformatii() const override;
};
#endif
