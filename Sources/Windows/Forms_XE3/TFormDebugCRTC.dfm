object FormDebugCRTC: TFormDebugCRTC
  Left = 825
  Top = 181
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'CRTC'
  ClientHeight = 257
  ClientWidth = 425
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  ShowHint = True
  OnHide = FormHide
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBoxRegisters: TGroupBox
    Left = 8
    Top = 8
    Width = 233
    Height = 241
    Caption = ' Registers '
    TabOrder = 0
    object LabelR0: TLabel
      Left = 32
      Top = 18
      Width = 25
      Height = 21
      Hint = 'Horizontal Total'
      AutoSize = False
      Caption = 'R0'
      Layout = tlCenter
    end
    object LabelR1: TLabel
      Left = 32
      Top = 42
      Width = 25
      Height = 21
      Hint = 'Horizontal Displayed'
      AutoSize = False
      Caption = 'R1'
      Layout = tlCenter
    end
    object LabelR17: TLabel
      Left = 136
      Top = 210
      Width = 33
      Height = 21
      Hint = 'Light Pen (LSB)'
      AutoSize = False
      Caption = 'R17'
      Layout = tlCenter
    end
    object LabelR2: TLabel
      Left = 32
      Top = 66
      Width = 25
      Height = 21
      Hint = 'Horizontal Sync Position'
      AutoSize = False
      Caption = 'R2'
      Layout = tlCenter
    end
    object LabelR9: TLabel
      Left = 136
      Top = 18
      Width = 33
      Height = 21
      Hint = 'Maximum Raster Address'
      AutoSize = False
      Caption = 'R9'
      Layout = tlCenter
    end
    object LabelR10: TLabel
      Left = 136
      Top = 42
      Width = 33
      Height = 21
      Hint = 'Cursor Start Raster'
      AutoSize = False
      Caption = 'R10'
      Layout = tlCenter
    end
    object LabelR4: TLabel
      Left = 32
      Top = 114
      Width = 25
      Height = 21
      Hint = 'Vertical Total'
      AutoSize = False
      Caption = 'R4'
      Layout = tlCenter
    end
    object LabelR5: TLabel
      Left = 32
      Top = 138
      Width = 25
      Height = 21
      Hint = 'Vertical Total Adjust'
      AutoSize = False
      Caption = 'R5'
      Layout = tlCenter
    end
    object LabelR6: TLabel
      Left = 32
      Top = 162
      Width = 25
      Height = 21
      Hint = 'Vertical Displayed'
      AutoSize = False
      Caption = 'R6'
      Layout = tlCenter
    end
    object LabelR7: TLabel
      Left = 32
      Top = 186
      Width = 25
      Height = 21
      Hint = 'Vertical Sync Position'
      AutoSize = False
      Caption = 'R7'
      Layout = tlCenter
    end
    object LabelR8: TLabel
      Left = 32
      Top = 210
      Width = 25
      Height = 21
      Hint = 'Interlace & Skew'
      AutoSize = False
      Caption = 'R8'
      Layout = tlCenter
    end
    object LabelR11: TLabel
      Left = 136
      Top = 66
      Width = 33
      Height = 21
      Hint = 'Cursor End Raster'
      AutoSize = False
      Caption = 'R11'
      Layout = tlCenter
    end
    object LabelR3: TLabel
      Left = 32
      Top = 90
      Width = 25
      Height = 21
      Hint = 'Sync Width'
      AutoSize = False
      Caption = 'R3'
      Layout = tlCenter
    end
    object LabelR12: TLabel
      Left = 136
      Top = 90
      Width = 33
      Height = 21
      Hint = 'Start Address (MSB)'
      AutoSize = False
      Caption = 'R12'
      Layout = tlCenter
    end
    object LabelR13: TLabel
      Left = 136
      Top = 114
      Width = 33
      Height = 21
      Hint = 'Start Address (LSB)'
      AutoSize = False
      Caption = 'R13'
      Layout = tlCenter
    end
    object LabelR14: TLabel
      Left = 136
      Top = 138
      Width = 33
      Height = 21
      Hint = 'Cursor (MSB)'
      AutoSize = False
      Caption = 'R14'
      Layout = tlCenter
    end
    object LabelR15: TLabel
      Left = 136
      Top = 162
      Width = 33
      Height = 21
      Hint = 'Cursor (LSB)'
      AutoSize = False
      Caption = 'R15'
      Layout = tlCenter
    end
    object LabelR16: TLabel
      Left = 136
      Top = 186
      Width = 33
      Height = 21
      Hint = 'Light Pen (MSB)'
      AutoSize = False
      Caption = 'R16'
      Layout = tlCenter
    end
    object RadioButtonR17: TRadioButton
      Left = 120
      Top = 209
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 0
    end
    object RadioButtonR0: TRadioButton
      Left = 16
      Top = 17
      Width = 17
      Height = 21
      Checked = True
      Enabled = False
      TabOrder = 1
      TabStop = True
    end
    object RadioButtonR1: TRadioButton
      Left = 16
      Top = 41
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 2
    end
    object RadioButtonR2: TRadioButton
      Left = 16
      Top = 65
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 3
    end
    object RadioButtonR9: TRadioButton
      Left = 120
      Top = 17
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 4
    end
    object RadioButtonR10: TRadioButton
      Left = 120
      Top = 41
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 5
    end
    object RadioButtonR4: TRadioButton
      Left = 16
      Top = 113
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 6
    end
    object RadioButtonR5: TRadioButton
      Left = 16
      Top = 137
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 7
    end
    object RadioButtonR6: TRadioButton
      Left = 16
      Top = 161
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 8
    end
    object RadioButtonR7: TRadioButton
      Left = 16
      Top = 185
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 9
    end
    object RadioButtonR8: TRadioButton
      Left = 16
      Top = 209
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 10
    end
    object RadioButtonR11: TRadioButton
      Left = 120
      Top = 65
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 11
    end
    object RadioButtonR3: TRadioButton
      Left = 16
      Top = 89
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 12
    end
    object RadioButtonR12: TRadioButton
      Left = 120
      Top = 89
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 13
    end
    object RadioButtonR13: TRadioButton
      Left = 120
      Top = 113
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 14
    end
    object RadioButtonR14: TRadioButton
      Left = 120
      Top = 137
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 15
    end
    object RadioButtonR15: TRadioButton
      Left = 120
      Top = 161
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 16
    end
    object RadioButtonR16: TRadioButton
      Left = 120
      Top = 185
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 17
    end
    object PanelFormatR0: TPanel
      Left = 56
      Top = 18
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 18
      OnClick = PanelFormatClick
      OnDblClick = PanelFormatClick
    end
    object PanelFormatR1: TPanel
      Tag = 1
      Left = 56
      Top = 42
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 19
      OnClick = PanelFormatClick
      OnDblClick = PanelFormatClick
    end
    object PanelFormatR2: TPanel
      Tag = 2
      Left = 56
      Top = 66
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 20
      OnClick = PanelFormatClick
      OnDblClick = PanelFormatClick
    end
    object PanelFormatR3: TPanel
      Tag = 3
      Left = 56
      Top = 90
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 21
      OnClick = PanelFormatClick
      OnDblClick = PanelFormatClick
    end
    object PanelFormatR4: TPanel
      Tag = 4
      Left = 56
      Top = 114
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 22
      OnClick = PanelFormatClick
      OnDblClick = PanelFormatClick
    end
    object PanelFormatR5: TPanel
      Tag = 5
      Left = 56
      Top = 138
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 23
      OnClick = PanelFormatClick
      OnDblClick = PanelFormatClick
    end
    object PanelFormatR6: TPanel
      Tag = 6
      Left = 56
      Top = 162
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 24
      OnClick = PanelFormatClick
      OnDblClick = PanelFormatClick
    end
    object PanelFormatR7: TPanel
      Tag = 7
      Left = 56
      Top = 186
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 25
      OnClick = PanelFormatClick
      OnDblClick = PanelFormatClick
    end
    object PanelFormatR8: TPanel
      Tag = 8
      Left = 56
      Top = 210
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 26
      OnClick = PanelFormatClick
      OnDblClick = PanelFormatClick
    end
    object PanelFormatR9: TPanel
      Tag = 9
      Left = 168
      Top = 18
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 27
      OnClick = PanelFormatClick
      OnDblClick = PanelFormatClick
    end
    object PanelFormatR10: TPanel
      Tag = 10
      Left = 168
      Top = 42
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 28
      OnClick = PanelFormatClick
      OnDblClick = PanelFormatClick
    end
    object PanelFormatR11: TPanel
      Tag = 11
      Left = 168
      Top = 66
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 29
      OnClick = PanelFormatClick
      OnDblClick = PanelFormatClick
    end
    object PanelFormatR12: TPanel
      Tag = 12
      Left = 168
      Top = 90
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 30
      OnClick = PanelFormatClick
      OnDblClick = PanelFormatClick
    end
    object PanelFormatR13: TPanel
      Tag = 13
      Left = 168
      Top = 114
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 31
      OnClick = PanelFormatClick
      OnDblClick = PanelFormatClick
    end
    object PanelFormatR14: TPanel
      Tag = 14
      Left = 168
      Top = 138
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 32
      OnClick = PanelFormatClick
      OnDblClick = PanelFormatClick
    end
    object PanelFormatR15: TPanel
      Tag = 15
      Left = 168
      Top = 162
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 33
      OnClick = PanelFormatClick
      OnDblClick = PanelFormatClick
    end
    object PanelFormatR16: TPanel
      Tag = 16
      Left = 168
      Top = 186
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 34
      OnClick = PanelFormatClick
      OnDblClick = PanelFormatClick
    end
    object PanelFormatR17: TPanel
      Tag = 17
      Left = 168
      Top = 210
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 35
      OnClick = PanelFormatClick
      OnDblClick = PanelFormatClick
    end
    object PanelR0: TPanel
      Left = 73
      Top = 18
      Width = 33
      Height = 21
      Hint = 'Horizontal Total'
      Color = clWhite
      ParentBackground = False
      TabOrder = 36
      OnClick = PanelRegClick
      OnDblClick = PanelRegClick
    end
    object PanelR1: TPanel
      Tag = 1
      Left = 73
      Top = 42
      Width = 33
      Height = 21
      Hint = 'Horizontal Displayed'
      Color = clWhite
      ParentBackground = False
      TabOrder = 37
      OnClick = PanelRegClick
      OnDblClick = PanelRegClick
    end
    object PanelR2: TPanel
      Tag = 2
      Left = 73
      Top = 66
      Width = 33
      Height = 21
      Hint = 'Horizontal Sync Position'
      Color = clWhite
      ParentBackground = False
      TabOrder = 38
      OnClick = PanelRegClick
      OnDblClick = PanelRegClick
    end
    object PanelR3: TPanel
      Tag = 3
      Left = 73
      Top = 90
      Width = 33
      Height = 21
      Hint = 'Sync Width'
      Color = clWhite
      ParentBackground = False
      TabOrder = 39
      OnClick = PanelRegClick
      OnDblClick = PanelRegClick
    end
    object PanelR4: TPanel
      Tag = 4
      Left = 73
      Top = 114
      Width = 33
      Height = 21
      Hint = 'Vertical Total'
      Color = clWhite
      ParentBackground = False
      TabOrder = 40
      OnClick = PanelRegClick
      OnDblClick = PanelRegClick
    end
    object PanelR5: TPanel
      Tag = 5
      Left = 73
      Top = 138
      Width = 33
      Height = 21
      Hint = 'Vertical Total Adjust'
      Color = clWhite
      ParentBackground = False
      TabOrder = 41
      OnClick = PanelRegClick
      OnDblClick = PanelRegClick
    end
    object PanelR6: TPanel
      Tag = 6
      Left = 73
      Top = 162
      Width = 33
      Height = 21
      Hint = 'Vertical Displayed'
      Color = clWhite
      ParentBackground = False
      TabOrder = 42
      OnClick = PanelRegClick
      OnDblClick = PanelRegClick
    end
    object PanelR7: TPanel
      Tag = 7
      Left = 73
      Top = 186
      Width = 33
      Height = 21
      Hint = 'Vertical Sync Position'
      Color = clWhite
      ParentBackground = False
      TabOrder = 43
      OnClick = PanelRegClick
      OnDblClick = PanelRegClick
    end
    object PanelR8: TPanel
      Tag = 8
      Left = 73
      Top = 210
      Width = 33
      Height = 21
      Hint = 'Interlace & Skew'
      Color = clWhite
      ParentBackground = False
      TabOrder = 44
      OnClick = PanelRegClick
      OnDblClick = PanelRegClick
    end
    object PanelR9: TPanel
      Tag = 9
      Left = 185
      Top = 18
      Width = 33
      Height = 21
      Hint = 'Maximum Raster Address'
      Color = clWhite
      ParentBackground = False
      TabOrder = 45
      OnClick = PanelRegClick
      OnDblClick = PanelRegClick
    end
    object PanelR10: TPanel
      Tag = 10
      Left = 185
      Top = 42
      Width = 33
      Height = 21
      Hint = 'Cursor Start Raster'
      Color = clWhite
      ParentBackground = False
      TabOrder = 46
      OnClick = PanelRegClick
      OnDblClick = PanelRegClick
    end
    object PanelR11: TPanel
      Tag = 11
      Left = 185
      Top = 66
      Width = 33
      Height = 21
      Hint = 'Cursor End Raster'
      Color = clWhite
      ParentBackground = False
      TabOrder = 47
      OnClick = PanelRegClick
      OnDblClick = PanelRegClick
    end
    object PanelR12: TPanel
      Tag = 12
      Left = 185
      Top = 90
      Width = 33
      Height = 21
      Hint = 'Start Address (MSB)'
      Color = clWhite
      ParentBackground = False
      TabOrder = 48
      OnClick = PanelRegClick
      OnDblClick = PanelRegClick
    end
    object PanelR13: TPanel
      Tag = 13
      Left = 185
      Top = 114
      Width = 33
      Height = 21
      Hint = 'Start Address (LSB)'
      Color = clWhite
      ParentBackground = False
      TabOrder = 49
      OnClick = PanelRegClick
      OnDblClick = PanelRegClick
    end
    object PanelR14: TPanel
      Tag = 14
      Left = 185
      Top = 138
      Width = 33
      Height = 21
      Hint = 'Cursor (MSB)'
      Color = clWhite
      ParentBackground = False
      TabOrder = 50
      OnClick = PanelRegClick
      OnDblClick = PanelRegClick
    end
    object PanelR15: TPanel
      Tag = 15
      Left = 185
      Top = 162
      Width = 33
      Height = 21
      Hint = 'Cursor (LSB)'
      Color = clWhite
      ParentBackground = False
      TabOrder = 51
      OnClick = PanelRegClick
      OnDblClick = PanelRegClick
    end
    object PanelR16: TPanel
      Tag = 16
      Left = 185
      Top = 186
      Width = 33
      Height = 21
      Hint = 'Light Pen (MSB)'
      Color = clWhite
      ParentBackground = False
      TabOrder = 52
      OnClick = PanelRegClick
      OnDblClick = PanelRegClick
    end
    object PanelR17: TPanel
      Tag = 17
      Left = 185
      Top = 210
      Width = 33
      Height = 21
      Hint = 'Light Pen (LSB)'
      Color = clWhite
      ParentBackground = False
      TabOrder = 53
      OnClick = PanelRegClick
      OnDblClick = PanelRegClick
    end
  end
  object GroupBoxStatus: TGroupBox
    Left = 248
    Top = 8
    Width = 169
    Height = 145
    Caption = ' Status '
    TabOrder = 1
    object LabelVBL: TLabel
      Left = 24
      Top = 18
      Width = 33
      Height = 13
      AutoSize = False
      Caption = 'VBL'
      Layout = tlCenter
    end
    object LabelHBL: TLabel
      Left = 80
      Top = 18
      Width = 33
      Height = 13
      AutoSize = False
      Caption = 'HBL'
      Layout = tlCenter
    end
    object LabelCharCount: TLabel
      Left = 8
      Top = 56
      Width = 65
      Height = 17
      AutoSize = False
      Caption = 'Char'
      Layout = tlCenter
    end
    object LabelLineCount: TLabel
      Left = 8
      Top = 80
      Width = 65
      Height = 17
      AutoSize = False
      Caption = 'Line'
      Layout = tlCenter
    end
    object LabelRasterCount: TLabel
      Left = 8
      Top = 104
      Width = 65
      Height = 17
      AutoSize = False
      Caption = 'Raster'
      Layout = tlCenter
    end
    object CheckBoxVBL: TCheckBox
      Left = 8
      Top = 16
      Width = 15
      Height = 17
      Enabled = False
      TabOrder = 0
    end
    object CheckBoxHBL: TCheckBox
      Left = 64
      Top = 16
      Width = 15
      Height = 17
      Enabled = False
      TabOrder = 1
    end
    object PanelCharCount: TPanel
      Left = 72
      Top = 56
      Width = 89
      Height = 21
      ParentBackground = False
      ParentColor = True
      TabOrder = 2
    end
    object PanelLineCount: TPanel
      Left = 72
      Top = 80
      Width = 89
      Height = 21
      ParentBackground = False
      ParentColor = True
      TabOrder = 3
    end
    object PanelRasterCount: TPanel
      Left = 72
      Top = 104
      Width = 89
      Height = 21
      ParentBackground = False
      ParentColor = True
      TabOrder = 4
    end
  end
  object GroupBoxPosition: TGroupBox
    Left = 248
    Top = 168
    Width = 169
    Height = 81
    Caption = ' Beam Position '
    TabOrder = 2
    object LabelBeamPosX: TLabel
      Left = 8
      Top = 20
      Width = 65
      Height = 21
      AutoSize = False
      Caption = 'Screen X'
      Layout = tlCenter
    end
    object LabelBeamPosY: TLabel
      Left = 8
      Top = 44
      Width = 65
      Height = 21
      AutoSize = False
      Caption = 'Screen Y'
      Layout = tlCenter
    end
    object PanelBeamPosX: TPanel
      Left = 96
      Top = 20
      Width = 65
      Height = 21
      Hint = 'Position in pixels from Left of emulator screen'
      ParentBackground = False
      ParentColor = True
      TabOrder = 0
    end
    object PanelBeamPosY: TPanel
      Left = 96
      Top = 44
      Width = 65
      Height = 21
      Hint = 'Position in pixels from Top of emulator screen'
      ParentBackground = False
      ParentColor = True
      TabOrder = 1
    end
  end
end
