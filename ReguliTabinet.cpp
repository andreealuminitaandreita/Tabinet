//---------------------------------------------------------------------------

#pragma hdrstop

#include "ReguliTabinet.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#include <algorithm>
#include <iostream>
#include <vector>
#include <map>

// Structură auxiliară pentru backtracking, reține indexul original și valoarea flexibilă
struct CarteFlexibila {
	int indexOriginal;
	int valoare;
	int indexInListaValori; // Indexul său unic în lista de valori (pentru As)
};


static bool backtrackSubset(const std::vector<CarteFlexibila> &valori, int idx, int target,
							std::vector<int> &rez, int& indexAsFolosit)
{
	if (target == 0)
	{
		return true;
	}
	if (idx >= (int)valori.size() || target < 0) return false;

	// 1. Încearcă să includă elementul idx
	CarteFlexibila curent = valori[idx];

	// Verificăm dacă As-ul de pe indexul curent a fost deja folosit (A=1 sau A=11)
	if (indexAsFolosit == curent.indexOriginal && curent.valoare == 1) {
		// Dacă As-ul a fost deja folosit ca 11, nu-l putem folosi din nou ca 1. Ignorăm.
        return backtrackSubset(valori, idx + 1, target, rez, indexAsFolosit);
	}

	// Dacă valoarea curentă este 1 (As), o marcăm ca folosită pe indexul său original
    int tempAsFolosit = indexAsFolosit;
	if (curent.valoare == 1 && curent.indexOriginal != indexAsFolosit) {
        tempAsFolosit = curent.indexOriginal;
	}

	// Se continuă căutarea în restul elementelor (idx+1) cu target redus
    if (backtrackSubset(valori, idx + 1, target - curent.valoare, rez, tempAsFolosit))
	{
		// Dacă s-a găsit o soluție, adăugăm indexul original în vectorul de rezultate
        rez.push_back(curent.indexOriginal);
		indexAsFolosit = tempAsFolosit;
        return true;
	}

    // 2. Încearcă să excludă elementul idx
    return backtrackSubset(valori, idx + 1, target, rez, indexAsFolosit);
}


bool ReguliTabinet::GasesteCombinatiiSuma(const std::vector<Carte> &cartiPeMasa, int target, std::vector<int> &rezIndices)
{
    std::vector<CarteFlexibila> valoriFlexibile;

    for (size_t i = 0; i < cartiPeMasa.size(); ++i)
	{
		const auto &c = cartiPeMasa[i];


		if (!c.EsteAs()) {
			valoriFlexibile.push_back({(int)i, c.GetValoare(), (int)i});
        }
		// Cărțile As (pot fi 1 sau 11)
		else {
            valoriFlexibile.push_back({(int)i, 11, (int)i});
			valoriFlexibile.push_back({(int)i, 1, (int)i});
        }
	}

	//
    std::sort(valoriFlexibile.begin(), valoriFlexibile.end(),
			  [](const CarteFlexibila& a, const CarteFlexibila& b) {
                  return a.valoare > b.valoare;
              });

	std::vector<int> rez;
    int indexAsFolosit = -1;

	//
	if (backtrackSubset(valoriFlexibile, 0, target, rez, indexAsFolosit))
	{

        std::sort(rez.begin(), rez.end());
		rez.erase(std::unique(rez.begin(), rez.end()), rez.end());

        rezIndices = rez;
		return true;
	}

	return false;
}

bool ReguliTabinet::PoateCaptura(const Carte &carteJucata, const std::vector<Carte> &cartiPeMasa)
{
    std::vector<int> dummy;
	return GasesteCombinatiiSuma(cartiPeMasa, carteJucata.GetValoare(), dummy);
}

int ReguliTabinet::CalculeazaPuncte(const std::vector<Jucator> &listaJucatori)
{

	return 0;
}