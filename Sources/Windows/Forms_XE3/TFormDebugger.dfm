object FormDebugger: TFormDebugger
  Left = 539
  Top = 435
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Execution'
  ClientHeight = 113
  ClientWidth = 306
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnHide = FormHide
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object ButtonPlayTillVBL: TButton
    Left = 8
    Top = 72
    Width = 129
    Height = 33
    Caption = 'Play till VBL'
    TabOrder = 0
    OnClick = ButtonPlayTillVBLClick
  end
  object ButtonPlayTillHBL: TButton
    Left = 152
    Top = 72
    Width = 137
    Height = 33
    Caption = 'Play till HBL'
    TabOrder = 1
    OnClick = ButtonPlayTillHBLClick
  end
  object ButtonStep: TButton
    Left = 8
    Top = 8
    Width = 89
    Height = 41
    Caption = 'Step'
    TabOrder = 2
    OnClick = ButtonStepClick
  end
  object ButtonPlay: TButton
    Left = 104
    Top = 8
    Width = 89
    Height = 41
    Caption = 'Play'
    TabOrder = 3
    OnClick = ButtonPlayClick
  end
  object ButtonRun: TButton
    Left = 200
    Top = 8
    Width = 91
    Height = 41
    Caption = 'Run'
    TabOrder = 4
    OnClick = ButtonRunClick
  end
end
