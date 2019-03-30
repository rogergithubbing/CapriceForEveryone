object FormDiskCatalog: TFormDiskCatalog
  Left = 582
  Top = 213
  BorderIcons = [biSystemMenu]
  Caption = 'Drive A disk catalog'
  ClientHeight = 184
  ClientWidth = 299
  Color = clWindow
  Constraints.MinHeight = 222
  Constraints.MinWidth = 315
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object ListBoxCatalogEntries: TListBox
    Left = 0
    Top = 0
    Width = 192
    Height = 184
    Align = alClient
    BorderStyle = bsNone
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -21
    Font.Name = 'Courier New'
    Font.Style = []
    IntegralHeight = True
    ItemHeight = 23
    Items.Strings = (
      '1'
      '2'
      '3'
      '4'
      '5'
      '6'
      '7'
      '8')
    ParentFont = False
    Sorted = True
    TabOrder = 0
    OnDblClick = ListBoxCatalogEntriesDblClick
  end
  object PanelButtons: TPanel
    Left = 192
    Top = 0
    Width = 107
    Height = 184
    Align = alRight
    BevelOuter = bvNone
    TabOrder = 1
    DesignSize = (
      107
      184)
    object ButtonCancel: TButton
      Left = 6
      Top = 142
      Width = 91
      Height = 33
      Anchors = [akLeft, akBottom]
      Cancel = True
      Caption = 'Cancel'
      ModalResult = 2
      TabOrder = 0
    end
    object ButtonLOAD: TButton
      Left = 8
      Top = 48
      Width = 89
      Height = 33
      Hint = 'Press F2'
      Cancel = True
      Caption = 'LOAD'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Courier New'
      Font.Style = []
      ModalResult = 1
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 2
      OnClick = ButtonLOADClick
    end
    object ButtonRUN: TButton
      Left = 8
      Top = 8
      Width = 89
      Height = 33
      Hint = 'Press F1'
      Caption = 'RUN'
      Default = True
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Courier New'
      Font.Style = []
      ModalResult = 1
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 1
      OnClick = ButtonRUNClick
    end
    object ButtonCPM: TButton
      Left = 8
      Top = 88
      Width = 89
      Height = 33
      Hint = 'Press F3'
      Cancel = True
      Caption = '|CPM'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Courier New'
      Font.Style = []
      ModalResult = 1
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 3
      OnClick = ButtonCPMClick
    end
  end
end
