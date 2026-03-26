object FormClient: TFormClient
  Left = 0
  Top = 0
  Caption = 'FormClient'
  ClientHeight = 441
  ClientWidth = 624
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  TextHeight = 15
  object lblPunctaj: TLabel
    Left = 16
    Top = 184
    Width = 93
    Height = 15
    Caption = 'Tu: 0 | Adversar: 0'
  end
  object pnlMasaJoc: TPanel
    Left = 64
    Top = 24
    Width = 481
    Height = 105
    TabOrder = 0
  end
  object memLog: TMemo
    Left = 248
    Top = 152
    Width = 297
    Height = 89
    Lines.Strings = (
      'memLog')
    ReadOnly = True
    TabOrder = 1
  end
  object pnlMana: TPanel
    Left = 64
    Top = 264
    Width = 481
    Height = 81
    TabOrder = 2
  end
  object btnConecteaza: TButton
    Left = 32
    Top = 368
    Width = 97
    Height = 41
    Caption = 'Conecteaza'
    TabOrder = 3
    OnClick = btnConecteazaClick
  end
  object btnInchidere: TButton
    Left = 496
    Top = 368
    Width = 99
    Height = 41
    Caption = 'Inchidere'
    TabOrder = 4
    OnClick = btnInchidereClick
  end
  object IdTCPClient1: TIdTCPClient
    ConnectTimeout = 0
    Port = 2000
    ReadTimeout = -1
    Left = 176
    Top = 384
  end
  object TimerCitire: TTimer
    Enabled = False
    Interval = 100
    OnTimer = TimerCitireTimer
    Left = 360
    Top = 384
  end
end
