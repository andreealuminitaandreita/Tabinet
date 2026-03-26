//---------------------------------------------------------------------------

#ifndef MesajeReteaH
#define MesajeReteaH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <vector>
#include "Carte.h"

class MesajeRetea {
public:
    static String SerializeazaCarte(const Carte& c);
    static Carte DeserializeazaCarte(const String& str);
    static String CreeazaMesajMutare(int indexCarte);
    static String CreeazaMesajStareJoc(const std::vector<Carte>& masa,
                                        int punctajJ1, int punctajJ2,
                                        int randJucator);
    static String CreeazaMesajDistribuie(const std::vector<Carte>& carti);
};
#endif
