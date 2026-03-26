//---------------------------------------------------------------------------

#ifndef InformatiiSuplimentareH
#define InformatiiSuplimentareH
#include <string>
//---------------------------------------------------------------------------
class InformatiiSuplimentare {
private:
    bool esteCarteaCuPuncte;
    int valoarePuncte;

public:
    InformatiiSuplimentare(const std::string& simbolCarte, const std::string& suitaCarte);
    bool EsteCuPuncte() const;
    int GetValoarePuncte() const;
};
#endif
