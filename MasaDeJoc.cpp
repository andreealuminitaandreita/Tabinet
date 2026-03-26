//---------------------------------------------------------------------------

#pragma hdrstop

#include "MasaDeJoc.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#include <iostream>

MasaDeJoc::MasaDeJoc() : cartiPeMasa() {}
void MasaDeJoc::AdaugaCartePeMasa(const Carte &c) { cartiPeMasa.push_back(c); }
void MasaDeJoc::StergeCartile() { cartiPeMasa.clear(); }
const std::vector<Carte>& MasaDeJoc::GetCartiPeMasa() const { return cartiPeMasa; }
size_t MasaDeJoc::NumarCartiPeMasa() const { return cartiPeMasa.size(); }
void MasaDeJoc::AfiseazaInformatii() const {
    std::cout << "Carti pe masa: " << NumarCartiPeMasa() << "\n";
}
std::vector<Carte>& MasaDeJoc::GetCartiPeMasaReferinta() {
    return cartiPeMasa;
}
