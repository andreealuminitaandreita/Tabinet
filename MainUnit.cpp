//---------------------------------------------------------------------------

#include <vcl.h>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#include "MainUnit.h"
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#pragma hdrstop
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

__fastcall TForm1::TForm1(TComponent* Owner) : TForm(Owner) {
    jocTabinet = new GestionareJoc();
	jocTabinet->AdaugaJucator("Jucator 1");
    jocTabinet->AdaugaJucator("Jucator PC");
    jocTabinet->InitializareJoc();


    btnAccepta->Visible = false;
	btnRefuza->Visible = false;


	memLog->Width = 350;
    memLog->Height = 80;
    memLog->Font->Size = 8;
}

__fastcall TForm1::~TForm1() {
	delete jocTabinet;
}

// Logica pentru butonul "Start Runda"
void __fastcall TForm1::btnInitializareRundaClick(TObject *Sender) {
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

void TForm1::FinalizeazaAlegerea(bool aAcceptat) {
    jocTabinet->FinalizeazaDistributiaPrimeiRunde(aAcceptat, cartiPregatite);
	btnAccepta->Visible = false;
    btnRefuza->Visible = false;
    AfiseazaStareJoc();
}

void __fastcall TForm1::btnAcceptaClick(TObject *Sender) {
    FinalizeazaAlegerea(true);
}

void __fastcall TForm1::btnRefuzaClick(TObject *Sender) {
	FinalizeazaAlegerea(false);
}

String TForm1::ObtineCaleImagine(const Carte& c) {
	// Mapare suita
    std::string sEng;
    if (c.GetSuita() == "Trefla") sEng = "clubs";
    else if (c.GetSuita() == "Romb") sEng = "diamonds";
    else if (c.GetSuita() == "Inima Rosie") sEng = "hearts";
    else sEng = "spades";

    // Mapare simbol
    std::string nSim;
    if (c.GetSimbol() == "A") nSim = "ace";
    else if (c.GetSimbol() == "J") nSim = "jack";
    else if (c.GetSimbol() == "D") nSim = "queen";
    else if (c.GetSimbol() == "K") nSim = "king";
    else nSim = c.GetSimbol();

	// Construieste numele fisierului
    std::string numeFisier = nSim + "_of_" + sEng;

	// Pentru figuri adauga "2" la final
    if (c.GetSimbol() == "J" || c.GetSimbol() == "D" || c.GetSimbol() == "K") {
        numeFisier += "2";
    }

    numeFisier += ".png";

	// Calea completa
    String caleBaza = ExtractFilePath(Application->ExeName);
    return caleBaza + "PNG-cards-1.3\\" + String(numeFisier.c_str());
}

void TForm1::AfiseazaStareJoc() {
    while (pnlMasa->ControlCount > 0) delete pnlMasa->Controls[0];
    while (pnlMasaJoc->ControlCount > 0) delete pnlMasaJoc->Controls[0];

	const auto& juc = jocTabinet->GetListaJucatori();

	// Mana Jucatorului 1
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

	// Afisam Masa de joc
    for (size_t i = 0; i < jocTabinet->GetMasa()->GetCartiPeMasa().size(); i++) {
		TImage *img = new TImage(this);
        img->Parent = pnlMasaJoc;
        img->SetBounds(10 + ((i%6) * 85), 20 + ((i/6) * 105), 71, 96);
		img->Stretch = true;
        img->Proportional = true;
        img->Picture->LoadFromFile(ObtineCaleImagine(jocTabinet->GetMasa()->GetCartiPeMasa()[i]));
    }

	// Afiseaza mesaj despre randul cui este
	memLog->Clear();
    memLog->Lines->Add("");
	memLog->Lines->Add("      RANDUL TAU!");
    memLog->Lines->Add("");
	lblPunctajJucator->Caption = "Punctaj: " + IntToStr(juc[0].CalculeazaPunctajFinal());
}

void __fastcall TForm1::ImagineCarteClick(TObject *Sender) {
    TImage *img = dynamic_cast<TImage*>(Sender);
	if (img) {
		// JUCATORUL joaca cartea
        const auto& juc = jocTabinet->GetListaJucatori();
        Carte cartaJucata = juc[0].GetCartiInMana()[img->Tag];

        memLog->Clear();
        memLog->Lines->Add("");
        memLog->Lines->Add("  AI JUCAT: " + String(cartaJucata.GetSimbol().c_str()) +
                          " de " + String(cartaJucata.GetSuita().c_str()));
        memLog->Lines->Add("");

        jocTabinet->ExecutaMutare(0, img->Tag);
        AfiseazaStareJoc();
        Application->ProcessMessages();
        Sleep(800);

        // Dacă PC-ul mai are cărți, e rândul lui
        if (jocTabinet->GetListaJucatori()[1].NumarCartiInMana() > 0) {
			// Salvează cartea PC-ului INAINTE sa o joace
            const auto& jucPC = jocTabinet->GetListaJucatori();
            Carte cartaPC = jucPC[1].GetCartiInMana()[0];

            // Arată mesaj că PC-ul se gândește
            memLog->Clear();
            memLog->Lines->Add("");
            memLog->Lines->Add("      PC-ul se gandeste...");
            memLog->Lines->Add("");
            Application->ProcessMessages();
            Sleep(1000);

            // Arată ce carte joacă PC-ul
            memLog->Clear();
            memLog->Lines->Add("");
            memLog->Lines->Add("  PC-ul JOACA: " + String(cartaPC.GetSimbol().c_str()) +
                              " de " + String(cartaPC.GetSuita().c_str()));
            memLog->Lines->Add("");
            Application->ProcessMessages();
            Sleep(1500);

            // PC-ul execută mutarea
            jocTabinet->ExecutaMutare(1, 0);
            AfiseazaStareJoc();
        }

        // Arată din nou că e rândul tău
        memLog->Clear();
		memLog->Lines->Add("");
        memLog->Lines->Add("      RANDUL TAU!");
        memLog->Lines->Add("");
        memLog->Lines->Add("  Click pe o carte sa joci");
        memLog->Lines->Add("");

        VerificaFinalJoc();
    }
}

void TForm1::VerificaFinalJoc() {
	if (jocTabinet->GetPachet()->EsteGol() && jocTabinet->GetListaJucatori()[0].NumarCartiInMana() == 0) {
        int s1 = jocTabinet->GetListaJucatori()[0].CalculeazaPunctajFinal();
		int s2 = jocTabinet->GetListaJucatori()[1].CalculeazaPunctajFinal();
        ShowMessage("Joc Terminat! Scor: " + IntToStr(s1) + " - " + IntToStr(s2));
    }
}

void TForm1::AfiseazaCartiPregatite(const std::vector<Carte>& c) {
    // Curăță panoul mâinii
    while (pnlMasa->ControlCount > 0) delete pnlMasa->Controls[0];

    // Afișează cele 4 cărți PREGATITE
    for (size_t i = 0; i < c.size(); i++) {
        TImage *img = new TImage(this);
        img->Parent = pnlMasa;
        img->SetBounds(5 + (i * 80), 5, 71, 96);
        img->Stretch = true;
        img->Proportional = true;
        img->Picture->LoadFromFile(ObtineCaleImagine(c[i]));
    }

    // Actualizează label-ul
    lblPunctajJucator->Caption = "Alege: Accepta sau Refuza?";
}

void __fastcall TForm1::btnInchidereClick(TObject *Sender)
{
	Application->Terminate();
}
void __fastcall TForm1::pnlMasaJocClick(TObject *Sender)
{
    // Lasă gol
}
