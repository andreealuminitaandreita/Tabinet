//---------------------------------------------------------------------------

#pragma hdrstop

#include "GestionareJoc.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#include <iostream>
#include <algorithm>
#include <numeric>
#include <stdexcept>


GestionareJoc::GestionareJoc() : indexJucatorCurent(0) {}

void GestionareJoc::AfiseazaInformatii() const {
    pachetPrincipal.AfiseazaInformatii();
    masaDeJoc.AfiseazaInformatii();
	for (const auto &j : listaJucatori) j.AfiseazaInformatii();
}

void GestionareJoc::AdaugaJucator(const std::string &nume) {
    listaJucatori.emplace_back(nume);
}

void GestionareJoc::InitializareJoc() {
    pachetPrincipal.ConstruiestePachetStandard();
    pachetPrincipal.AmestecaPachet();
    masaDeJoc.StergeCartile();
    for (auto &j : listaJucatori) j = Jucator(j.GetNume());
    indexJucatorCurent = 0;
}

int GestionareJoc::GetValoareTabinet(Carte c) {
    int v = c.GetValoare();
    if (v == 1) return 11;
    return v;
}

bool GestionareJoc::ContineDouaCartiCuPuncte(const std::vector<Carte> &carti) const {
    int puncte = 0;
    for (const auto &c : carti) if (c.EsteCarteaCuPuncte()) puncte++;
    return puncte >= 2;
}

std::vector<Carte> GestionareJoc::DistribuieCartiInitialPrimeiRunde() {
    std::vector<Carte> initial;
    for (int i = 0; i < 4; i++) if (!pachetPrincipal.EsteGol()) initial.push_back(pachetPrincipal.ExtrageCarte());
    return initial;
}

bool GestionareJoc::FinalizeazaDistributiaPrimeiRunde(bool acceptaJ1, const std::vector<Carte> &cartiJ1) {
    if (listaJucatori.size() < 2) return false;
    Jucator &j1 = listaJucatori[0];
    Jucator &j2 = listaJucatori[1];

    if (acceptaJ1 || ContineDouaCartiCuPuncte(cartiJ1)) {
        for (const auto &c : cartiJ1) j1.PrimesteCarte(c);
        DistribuieCartiRundaNoua(2, 0);
        std::vector<Carte> cartiJ2 = DistribuieCartiInitialPrimeiRunde();
        if (ContineDouaCartiCuPuncte(cartiJ2)) {
            for (const auto &c : cartiJ2) j2.PrimesteCarte(c);
            DistribuieCartiRundaNoua(2, 1);
            for (int i = 0; i < 4; i++) masaDeJoc.AdaugaCartePeMasa(pachetPrincipal.ExtrageCarte());
        } else {
            for (const auto &c : cartiJ2) masaDeJoc.AdaugaCartePeMasa(c);
            DistribuieCartiRundaNoua(6, 1);
        }
    } else {
        for (const auto &c : cartiJ1) masaDeJoc.AdaugaCartePeMasa(c);
        DistribuieCartiRundaNoua(6, 0);
        DistribuieCartiRundaNoua(6, 1);
    }

    // Serverul (jucãtorul 0) începe primul
    indexJucatorCurent = 0;
    return true;
}

void GestionareJoc::ExecutaMutare(int idxJ, int idxC) {
    Jucator &j = listaJucatori[idxJ];
    Carte cJucata = j.JoacaCarte(idxC);
    std::vector<Carte>& masa = masaDeJoc.GetCartiPeMasaReferinta();

    // Asul jucat poate fi 11 sau 1
    std::vector<int> tinte = { GetValoareTabinet(cJucata) };
    if (cJucata.GetValoare() == 1) tinte.push_back(1);

    bool aLuat = false;
    for (int t : tinte) {
        bool gasit;
        do {
            gasit = false;
            std::vector<int> deSters;

            if (reguli.GasesteCombinatiiSuma(masa, t, deSters)) {
                gasit = aLuat = true;
                std::sort(deSters.rbegin(), deSters.rend());
                for (int i : deSters) {
                    j.AdaugaInCastig(masa[i]);
                    masa.erase(masa.begin() + i);
                }
            }
        } while (gasit);
    }

    if (aLuat) {
        j.AdaugaInCastig(cJucata);
        for (size_t i = 0; i < listaJucatori.size(); i++)
            listaJucatori[i].SeteazaUltimaMana(i == idxJ);
        if (masa.empty()) j.AdaugaPunctTabla();
    } else {
        masaDeJoc.AdaugaCartePeMasa(cJucata);
        j.SeteazaUltimaMana(false);
    }


    indexJucatorCurent = (indexJucatorCurent + 1) % listaJucatori.size();

    VerificaSiImparteCartiNoi();
    VerificaFinalJoc();
}

void GestionareJoc::VerificaSiImparteCartiNoi() {
    bool goale = true;
    for (const auto& j : listaJucatori) if (j.NumarCartiInMana() > 0) goale = false;
    if (goale && !pachetPrincipal.EsteGol())
        for (size_t i = 0; i < listaJucatori.size(); i++) DistribuieCartiRundaNoua(6, i);
}

void GestionareJoc::VerificaFinalJoc() {
    if (pachetPrincipal.EsteGol()) {
        bool goale = true;
        for (auto& j : listaJucatori) if (j.NumarCartiInMana() > 0) goale = false;
        if (goale) {
            auto& m = masaDeJoc.GetCartiPeMasaReferinta();
            for (auto& j : listaJucatori)
                if (j.GetUltimaMana()) { for(auto& c : m) j.AdaugaInCastig(c); break; }
            m.clear();
            for (size_t i = 0; i < listaJucatori.size(); i++) {
                listaJucatori[i].SetPunctajFinal(listaJucatori[i].CalculeazaPunctajFinal());
            }
        }
    }
}

void GestionareJoc::DistribuieCartiRundaNoua(int n, size_t idx) {
    for (int i = 0; i < n; i++) if (!pachetPrincipal.EsteGol()) listaJucatori[idx].PrimesteCarte(pachetPrincipal.ExtrageCarte());
}

MasaDeJoc* GestionareJoc::GetMasa() { return &masaDeJoc; }
Pachet* GestionareJoc::GetPachet() { return &pachetPrincipal; }
const std::vector<Jucator>& GestionareJoc::GetListaJucatori() const { return listaJucatori; }
int GestionareJoc::GetIndexJucatorCurent() const { return indexJucatorCurent; }
