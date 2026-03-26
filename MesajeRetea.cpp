//---------------------------------------------------------------------------

#pragma hdrstop

#include "MesajeRetea.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

String MesajeRetea::SerializeazaCarte(const Carte& c) {
    return IntToStr(c.GetValoare()) + "|" +
           String(c.GetSimbol().c_str()) + "|" +
           String(c.GetSuita().c_str());
}

Carte MesajeRetea::DeserializeazaCarte(const String& str) {
    TStringList* parts = new TStringList();
    parts->Delimiter = '|';
    parts->StrictDelimiter = true;
    parts->DelimitedText = str;

    if (parts->Count == 3) {
        int val = StrToInt(parts->Strings[0]);
        std::string simbol = AnsiString(parts->Strings[1]).c_str();
        std::string suita = AnsiString(parts->Strings[2]).c_str();

        delete parts;
        return Carte(val, simbol, suita);
    }

    delete parts;
    return Carte();
}

String MesajeRetea::CreeazaMesajMutare(int indexCarte) {
    return "MUTARE:" + IntToStr(indexCarte);
}

String MesajeRetea::CreeazaMesajStareJoc(const std::vector<Carte>& masa,
                                         int punctajJ1, int punctajJ2,
                                         int randJucator) {
    String mesaj = "STARE:";
    mesaj += IntToStr(punctajJ1) + ";";
    mesaj += IntToStr(punctajJ2) + ";";

    for (size_t i = 0; i < masa.size(); i++) {
        if (i > 0) mesaj += ",";
        mesaj += SerializeazaCarte(masa[i]);
    }

    mesaj += ";" + IntToStr(randJucator);

    return mesaj;
}

String MesajeRetea::CreeazaMesajDistribuie(const std::vector<Carte>& carti) {
    String mesaj = "CARTI:";

    for (size_t i = 0; i < carti.size(); i++) {
        if (i > 0) mesaj += ",";
        mesaj += SerializeazaCarte(carti[i]);
    }

    return mesaj;
}
