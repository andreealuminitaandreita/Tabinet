//---------------------------------------------------------------------------

#ifndef MainUnitH
#define MainUnitH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#include <vector>
//---------------------------------------------------------------------------
#include "GestionareJoc.h"
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Imaging.jpeg.hpp>
#include <Vcl.Imaging.pngimage.hpp>

class TForm1 : public TForm
{
__published:	// Componente gestionate de IDE
	TButton *btnInchidere;
	TButton *btnInitializareRunda;
	TButton *btnAccepta;
	TButton *btnRefuza;
    TImage *Image1;

	TMemo *memLog;
	TLabel *lblPunctajJucator;
	TPanel *pnlMasaJoc;
	TPanel *pnlMasa;

	void __fastcall btnInchidereClick(TObject *Sender);
	void __fastcall btnInitializareRundaClick(TObject *Sender);
	void __fastcall btnAcceptaClick(TObject *Sender);
	void __fastcall btnRefuzaClick(TObject *Sender);
	void __fastcall pnlMasaJocClick(TObject *Sender);

private:	// Declarații utilizator
	GestionareJoc *jocTabinet;
	std::vector<Carte> cartiPregatite;
	void AfiseazaStareJoc();
	void AfiseazaCartiPregatite(const std::vector<Carte>& carti);
	void FinalizeazaAlegerea(bool aAcceptat);
	void VerificaFinalJoc();
	String ObtineCaleImagine(const Carte& c);
	void __fastcall ImagineCarteClick(TObject *Sender);

public:		// Declarații utilizator
	__fastcall TForm1(TComponent* Owner);
	__fastcall ~TForm1();
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
