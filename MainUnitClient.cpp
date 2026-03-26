//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MainUnitClient.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormClient *FormClient;
//---------------------------------------------------------------------------
__fastcall TFormClient::TFormClient(TComponent* Owner) : TForm(Owner)
{
    conectat = false;
    punctajMeu = 0;
    punctajAdversar = 0;

    memLog->Width = 400;
    memLog->Height = 120;
    memLog->Font->Size = 8;

    TimerCitire->Interval = 100;
    TimerCitire->Enabled = false;

    memLog->Lines->Add("CLIENT TABINET");
    memLog->Lines->Add("");
    memLog->Lines->Add("Apasa 'Conecteaza' pentru a te alatura");
    memLog->Lines->Add("jocului de pe server.");
    memLog->Lines->Add("");
}
//---------------------------------------------------------------------------
void __fastcall TFormClient::btnConecteazaClick(TObject *Sender)
{
    String adresaIP;
    if (InputQuery("Conectare la Server", "Introdu adresa IP a serverului:", adresaIP)) {
        if (adresaIP.Length() == 0) {
            ShowMessage("Adresa IP nu poate fi goala!");
            return;
        }

        try {
            IdTCPClient1->Host = adresaIP;
            IdTCPClient1->Port = 2000;
            IdTCPClient1->Connect();

            conectat = true;
            TimerCitire->Enabled = true;
            btnConecteaza->Enabled = false;

            memLog->Lines->Add("CONECTAT LA SERVER!");
            memLog->Lines->Add("Adresa: " + adresaIP + ":2000");
            memLog->Lines->Add("");
            memLog->Lines->Add("Asteapta ca serverul sa inceapa jocul...");
            memLog->Lines->Add("");
        } catch (Exception &e) {
            ShowMessage("EROARE conectare: " + e.Message);
            memLog->Lines->Add("EROARE: " + e.Message);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormClient::TimerCitireTimer(TObject *Sender)
{
    if (!conectat || !IdTCPClient1->Connected()) {
        if (conectat) {
            TimerCitire->Enabled = false;
            conectat = false;
            btnConecteaza->Enabled = true;
            memLog->Lines->Add("DECONECTAT DE LA SERVER!");
        }
        return;
    }

    try {
        IdTCPClient1->IOHandler->CheckForDataOnSource(10);

        if (!IdTCPClient1->IOHandler->InputBufferIsEmpty()) {
            String mesaj = IdTCPClient1->IOHandler->ReadLn();

            if (!mesaj.IsEmpty()) {
                memLog->Lines->Add("Primit de la server: " + mesaj.SubString(1, 20) + "...");
                ProceseazaMesajDelaServer(mesaj);
            }
        }
    } catch (Exception &e) {
        // Daca apare eroare de conexiune, deconecteaza
        TimerCitire->Enabled = false;
        conectat = false;
        btnConecteaza->Enabled = true;
        memLog->Lines->Add("EROARE: Conexiune pierduta!");
	}
}
//---------------------------------------------------------------------------
void TFormClient::ProceseazaMesajDelaServer(const String& mesaj)
{
  // Verifica daca serverul s-a inchis
    if (mesaj.Pos("SERVER_INCHIS") == 1) {
        TimerCitire->Enabled = false;
        conectat = false;
        ShowMessage("Serverul s-a inchis!");
        btnConecteaza->Enabled = true;
        return;
    }

    // Verifica mesajul de final joc
    if (mesaj.Pos("FINAL:") == 1) {
        String date = mesaj.SubString(7, mesaj.Length() - 6);
        TStringList *parti = new TStringList();
        parti->Delimiter = ';';
        parti->StrictDelimiter = true;
        parti->DelimitedText = date;

        if (parti->Count >= 2) {
            int s1 = StrToInt(parti->Strings[0]);
            int s2 = StrToInt(parti->Strings[1]);

            String rezultat = "JOC TERMINAT!\n\n";
            rezultat += "Scor Final:\n";
            rezultat += "Server: " + IntToStr(s1) + "\n";
            rezultat += "Tu (Client): " + IntToStr(s2) + "\n\n";

            if (s2 > s1) {
                rezultat += "AI CASTIGAT! Felicitari!";
            } else if (s1 > s2) {
                rezultat += "AI PIERDUT! Mai mult noroc data viitoare!";
            } else {
                rezultat += "EGALITATE! Joc strans!";
            }

            ShowMessage(rezultat);
        }

        delete parti;
        return;
    }

    if (mesaj.Pos("CARTI:") == 1) {
        String date = mesaj.SubString(7, mesaj.Length() - 6);

        cartiMana.clear();

        if (date.Length() > 0) {
            TStringList *carti = new TStringList();
            carti->Delimiter = ',';
            carti->StrictDelimiter = true;
            carti->DelimitedText = date;

            for (int i = 0; i < carti->Count; i++) {
                TStringList *parti = new TStringList();
                parti->Delimiter = '|';
                parti->StrictDelimiter = true;
                parti->DelimitedText = carti->Strings[i];

                if (parti->Count == 3) {
                    int val = StrToInt(parti->Strings[0]);
                    std::string simb = AnsiString(parti->Strings[1]).c_str();
                    std::string suit = AnsiString(parti->Strings[2]).c_str();

                    cartiMana.push_back(Carte(val, simb, suit));
                }

                delete parti;
            }

            delete carti;
        }
    }
    else if (mesaj.Pos("STARE:") == 1) {
        String date = mesaj.SubString(7, mesaj.Length() - 6);

        TStringList *parti = new TStringList();
        parti->Delimiter = ';';
        parti->StrictDelimiter = true;
        parti->DelimitedText = date;

        int randJucator = 0;

        if (parti->Count >= 2) {
            punctajAdversar = StrToInt(parti->Strings[0]);
            punctajMeu = StrToInt(parti->Strings[1]);
        }

        cartiMasa.clear();

        if (parti->Count > 2 && parti->Strings[2].Length() > 0) {
            TStringList *carti = new TStringList();
            carti->Delimiter = ',';
            carti->StrictDelimiter = true;
            carti->DelimitedText = parti->Strings[2];

            for (int i = 0; i < carti->Count; i++) {
                TStringList *pc = new TStringList();
                pc->Delimiter = '|';
                pc->StrictDelimiter = true;
                pc->DelimitedText = carti->Strings[i];

                if (pc->Count == 3) {
                    int val = StrToInt(pc->Strings[0]);
                    std::string simb = AnsiString(pc->Strings[1]).c_str();
                    std::string suit = AnsiString(pc->Strings[2]).c_str();

                    cartiMasa.push_back(Carte(val, simb, suit));
                }

                delete pc;
            }

            delete carti;
        }

        if (parti->Count > 3) {
            randJucator = StrToInt(parti->Strings[3]);
        }

        delete parti;
        AfiseazaStareJoc(randJucator);
	}
}
//---------------------------------------------------------------------------
void TFormClient::AfiseazaStareJoc(int randJucator)
{
    while (pnlMana->ControlCount > 0) delete pnlMana->Controls[0];
    while (pnlMasaJoc->ControlCount > 0) delete pnlMasaJoc->Controls[0];

    for (size_t i = 0; i < cartiMana.size(); i++) {
        TImage *img = new TImage(this);
        img->Parent = pnlMana;
        img->SetBounds(5 + (i * 80), 5, 71, 96);
        img->Stretch = true;
        img->Proportional = true;
        img->Tag = i;
        img->OnClick = ImagineCarteClick;
        img->Picture->LoadFromFile(ObtineCaleImagine(cartiMana[i]));
    }

    for (size_t i = 0; i < cartiMasa.size(); i++) {
        TImage *img = new TImage(this);
        img->Parent = pnlMasaJoc;
        img->SetBounds(10 + ((i%6) * 85), 20 + ((i/6) * 105), 71, 96);
        img->Stretch = true;
        img->Proportional = true;
        img->Picture->LoadFromFile(ObtineCaleImagine(cartiMasa[i]));
    }

    lblPunctaj->Caption = "Tu: " + IntToStr(punctajMeu) +
                          " | Adversar: " + IntToStr(punctajAdversar);

    if (cartiMana.size() > 0) {
        memLog->Clear();
        memLog->Lines->Add("");

        // randJucator == 1 înseamnã cã este rândul clientului (Tu)
        if (randJucator == 1) {
            memLog->Lines->Add("      RANDUL TAU!");
            memLog->Lines->Add("");
            memLog->Lines->Add("  Click pe o carte pentru a juca");
        } else {
            memLog->Clear();
            memLog->Lines->Add("");
            memLog->Lines->Add("      Randul adversarului...");
            memLog->Lines->Add("");
            memLog->Lines->Add("  Asteapta mutarea adversarului");
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormClient::ImagineCarteClick(TObject *Sender)
{
    if (!conectat || !IdTCPClient1->Connected()) {
        ShowMessage("Nu esti conectat la server!");
        return;
    }

    TImage *img = dynamic_cast<TImage*>(Sender);
    if (img && img->Tag < (int)cartiMana.size()) {
        String mesaj = MesajeRetea::CreeazaMesajMutare(img->Tag);

        try {
            IdTCPClient1->IOHandler->WriteLn(mesaj);

            memLog->Clear();
            memLog->Lines->Add("");
            memLog->Lines->Add("  AI JUCAT: " + String(cartiMana[img->Tag].GetSimbol().c_str()) +
                              " de " + String(cartiMana[img->Tag].GetSuita().c_str()));
            memLog->Lines->Add("");
            memLog->Lines->Add("  Asteapta raspuns de la server...");

        } catch (Exception &e) {
            ShowMessage("Eroare trimitere: " + e.Message);
        }
    }
}
//---------------------------------------------------------------------------
String TFormClient::ObtineCaleImagine(const Carte& c)
{
    std::string sEng;
    if (c.GetSuita() == "Trefla") sEng = "clubs";
    else if (c.GetSuita() == "Romb") sEng = "diamonds";
    else if (c.GetSuita() == "Inima Rosie") sEng = "hearts";
    else sEng = "spades";

    std::string nSim;
    if (c.GetSimbol() == "A") nSim = "ace";
    else if (c.GetSimbol() == "J") nSim = "jack";
    else if (c.GetSimbol() == "D") nSim = "queen";
    else if (c.GetSimbol() == "K") nSim = "king";
    else nSim = c.GetSimbol();

    std::string numeFisier = nSim + "_of_" + sEng;
    if (c.GetSimbol() == "J" || c.GetSimbol() == "D" || c.GetSimbol() == "K") {
        numeFisier += "2";
    }
    numeFisier += ".png";

    String caleBaza = ExtractFilePath(Application->ExeName);
    return caleBaza + "PNG-cards-1.3\\" + String(numeFisier.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TFormClient::btnInchidereClick(TObject *Sender)
{

    TimerCitire->Enabled = false;
    conectat = false;

    // Deconecteaza de la server
    if (IdTCPClient1->Connected()) {
        try {
            IdTCPClient1->IOHandler->WriteLn("CLIENT_INCHIS");
            IdTCPClient1->Disconnect();
        } catch (...) {
        }
    }

    // Asteapta putin pentru a se inchide conexiunea
    Application->ProcessMessages();
    Sleep(200);

    Application->Terminate();
}
//---------------------------------------------------------------------------
