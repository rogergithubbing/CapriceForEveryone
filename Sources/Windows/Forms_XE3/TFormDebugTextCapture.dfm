object FormDebugTextCapture: TFormDebugTextCapture
  Left = 352
  Top = 216
  BorderIcons = [biSystemMenu]
  Caption = 'Text Capture'
  ClientHeight = 256
  ClientWidth = 364
  Color = clBtnFace
  Constraints.MinHeight = 230
  Constraints.MinWidth = 350
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnHide = FormHide
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object MemoCapture: TMemo
    Left = 0
    Top = 49
    Width = 364
    Height = 207
    Cursor = crArrow
    Align = alClient
    BorderStyle = bsNone
    Lines.Strings = (
      
        '0000000001111111111222222222233333333334444444444555555555566666' +
        '6666677777777778'
      
        '1234567890123456789012345678901234567890123456789012345678901234' +
        '5678901234567890')
    ParentColor = True
    ReadOnly = True
    ScrollBars = ssBoth
    TabOrder = 0
  end
  object PanelControls: TPanel
    Left = 0
    Top = 0
    Width = 364
    Height = 49
    Align = alTop
    ParentColor = True
    TabOrder = 1
    object ButtonStart: TButton
      Left = 8
      Top = 8
      Width = 129
      Height = 25
      Caption = 'Start / Stop'
      TabOrder = 0
      OnClick = ButtonStartClick
    end
    object ButtonClear: TButton
      Left = 144
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Clear'
      TabOrder = 1
      OnClick = ButtonClearClick
    end
  end
end
