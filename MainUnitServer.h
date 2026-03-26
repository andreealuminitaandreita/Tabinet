//---------------------------------------------------------------------------

#ifndef MainUnitServerH
#define MainUnitServerH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdCustomTCPServer.hpp>
#include <IdTCPServer.hpp>
#include <IdContext.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#include <vector>
#include <sstream>
#include "GestionareJoc.h"
#include "EntitateJoc.h"
#include "MesajeRetea.h"
//---------------------------------------------------------------------------
class TFormServer : public TForm
{
__published:
    TIdTCPServer *IdTCPServer1;
    TButton *btnInchidere;
    TButton *btnInitializareRunda;
    TButton *btnAccepta;
    TButton *btnRefuza;
    TMemo *memLog;
    TLabel *lblPunctajJucator;
    TPanel *pnlMasaJoc;
    TPanel *pnlMasa;
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall btnInchidereClick(TObject *Sender);
    void __fastcall btnInitializareRundaClick(TObject *Sender);
    void __fastcall btnAcceptaClick(TObject *Sender);
    void __fastcall btnRefuzaClick(TObject *Sender);
    void __fastcall IdTCPServer1Connect(TIdContext *AContext);
    void __fastcall IdTCPServer1Disconnect(TIdContext *AContext);
    void __fastcall IdTCPServer1Execute(TIdContext *AContext);

private:
    GestionareJoc *jocTabinet;
    std::vector<Carte> cartiPregatite;
    bool clientConectat;
    TIdContext *clientContext;

    void AfiseazaStareJoc();
    void AfiseazaCartiPregatite(const std::vector<Carte>& carti);
    void FinalizeazaAlegerea(bool aAcceptat);
    void VerificaFinalJoc();
    String ObtineCaleImagine(const Carte& c);
    void TrimiteStareJocCatreClient(int randJucator);
    void TrimiteCartiCatreClient();
    void ProceseazaMesajDelaClient(const String& mesaj);
    void __fastcall ImagineCarteClick(TObject *Sender);

public:
    __fastcall TFormServer(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormServer *FormServer;
//---------------------------------------------------------------------------
#endif
