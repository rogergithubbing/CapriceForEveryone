object FormCustomSpeed: TFormCustomSpeed
  Left = 523
  Top = 317
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Custom speed'
  ClientHeight = 141
  ClientWidth = 296
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object LabelCustomSpeed: TLabel
    Left = 24
    Top = 16
    Width = 153
    Height = 31
    AutoSize = False
    Caption = 'Speed (%)'
    Layout = tlCenter
  end
  object LabelNote: TLabel
    Left = 24
    Top = 56
    Width = 145
    Height = 13
    Caption = 'Note: Real CPC speed = 100%'
  end
  object ButtonOK: TButton
    Left = 40
    Top = 88
    Width = 113
    Height = 41
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 1
  end
  object ButtonCancel: TButton
    Left = 168
    Top = 88
    Width = 89
    Height = 41
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 2
  end
  object EditCustomSpeed: TEdit
    Left = 184
    Top = 16
    Width = 89
    Height = 31
    AutoSize = False
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Verdana'
    Font.Style = []
    MaxLength = 4
    ParentFont = False
    TabOrder = 0
    Text = '0'
    OnChange = EditCustomSpeedChange
  end
end
