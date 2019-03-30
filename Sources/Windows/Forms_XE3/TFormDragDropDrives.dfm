object FormDragDropDrives: TFormDragDropDrives
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Load on drive ?'
  ClientHeight = 129
  ClientWidth = 257
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnKeyPress = FormKeyPress
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object PanelDriveB: TPanel
    Left = 128
    Top = 0
    Width = 129
    Height = 129
    Align = alRight
    Caption = 'B'
    TabOrder = 0
    OnClick = PanelDriveBClick
    OnMouseEnter = PanelDriveBMouseEnter
    OnMouseLeave = PanelDriveBMouseLeave
  end
  object PanelDriveA: TPanel
    Left = 0
    Top = 0
    Width = 129
    Height = 129
    Align = alLeft
    Caption = 'A'
    TabOrder = 1
    OnClick = PanelDriveAClick
    OnMouseEnter = PanelDriveAMouseEnter
    OnMouseLeave = PanelDriveAMouseLeave
  end
end
