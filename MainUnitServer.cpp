//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MainUnitServer.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormServer *FormServer;
//---------------------------------------------------------------------------
__fastcall TFormServer::TFormServer(TComponent* Owner) : TForm(Owner)
{
    jocTabinet = new GestionareJoc();
    jocTabinet->AdaugaJucator("Server (Tu)");
    jocTabinet->AdaugaJucator("Client");
    jocTabinet->InitializareJoc();

    clientConectat = false;
    clientContext = NULL;

    btnAccepta->Visible = false;
    btnRefuza->Visible = false;
    btnInitializareRunda->Enabled = false;

    memLog->Width = 400;
    memLog->Height = 120;
    memLog->Font->Size = 8;

    try {
        IdTCPServer1->DefaultPort = 2000;
        IdTCPServer1->Active = true;
        memLog->Lines->Add("SERVER PORNIT pe portul 2000");
        memLog->Lines->Add("Asteapta client sa se conecteze...");
        memLog->Lines->Add("");
    } catch (Exception &e) {
        ShowMessage("EROARE pornire server: " + e.Message);
        memLog->Lines->Add("EROARE: " + e.Message);
        memLog->Lines->Add("Incearca alt port sau verifica firewall-ul");
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormServer::FormDestroy(TObject *Sender)
{
    IdTCPServer1->Active = false;
    delete jocTabinet;
}
//---------------------------------------------------------------------------
void __fastcall TFormServer::IdTCPServer1Connect(TIdContext *AContext)
{
    TThread::Synchronize(NULL, [this, AContext]() {
        clientConectat = true;
        clientContext = AContext;
        memLog->Lines->Add("CLIENT CONECTAT!");
        memLog->Lines->Add("IP: " + AContext->Binding->PeerIP);
        memLog->Lines->Add("");
        btnInitializareRunda->Enabled = true;
    });
}
//---------------------------------------------------------------------------
void __fastcall TFormServer::IdTCPServer1Disconnect(TIdContext *AContext)
{
    TThread::Synchronize(NULL, [this]() {
        clientConectat = false;
        clientContext = NULL;
        memLog->Lines->Add("CLIENT DECONECTAT!");
        memLog->Lines->Add("");
        btnInitializareRunda->Enabled = false;
	});
}
//---------------------------------------------------------------------------
void __fastcall TFormServer::IdTCPServer1Execute(TIdContext *AContext)
{
    try {
        String mesaj = AContext->Connection->IOHandler->ReadLn();

        if (!mesaj.IsEmpty()) {
            TThread::Synchronize(NULL, [this, mesaj]() {
                // Verifica daca clientul se inchide
                if (mesaj.Pos("CLIENT_INCHIS") == 1) {
                    memLog->Lines->Add("Clientul s-a deconectat.");
                    return;
                }

                memLog->Lines->Add("Primit de la client: " + mesaj);
                ProceseazaMesajDelaClient(mesaj);
            });
        }
    } catch (...) {
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormServer::btnInitializareRundaClick(TObject *Sender)
{
    cartiPregatite = jocTabinet->DistribuieCartiInitialPrimeiRunde();
    AfiseazaCartiPregatite(cartiPregatite);

    if (jocTabinet->ContineDouaCartiCuPuncte(cartiPregatite)) {
        ShowMessage("Ai 2 puncte! Esti obligat sa le pastrezi.");
        FinalizeazaAlegerea(true);
    } else {
        btnAccepta->Visible = true;
        btnRefuza->Visible = true;
    }
    btnInitializareRunda->Enabled = false;
}
//---------------------------------------------------------------------------
void TFormServer::FinalizeazaAlegerea(bool aAcceptat)
{
    jocTabinet->FinalizeazaDistributiaPrimeiRunde(aAcceptat, cartiPregatite);
    btnAccepta->Visible = false;
    btnRefuza->Visible = false;
    AfiseazaStareJoc();

    TrimiteCartiCatreClient();
    // Serverul (index 0) începe primul
    TrimiteStareJocCatreClient(jocTabinet->GetIndexJucatorCurent());
}
//---------------------------------------------------------------------------
void __fastcall TFormServer::btnAcceptaClick(TObject *Sender)
{
    FinalizeazaAlegerea(true);
}
//---------------------------------------------------------------------------
void __fastcall TFormServer::btnRefuzaClick(TObject *Sender)
{
    FinalizeazaAlegerea(false);
}
//---------------------------------------------------------------------------
void TFormServer::TrimiteCartiCatreClient()
{
    if (!clientConectat || clientContext == NULL) return;

    const auto& juc = jocTabinet->GetListaJucatori();
    const auto& cartiClient = juc[1].GetCartiInMana();

    String mesaj = MesajeRetea::CreeazaMesajDistribuie(cartiClient);

    try {
        clientContext->Connection->IOHandler->WriteLn(mesaj);
        memLog->Lines->Add("Trimis carti catre client");
    } catch (...) {
        memLog->Lines->Add("EROARE: Nu pot trimite carti");
    }
}
//---------------------------------------------------------------------------
void TFormServer::TrimiteStareJocCatreClient(int randJucator)
{
    if (!clientConectat || clientContext == NULL) return;

    const auto& juc = jocTabinet->GetListaJucatori();
    const auto& masa = jocTabinet->GetMasa()->GetCartiPeMasa();

    String mesaj = MesajeRetea::CreeazaMesajStareJoc(
        masa,
        juc[0].CalculeazaPunctajFinal(),
        juc[1].CalculeazaPunctajFinal(),
        randJucator
    );

    try {
        clientContext->Connection->IOHandler->WriteLn(mesaj);
        memLog->Lines->Add("Trimis stare joc catre client");
    } catch (...) {
        memLog->Lines->Add("EROARE: Nu pot trimite starea");
    }
}
//---------------------------------------------------------------------------
void TFormServer::ProceseazaMesajDelaClient(const String& mesaj)
{
    if (mesaj.Pos("MUTARE:") == 1) {
        int indexCarte = StrToInt(mesaj.SubString(8, mesaj.Length() - 7));

        const auto& juc = jocTabinet->GetListaJucatori();
        Carte cartaJucata = juc[1].GetCartiInMana()[indexCarte];

        memLog->Clear();
        memLog->Lines->Add("");
        memLog->Lines->Add("  CLIENT-ul joaca: " + String(cartaJucata.GetSimbol().c_str()) +
                          " de " + String(cartaJucata.GetSuita().c_str()));
        memLog->Lines->Add("");
        Application->ProcessMessages();
        Sleep(1000);

        jocTabinet->ExecutaMutare(1, indexCarte);

        AfiseazaStareJoc();
        TrimiteCartiCatreClient();
        TrimiteStareJocCatreClient(jocTabinet->GetIndexJucatorCurent());

        // Verificã dacã jocul s-a terminat
        VerificaFinalJoc();
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormServer::ImagineCarteClick(TObject *Sender)
{
    // Verificã dacã este rândul serverului
    if (jocTabinet->GetIndexJucatorCurent() != 0) {
        ShowMessage("Nu este randul tau!");
        return;
    }

    TImage *img = dynamic_cast<TImage*>(Sender);
    if (img) {
        const auto& juc = jocTabinet->GetListaJucatori();
        Carte cartaJucata = juc[0].GetCartiInMana()[img->Tag];

        memLog->Clear();
        memLog->Lines->Add("");
        memLog->Lines->Add("  AI JUCAT: " + String(cartaJucata.GetSimbol().c_str()) +
                          " de " + String(cartaJucata.GetSuita().c_str()));
        memLog->Lines->Add("");
        Application->ProcessMessages();
        Sleep(800);

        jocTabinet->ExecutaMutare(0, img->Tag);

        AfiseazaStareJoc();
        TrimiteCartiCatreClient();
        TrimiteStareJocCatreClient(jocTabinet->GetIndexJucatorCurent());

        VerificaFinalJoc();
    }
}
//---------------------------------------------------------------------------
String TFormServer::ObtineCaleImagine(const Carte& c)
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
void TFormServer::AfiseazaStareJoc()
{
    while (pnlMasa->ControlCount > 0) delete pnlMasa->Controls[0];
    while (pnlMasaJoc->ControlCount > 0) delete pnlMasaJoc->Controls[0];

    const auto& juc = jocTabinet->GetListaJucatori();

    for (size_t i = 0; i < juc[0].GetCartiInMana().size(); i++) {
        TImage *img = new TImage(this);
        img->Parent = pnlMasa;
        img->SetBounds(5 + (i * 80), 5, 71, 96);
        img->Stretch = true;
        img->Proportional = true;
        img->Tag = i;
        img->OnClick = ImagineCarteClick;
        img->Picture->LoadFromFile(ObtineCaleImagine(juc[0].GetCartiInMana()[i]));
    }

    for (size_t i = 0; i < jocTabinet->GetMasa()->GetCartiPeMasa().size(); i++) {
        TImage *img = new TImage(this);
        img->Parent = pnlMasaJoc;
        img->SetBounds(10 + ((i%6) * 85), 20 + ((i/6) * 105), 71, 96);
        img->Stretch = true;
        img->Proportional = true;
        img->Picture->LoadFromFile(ObtineCaleImagine(jocTabinet->GetMasa()->GetCartiPeMasa()[i]));
    }

    lblPunctajJucator->Caption = "Punctaj: " + IntToStr(juc[0].CalculeazaPunctajFinal());

    memLog->Clear();
    memLog->Lines->Add("");

    if (jocTabinet->GetIndexJucatorCurent() == 0) {
        memLog->Lines->Add("      RANDUL TAU!");
        memLog->Lines->Add("");
        memLog->Lines->Add("  Click pe o carte pentru a juca");
    } else {
        memLog->Lines->Add("      Randul clientului...");
        memLog->Lines->Add("");
        memLog->Lines->Add("  Asteapta mutarea adversarului");
    }
}
//---------------------------------------------------------------------------
void TFormServer::AfiseazaCartiPregatite(const std::vector<Carte>& c)
{
    while (pnlMasa->ControlCount > 0) delete pnlMasa->Controls[0];

    for (size_t i = 0; i < c.size(); i++) {
        TImage *img = new TImage(this);
        img->Parent = pnlMasa;
        img->SetBounds(5 + (i * 80), 5, 71, 96);
        img->Stretch = true;
        img->Proportional = true;
        img->Picture->LoadFromFile(ObtineCaleImagine(c[i]));
    }

    lblPunctajJucator->Caption = "Alege: Accepta sau Refuza?";
}
//---------------------------------------------------------------------------
void TFormServer::VerificaFinalJoc()
{
	if (jocTabinet->GetPachet()->EsteGol() &&
        jocTabinet->GetListaJucatori()[0].NumarCartiInMana() == 0 &&
        jocTabinet->GetListaJucatori()[1].NumarCartiInMana() == 0) {

        int s1 = jocTabinet->GetListaJucatori()[0].CalculeazaPunctajFinal();
        int s2 = jocTabinet->GetListaJucatori()[1].CalculeazaPunctajFinal();

        String rezultat = "JOC TERMINAT!\n\n";
        rezultat += "Scor Final:\n";
        rezultat += "Server (Tu): " + IntToStr(s1) + "\n";
        rezultat += "Client: " + IntToStr(s2) + "\n\n";

        if (s1 > s2) {
            rezultat += "AI CASTIGAT! Felicitari!";
        } else if (s2 > s1) {
            rezultat += "AI PIERDUT! Mai mult noroc data viitoare!";
        } else {
            rezultat += "EGALITATE! Joc strans!";
        }

        ShowMessage(rezultat);

        // Trimite mesaj de final catre client
        if (clientConectat && clientContext != NULL) {
            try {
                clientContext->Connection->IOHandler->WriteLn("FINAL:" + IntToStr(s1) + ";" + IntToStr(s2));
            } catch (...) {
            }
        }
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormServer::btnInchidereClick(TObject *Sender)
{
    // Deconecteaza clientul daca este conectat
    if (clientConectat && clientContext != NULL) {
        try {
            clientContext->Connection->IOHandler->WriteLn("SERVER_INCHIS");
            clientContext->Connection->Disconnect();
        } catch (...) {
        }
    }

    // Opreste serverul
    try {
        IdTCPServer1->Active = false;
    } catch (...) {
    }

    // Asteapta putin pentru a se inchide conexiunile
    Application->ProcessMessages();
    Sleep(200);

    Application->Terminate();
}
//---------------------------------------------------------------------------
