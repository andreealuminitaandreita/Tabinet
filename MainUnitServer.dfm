object FormServer: TFormServer
  Left = 0
  Top = 0
  Caption = 'FormServer'
  ClientHeight = 433
  ClientWidth = 622
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  OnDestroy = FormDestroy
  TextHeight = 15
  object lblPunctajJucator: TLabel
    Left = 24
    Top = 184
    Width = 52
    Height = 15
    Caption = 'Punctaj: 0'
  end
  object pnlMasaJoc: TPanel
    Left = 72
    Top = 24
    Width = 449
    Height = 97
    TabOrder = 0
  end
  object memLog: TMemo
    Left = 200
    Top = 144
    Width = 185
    Height = 89
    ImeName = 'Memo1'
    Lines.Strings = (
      'memLog')
    ReadOnly = True
    TabOrder = 1
  end
  object pnlMasa: TPanel
    Left = 72
    Top = 256
    Width = 449
    Height = 73
    TabOrder = 2
  end
  object btnInitializareRunda: TButton
    Left = 48
    Top = 352
    Width = 81
    Height = 41
    Caption = 'Start Runda'
    TabOrder = 3
    OnClick = btnInitializareRundaClick
  end
  object btnAccepta: TButton
    Left = 184
    Top = 352
    Width = 75
    Height = 41
    Caption = 'Accepta'
    TabOrder = 4
    Visible = False
    OnClick = btnAcceptaClick
  end
  object btnRefuza: TButton
    Left = 328
    Top = 352
    Width = 75
    Height = 41
    Caption = 'Refuza'
    TabOrder = 5
    Visible = False
    OnClick = btnRefuzaClick
  end
  object btnInchidere: TButton
    Left = 464
    Top = 352
    Width = 75
    Height = 41
    Caption = 'Inchidere'
    TabOrder = 6
    OnClick = btnInchidereClick
  end
  object IdTCPServer1: TIdTCPServer
    Bindings = <>
    DefaultPort = 2000
    OnConnect = IdTCPServer1Connect
    OnDisconnect = IdTCPServer1Disconnect
    OnExecute = IdTCPServer1Execute
    Left = 144
    Top = 392
  end
end
