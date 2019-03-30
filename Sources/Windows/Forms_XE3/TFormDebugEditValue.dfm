object FormDebugEditValue: TFormDebugEditValue
  Left = 683
  Top = 282
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'New value'
  ClientHeight = 162
  ClientWidth = 346
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  Position = poOwnerFormCenter
  ShowHint = True
  OnKeyPress = FormKeyPress
  OnShow = FormShow
  DesignSize = (
    346
    162)
  PixelsPerInch = 96
  TextHeight = 13
  object LabelCurrent: TLabel
    Left = 16
    Top = 16
    Width = 65
    Height = 27
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'Current'
    Layout = tlCenter
  end
  object LabelNew: TLabel
    Left = 16
    Top = 64
    Width = 65
    Height = 33
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'New'
    Layout = tlCenter
  end
  object ButtonOK: TButton
    Left = 128
    Top = 112
    Width = 113
    Height = 41
    Anchors = [akRight, akBottom]
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 0
  end
  object ButtonCancel: TButton
    Left = 248
    Top = 112
    Width = 89
    Height = 41
    Anchors = [akRight, akBottom]
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
  end
  object PanelCurrent: TPanel
    Left = 113
    Top = 16
    Width = 104
    Height = 33
    Hint = 'Horizontal Total'
    ParentBackground = False
    ParentColor = True
    TabOrder = 4
    object LabelCurrentValue: TLabel
      Left = 5
      Top = 4
      Width = 93
      Height = 23
      AutoSize = False
    end
  end
  object PanelFormatNew: TPanel
    Left = 88
    Top = 64
    Width = 25
    Height = 33
    Caption = '&&'
    Color = 16773087
    ParentBackground = False
    TabOrder = 5
    OnClick = PanelFormatNewClick
    OnDblClick = PanelFormatNewClick
  end
  object PanelFormatCurrent: TPanel
    Left = 88
    Top = 16
    Width = 25
    Height = 33
    BevelOuter = bvNone
    Caption = '&&'
    ParentBackground = False
    ParentColor = True
    TabOrder = 6
  end
  object ButtonIncrement: TButton
    Left = 217
    Top = 64
    Width = 41
    Height = 16
    Caption = '+'
    TabOrder = 2
    OnClick = ButtonIncrementClick
  end
  object ButtonDecrement: TButton
    Left = 217
    Top = 81
    Width = 41
    Height = 16
    Caption = '-'
    TabOrder = 3
    OnClick = ButtonDecrementClick
  end
  object PanelNew: TPanel
    Left = 113
    Top = 64
    Width = 105
    Height = 33
    Color = clWhite
    ParentBackground = False
    TabOrder = 7
    object EditNew: TEdit
      Left = 5
      Top = 4
      Width = 93
      Height = 23
      AutoSize = False
      BorderStyle = bsNone
      MaxLength = 9
      TabOrder = 0
      Text = '0'
      OnChange = EditNewChange
    end
  end
  object PanelIncrement: TPanel
    Left = 258
    Top = 64
    Width = 73
    Height = 33
    Color = clWhite
    ParentBackground = False
    TabOrder = 8
    object EditIncrement: TEdit
      Left = 5
      Top = 4
      Width = 60
      Height = 25
      AutoSize = False
      BorderStyle = bsNone
      TabOrder = 0
      Text = '0'
      OnChange = EditIncrementChange
    end
  end
  object PanelNext: TPanel
    Left = 16
    Top = 112
    Width = 73
    Height = 41
    Caption = 'Next'
    TabOrder = 9
    OnClick = PanelNextClick
  end
end
