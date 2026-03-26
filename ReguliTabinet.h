//---------------------------------------------------------------------------

#ifndef ReguliTabinetH
#define ReguliTabinetH
#include "Carte.h"
#include <vector>
#include "Jucator.h"

class ReguliTabinet
{
public:

	bool PoateCaptura(const Carte &carteJucata, const std::vector<Carte> &cartiPeMasa);


	bool GasesteCombinatiiSuma(const std::vector<Carte> &cartiPeMasa, int target, std::vector<int> &rezIndices);


	int CalculeazaPuncte(const std::vector<Jucator> &listaJucatori);
};
#endif
