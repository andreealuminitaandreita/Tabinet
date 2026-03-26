//---------------------------------------------------------------------------

#pragma hdrstop

#include "Jucator.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#include <iostream>
#include <numeric>

Jucator::Jucator() : numeJucator("Anonim"), punctajTabla(0), aLuatUltimaMana(false) {}
Jucator::Jucator(const std::string &nume) : numeJucator(nume), punctajTabla(0), aLuatUltimaMana(false) {}

void Jucator::SetNume(const std::string &n) { numeJucator = n; }
const std::string& Jucator::GetNume() const { return numeJucator; }
void Jucator::PrimesteCarte(const Carte &c) { cartiInMana.push_back(c); }
size_t Jucator::NumarCartiInMana() const { return cartiInMana.size(); }
const std::vector<Carte>& Jucator::GetCartiInMana() const { return cartiInMana; }
void Jucator::AdaugaCartiLuate(const std::vector<Carte> &c) { cartiLuate.insert(cartiLuate.end(), c.begin(), c.end()); }
void Jucator::AdaugaPunctTabla() { punctajTabla++; }
void Jucator::RidicaTabla() { punctajTabla--; }
int Jucator::GetNumarTable() const { return punctajTabla; }
size_t Jucator::NumarCartiLuate() const { return cartiLuate.size(); }
void Jucator::SeteazaUltimaMana(bool status) { aLuatUltimaMana = status; }
bool Jucator::GetUltimaMana() const { return aLuatUltimaMana; }

Carte Jucator::JoacaCarte(size_t indexCarte) {
    if (indexCarte >= cartiInMana.size()) return Carte();
    Carte aleasa = cartiInMana[indexCarte];
    cartiInMana.erase(cartiInMana.begin() + indexCarte);
    return aleasa;
}

int Jucator::CalculeazaPunctajFinal() const {
    int totalPuncte = punctajTabla;
    for (const auto &c : cartiLuate) totalPuncte += c.GetPuncteCartea();
    return totalPuncte;
}

void Jucator::AfiseazaInformatii() const {
    std::cout << "Jucator: " << numeJucator << " | Table: " << punctajTabla << "\n";
}
