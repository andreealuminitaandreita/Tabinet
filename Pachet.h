//---------------------------------------------------------------------------

#ifndef PachetH
#define PachetH
#include "Carte.h"
#include "EntitateJoc.h"
#include <vector>
#include <cstddef>

class Pachet : public EntitateJoc {
private:
    std::vector<Carte> listaCarti;
    size_t pozitieCurenta;
public:
    Pachet();
    void ConstruiestePachetStandard();
    void AmestecaPachet();
    Carte ExtrageCarte();
    bool EsteGol() const;
    size_t NumarCartiRamase() const;
    void AfiseazaInformatii() const override;
};
#endif
