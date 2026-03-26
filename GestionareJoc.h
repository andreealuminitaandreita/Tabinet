//---------------------------------------------------------------------------

#ifndef GestionareJocH
#define GestionareJocH
#include "EntitateJoc.h"
#include "Pachet.h"
#include "Jucator.h"
#include "MasaDeJoc.h"
#include "ReguliTabinet.h"
#include <vector>
#include <string>

class GestionareJoc : public EntitateJoc
{
private:
    Pachet pachetPrincipal;
    std::vector<Jucator> listaJucatori;
    MasaDeJoc masaDeJoc;
    ReguliTabinet reguli;
    int indexJucatorCurent;

    int GetValoareTabinet(Carte c);
    void DistribuieCartiRundaNoua(int numarCartiPerJucator, size_t indexJucator);
    void VerificaSiImparteCartiNoi();
    void VerificaFinalJoc();

public:
    GestionareJoc();
    void AfiseazaInformatii() const override;
    void AdaugaJucator(const std::string &nume);
    void InitializareJoc();
    bool ContineDouaCartiCuPuncte(const std::vector<Carte> &carti) const;

    std::vector<Carte> DistribuieCartiInitialPrimeiRunde();
    bool FinalizeazaDistributiaPrimeiRunde(bool acceptaJ1, const std::vector<Carte> &cartiJ1);

    void ExecutaMutare(int indexJucator, int indexCarteMana);
    void ExecutaRunda();

    MasaDeJoc* GetMasa();
    Pachet* GetPachet();
    const std::vector<Jucator>& GetListaJucatori() const;
    int GetIndexJucatorCurent() const;
};
#endif
