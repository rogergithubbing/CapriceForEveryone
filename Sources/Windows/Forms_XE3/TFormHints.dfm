object FormHints: TFormHints
  Left = 334
  Top = 149
  BorderIcons = [biSystemMenu]
  Caption = 'Hints'
  ClientHeight = 362
  ClientWidth = 584
  Color = clBtnFace
  Constraints.MinHeight = 400
  Constraints.MinWidth = 600
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  Position = poScreenCenter
  OnKeyPress = FormKeyPress
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object RichEditHints: TRichEdit
    Left = 0
    Top = 0
    Width = 584
    Height = 362
    Align = alClient
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Verdana'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    ScrollBars = ssVertical
    TabOrder = 0
    Zoom = 100
  end
end
