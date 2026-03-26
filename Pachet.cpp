//---------------------------------------------------------------------------

#pragma hdrstop

#include "Pachet.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#include <algorithm>
#include <random>
#include <chrono>
#include <iostream>

Pachet::Pachet() : pozitieCurenta(0) { ConstruiestePachetStandard(); AmestecaPachet(); }

void Pachet::ConstruiestePachetStandard() {
    listaCarti.clear();
    const std::string suite[] = { "Inima Rosie", "Pica", "Romb", "Trefla" };
    const std::string simboluri[] = { "A","2","3","4","5","6","7","8","9","10","J","D","K" };
    for (int s = 0; s < 4; ++s) {
        for (int v = 0; v < 13; ++v) {
            int val = (v == 0) ? 11 : (v < 10 ? v + 1 : (v == 10 ? 12 : (v == 11 ? 13 : 14)));
            listaCarti.emplace_back(val, simboluri[v], suite[s]);
        }
    }
}

void Pachet::AmestecaPachet() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(listaCarti.begin(), listaCarti.end(), std::default_random_engine(seed));
    pozitieCurenta = 0;
}

Carte Pachet::ExtrageCarte() { return (EsteGol()) ? Carte() : listaCarti[pozitieCurenta++]; }
bool Pachet::EsteGol() const { return pozitieCurenta >= listaCarti.size(); }
size_t Pachet::NumarCartiRamase() const { return listaCarti.size() - pozitieCurenta; }
void Pachet::AfiseazaInformatii() const { std::cout << "Ramase: " << NumarCartiRamase() << "\n"; }
