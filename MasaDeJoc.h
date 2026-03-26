//---------------------------------------------------------------------------

#ifndef MasaDeJocH
#define MasaDeJocH
#include "EntitateJoc.h"
#include "Carte.h"
#include <vector>

class MasaDeJoc : public EntitateJoc {
private:
    std::vector<Carte> cartiPeMasa;
public:
    MasaDeJoc();
    void AdaugaCartePeMasa(const Carte &c);
    void StergeCartile();
    const std::vector<Carte>& GetCartiPeMasa() const;
    size_t NumarCartiPeMasa() const;
	void AfiseazaInformatii() const override;
	std::vector<Carte>& GetCartiPeMasaReferinta();
};
#endif
