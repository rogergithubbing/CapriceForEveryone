object FormZIPCatalog: TFormZIPCatalog
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  Caption = 'ZIP Catalog'
  ClientHeight = 120
  ClientWidth = 361
  Color = clBtnFace
  Constraints.MinHeight = 158
  Constraints.MinWidth = 377
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnShow = FormShow
  DesignSize = (
    361
    120)
  PixelsPerInch = 96
  TextHeight = 13
  object ListBoxFiles: TListBox
    Left = 8
    Top = 8
    Width = 217
    Height = 105
    Anchors = [akLeft, akTop, akRight, akBottom]
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
  end
  object ButtonOK: TButton
    Left = 232
    Top = 8
    Width = 121
    Height = 65
    Anchors = [akRight, akBottom]
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 1
  end
  object ButtonCancel: TButton
    Left = 232
    Top = 80
    Width = 121
    Height = 33
    Anchors = [akRight, akBottom]
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 2
  end
end
