//---------------------------------------------------------------------------

#ifndef MainUnitClientH
#define MainUnitClientH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#include <vector>
#include "Carte.h"
#include "MesajeRetea.h"
//---------------------------------------------------------------------------
class TFormClient : public TForm
{
__published:
    TIdTCPClient *IdTCPClient1;
    TTimer *TimerCitire;
    TButton *btnConecteaza;
    TButton *btnInchidere;
    TPanel *pnlMana;
    TPanel *pnlMasaJoc;
    TMemo *memLog;
    TLabel *lblPunctaj;

    void __fastcall btnConecteazaClick(TObject *Sender);
    void __fastcall btnInchidereClick(TObject *Sender);
    void __fastcall TimerCitireTimer(TObject *Sender);
    void __fastcall ImagineCarteClick(TObject *Sender);

private:
    bool conectat;
    std::vector<Carte> cartiMana;
    std::vector<Carte> cartiMasa;
    int punctajMeu;
    int punctajAdversar;

    void ProceseazaMesajDelaServer(const String& mesaj);
    void AfiseazaStareJoc(int randJucator);
    String ObtineCaleImagine(const Carte& c);

public:
    __fastcall TFormClient(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormClient *FormClient;
//---------------------------------------------------------------------------
#endif
