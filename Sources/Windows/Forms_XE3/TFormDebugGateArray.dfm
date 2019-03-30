object FormDebugGateArray: TFormDebugGateArray
  Left = 870
  Top = 253
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Gate Array'
  ClientHeight = 393
  ClientWidth = 625
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnHide = FormHide
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBoxPalette: TGroupBox
    Left = 8
    Top = 144
    Width = 609
    Height = 241
    Caption = ' Palette '
    TabOrder = 0
    object LabelInk0: TLabel
      Left = 32
      Top = 42
      Width = 49
      Height = 21
      AutoSize = False
      Caption = 'Ink 0'
      Layout = tlCenter
    end
    object LabelInkBorder: TLabel
      Left = 32
      Top = 18
      Width = 49
      Height = 21
      AutoSize = False
      Caption = 'Border'
      Layout = tlCenter
    end
    object LabelInk1: TLabel
      Left = 32
      Top = 66
      Width = 49
      Height = 21
      AutoSize = False
      Caption = 'Ink 1'
      Layout = tlCenter
    end
    object LabelInk2: TLabel
      Left = 32
      Top = 90
      Width = 49
      Height = 21
      AutoSize = False
      Caption = 'Ink 2'
      Layout = tlCenter
    end
    object LabelInk3: TLabel
      Left = 32
      Top = 114
      Width = 49
      Height = 21
      AutoSize = False
      Caption = 'Ink 3'
      Layout = tlCenter
    end
    object LabelInk4: TLabel
      Left = 32
      Top = 138
      Width = 49
      Height = 21
      AutoSize = False
      Caption = 'Ink 4'
      Layout = tlCenter
    end
    object LabelInk5: TLabel
      Left = 32
      Top = 162
      Width = 49
      Height = 21
      AutoSize = False
      Caption = 'Ink 5'
      Layout = tlCenter
    end
    object LabelInk6: TLabel
      Left = 32
      Top = 186
      Width = 49
      Height = 21
      AutoSize = False
      Caption = 'Ink 6'
      Layout = tlCenter
    end
    object LabelInk7: TLabel
      Left = 32
      Top = 210
      Width = 49
      Height = 21
      AutoSize = False
      Caption = 'Ink 7'
      Layout = tlCenter
    end
    object LabelInk8: TLabel
      Left = 328
      Top = 42
      Width = 49
      Height = 21
      AutoSize = False
      Caption = 'Ink 8'
      Layout = tlCenter
    end
    object LabelInk9: TLabel
      Left = 328
      Top = 66
      Width = 49
      Height = 21
      AutoSize = False
      Caption = 'Ink 9'
      Layout = tlCenter
    end
    object LabelInk10: TLabel
      Left = 328
      Top = 90
      Width = 49
      Height = 21
      AutoSize = False
      Caption = 'Ink 10'
      Layout = tlCenter
    end
    object LabelInk11: TLabel
      Left = 328
      Top = 114
      Width = 49
      Height = 21
      AutoSize = False
      Caption = 'Ink 11'
      Layout = tlCenter
    end
    object LabelInk12: TLabel
      Left = 328
      Top = 138
      Width = 49
      Height = 21
      AutoSize = False
      Caption = 'Ink 12'
      Layout = tlCenter
    end
    object LabelInk13: TLabel
      Left = 328
      Top = 162
      Width = 49
      Height = 21
      AutoSize = False
      Caption = 'Ink 13'
      Layout = tlCenter
    end
    object LabelInk14: TLabel
      Left = 328
      Top = 186
      Width = 49
      Height = 21
      AutoSize = False
      Caption = 'Ink 14'
      Layout = tlCenter
    end
    object LabelInk15: TLabel
      Left = 328
      Top = 210
      Width = 49
      Height = 21
      AutoSize = False
      Caption = 'Ink 15'
      Layout = tlCenter
    end
    object RadioButtonInk0: TRadioButton
      Left = 16
      Top = 42
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 0
    end
    object PanelInkColor0: TPanel
      Left = 153
      Top = 42
      Width = 33
      Height = 21
      Color = clWhite
      ParentBackground = False
      TabOrder = 1
      OnClick = PanelInkColorClick
    end
    object PanelInk0: TPanel
      Left = 88
      Top = 42
      Width = 57
      Height = 21
      Hint = 'BASIC color (GateArray Color)'
      Color = clWhite
      ParentBackground = False
      TabOrder = 2
      OnClick = PanelInkClick
    end
    object RadioButtonInkBorder: TRadioButton
      Left = 16
      Top = 18
      Width = 17
      Height = 21
      Checked = True
      Enabled = False
      TabOrder = 3
      TabStop = True
    end
    object PanelInkBorder: TPanel
      Tag = 16
      Left = 88
      Top = 18
      Width = 57
      Height = 21
      Hint = 'BASIC color (GateArray Color)'
      Color = clWhite
      ParentBackground = False
      TabOrder = 4
      OnClick = PanelInkClick
    end
    object PanelInkColorBorder: TPanel
      Tag = 16
      Left = 153
      Top = 18
      Width = 33
      Height = 21
      Color = clWhite
      ParentBackground = False
      TabOrder = 5
      OnClick = PanelInkColorClick
    end
    object RadioButtonInk1: TRadioButton
      Left = 16
      Top = 66
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 6
    end
    object PanelInk1: TPanel
      Tag = 1
      Left = 88
      Top = 66
      Width = 57
      Height = 21
      Hint = 'BASIC color (GateArray Color)'
      Color = clWhite
      ParentBackground = False
      TabOrder = 7
      OnClick = PanelInkClick
    end
    object PanelInkColor1: TPanel
      Tag = 1
      Left = 153
      Top = 66
      Width = 33
      Height = 21
      Color = clWhite
      ParentBackground = False
      TabOrder = 8
      OnClick = PanelInkColorClick
    end
    object RadioButtonInk2: TRadioButton
      Left = 16
      Top = 90
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 9
    end
    object PanelInk2: TPanel
      Tag = 2
      Left = 88
      Top = 90
      Width = 57
      Height = 21
      Hint = 'BASIC color (GateArray Color)'
      Color = clWhite
      ParentBackground = False
      TabOrder = 10
      OnClick = PanelInkClick
    end
    object PanelInkColor2: TPanel
      Tag = 2
      Left = 153
      Top = 90
      Width = 33
      Height = 21
      Color = clWhite
      ParentBackground = False
      TabOrder = 11
      OnClick = PanelInkColorClick
    end
    object RadioButtonInk3: TRadioButton
      Left = 16
      Top = 114
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 12
    end
    object PanelInk3: TPanel
      Tag = 3
      Left = 88
      Top = 114
      Width = 57
      Height = 21
      Hint = 'BASIC color (GateArray Color)'
      Color = clWhite
      ParentBackground = False
      TabOrder = 13
      OnClick = PanelInkClick
    end
    object PanelInkColor3: TPanel
      Tag = 3
      Left = 153
      Top = 114
      Width = 33
      Height = 21
      Color = clWhite
      ParentBackground = False
      TabOrder = 14
      OnClick = PanelInkColorClick
    end
    object PanelInkName0: TPanel
      Left = 192
      Top = 42
      Width = 113
      Height = 21
      Alignment = taLeftJustify
      BevelOuter = bvNone
      ParentColor = True
      TabOrder = 15
      OnClick = PanelInkColorClick
    end
    object PanelInkName1: TPanel
      Tag = 1
      Left = 192
      Top = 66
      Width = 113
      Height = 21
      Alignment = taLeftJustify
      BevelOuter = bvNone
      ParentColor = True
      TabOrder = 16
      OnClick = PanelInkColorClick
    end
    object PanelInkName2: TPanel
      Tag = 2
      Left = 192
      Top = 90
      Width = 113
      Height = 21
      Alignment = taLeftJustify
      BevelOuter = bvNone
      ParentColor = True
      TabOrder = 17
      OnClick = PanelInkColorClick
    end
    object PanelInkName3: TPanel
      Tag = 3
      Left = 192
      Top = 114
      Width = 113
      Height = 21
      Alignment = taLeftJustify
      BevelOuter = bvNone
      ParentColor = True
      TabOrder = 18
      OnClick = PanelInkColorClick
    end
    object PanelInkNameBorder: TPanel
      Tag = 16
      Left = 192
      Top = 18
      Width = 113
      Height = 21
      Alignment = taLeftJustify
      BevelOuter = bvNone
      ParentColor = True
      TabOrder = 19
      OnClick = PanelInkColorClick
    end
    object RadioButtonInk4: TRadioButton
      Left = 16
      Top = 138
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 20
    end
    object PanelInk4: TPanel
      Tag = 4
      Left = 88
      Top = 138
      Width = 57
      Height = 21
      Hint = 'BASIC color (GateArray Color)'
      Color = clWhite
      ParentBackground = False
      TabOrder = 21
      OnClick = PanelInkClick
    end
    object PanelInkColor4: TPanel
      Tag = 4
      Left = 153
      Top = 138
      Width = 33
      Height = 21
      Color = clWhite
      ParentBackground = False
      TabOrder = 22
      OnClick = PanelInkColorClick
    end
    object PanelInkName4: TPanel
      Tag = 4
      Left = 192
      Top = 138
      Width = 113
      Height = 21
      Alignment = taLeftJustify
      BevelOuter = bvNone
      ParentColor = True
      TabOrder = 23
      OnClick = PanelInkColorClick
    end
    object RadioButtonInk5: TRadioButton
      Left = 16
      Top = 162
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 24
    end
    object PanelInk5: TPanel
      Tag = 5
      Left = 88
      Top = 162
      Width = 57
      Height = 21
      Hint = 'BASIC color (GateArray Color)'
      Color = clWhite
      ParentBackground = False
      TabOrder = 25
      OnClick = PanelInkClick
    end
    object PanelInkColor5: TPanel
      Tag = 5
      Left = 153
      Top = 162
      Width = 33
      Height = 21
      Color = clWhite
      ParentBackground = False
      TabOrder = 26
      OnClick = PanelInkColorClick
    end
    object PanelInkName5: TPanel
      Tag = 5
      Left = 192
      Top = 162
      Width = 113
      Height = 21
      Alignment = taLeftJustify
      BevelOuter = bvNone
      ParentColor = True
      TabOrder = 27
      OnClick = PanelInkColorClick
    end
    object RadioButtonInk6: TRadioButton
      Left = 16
      Top = 186
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 28
    end
    object PanelInk6: TPanel
      Tag = 6
      Left = 88
      Top = 186
      Width = 57
      Height = 21
      Hint = 'BASIC color (GateArray Color)'
      Color = clWhite
      ParentBackground = False
      TabOrder = 29
      OnClick = PanelInkClick
    end
    object PanelInkColor6: TPanel
      Tag = 6
      Left = 153
      Top = 186
      Width = 33
      Height = 21
      Color = clWhite
      ParentBackground = False
      TabOrder = 30
      OnClick = PanelInkColorClick
    end
    object PanelInkName6: TPanel
      Tag = 6
      Left = 192
      Top = 186
      Width = 113
      Height = 21
      Alignment = taLeftJustify
      BevelOuter = bvNone
      ParentColor = True
      TabOrder = 31
      OnClick = PanelInkColorClick
    end
    object RadioButtonInk7: TRadioButton
      Left = 16
      Top = 210
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 32
    end
    object PanelInk7: TPanel
      Tag = 7
      Left = 88
      Top = 210
      Width = 57
      Height = 21
      Hint = 'BASIC color (GateArray Color)'
      Color = clWhite
      ParentBackground = False
      TabOrder = 33
      OnClick = PanelInkClick
    end
    object PanelInkColor7: TPanel
      Tag = 7
      Left = 153
      Top = 210
      Width = 33
      Height = 21
      Color = clWhite
      ParentBackground = False
      TabOrder = 34
      OnClick = PanelInkColorClick
    end
    object PanelInkName7: TPanel
      Tag = 7
      Left = 192
      Top = 210
      Width = 113
      Height = 21
      Alignment = taLeftJustify
      BevelOuter = bvNone
      ParentColor = True
      TabOrder = 35
      OnClick = PanelInkColorClick
    end
    object RadioButtonInk8: TRadioButton
      Left = 312
      Top = 42
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 36
    end
    object PanelInk8: TPanel
      Tag = 8
      Left = 384
      Top = 42
      Width = 57
      Height = 21
      Hint = 'BASIC color (GateArray Color)'
      Color = clWhite
      ParentBackground = False
      TabOrder = 37
      OnClick = PanelInkClick
    end
    object PanelInkColor8: TPanel
      Tag = 8
      Left = 449
      Top = 42
      Width = 33
      Height = 21
      Color = clWhite
      ParentBackground = False
      TabOrder = 38
      OnClick = PanelInkColorClick
    end
    object PanelInkName8: TPanel
      Tag = 8
      Left = 488
      Top = 42
      Width = 113
      Height = 21
      Alignment = taLeftJustify
      BevelOuter = bvNone
      ParentColor = True
      TabOrder = 39
      OnClick = PanelInkColorClick
    end
    object RadioButtonInk9: TRadioButton
      Left = 312
      Top = 66
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 40
    end
    object PanelInk9: TPanel
      Tag = 9
      Left = 384
      Top = 66
      Width = 57
      Height = 21
      Hint = 'BASIC color (GateArray Color)'
      Color = clWhite
      ParentBackground = False
      TabOrder = 41
      OnClick = PanelInkClick
    end
    object PanelInkColor9: TPanel
      Tag = 9
      Left = 449
      Top = 66
      Width = 33
      Height = 21
      Color = clWhite
      ParentBackground = False
      TabOrder = 42
      OnClick = PanelInkColorClick
    end
    object PanelInkName9: TPanel
      Tag = 9
      Left = 488
      Top = 66
      Width = 113
      Height = 21
      Alignment = taLeftJustify
      BevelOuter = bvNone
      ParentColor = True
      TabOrder = 43
      OnClick = PanelInkColorClick
    end
    object RadioButtonInk10: TRadioButton
      Left = 312
      Top = 90
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 44
    end
    object PanelInk10: TPanel
      Tag = 10
      Left = 384
      Top = 90
      Width = 57
      Height = 21
      Hint = 'BASIC color (GateArray Color)'
      Color = clWhite
      ParentBackground = False
      TabOrder = 45
      OnClick = PanelInkClick
    end
    object PanelInkColor10: TPanel
      Tag = 10
      Left = 449
      Top = 90
      Width = 33
      Height = 21
      Color = clWhite
      ParentBackground = False
      TabOrder = 46
      OnClick = PanelInkColorClick
    end
    object PanelInkName10: TPanel
      Tag = 10
      Left = 488
      Top = 90
      Width = 113
      Height = 21
      Alignment = taLeftJustify
      BevelOuter = bvNone
      ParentColor = True
      TabOrder = 47
      OnClick = PanelInkColorClick
    end
    object RadioButtonInk11: TRadioButton
      Left = 312
      Top = 114
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 48
    end
    object PanelInk11: TPanel
      Tag = 11
      Left = 384
      Top = 114
      Width = 57
      Height = 21
      Hint = 'BASIC color (GateArray Color)'
      Color = clWhite
      ParentBackground = False
      TabOrder = 49
      OnClick = PanelInkClick
    end
    object PanelInkColor11: TPanel
      Tag = 11
      Left = 449
      Top = 114
      Width = 33
      Height = 21
      Color = clWhite
      ParentBackground = False
      TabOrder = 50
      OnClick = PanelInkColorClick
    end
    object PanelInkName11: TPanel
      Tag = 11
      Left = 488
      Top = 114
      Width = 113
      Height = 21
      Alignment = taLeftJustify
      BevelOuter = bvNone
      ParentColor = True
      TabOrder = 51
      OnClick = PanelInkColorClick
    end
    object RadioButtonInk12: TRadioButton
      Left = 312
      Top = 138
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 52
    end
    object PanelInk12: TPanel
      Tag = 12
      Left = 384
      Top = 138
      Width = 57
      Height = 21
      Hint = 'BASIC color (GateArray Color)'
      Color = clWhite
      ParentBackground = False
      TabOrder = 53
      OnClick = PanelInkClick
    end
    object PanelInkColor12: TPanel
      Tag = 12
      Left = 449
      Top = 138
      Width = 33
      Height = 21
      Color = clWhite
      ParentBackground = False
      TabOrder = 54
      OnClick = PanelInkColorClick
    end
    object PanelInkName12: TPanel
      Tag = 12
      Left = 488
      Top = 138
      Width = 113
      Height = 21
      Alignment = taLeftJustify
      BevelOuter = bvNone
      ParentColor = True
      TabOrder = 55
      OnClick = PanelInkColorClick
    end
    object RadioButtonInk13: TRadioButton
      Left = 312
      Top = 162
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 56
    end
    object PanelInk13: TPanel
      Tag = 13
      Left = 384
      Top = 162
      Width = 57
      Height = 21
      Hint = 'BASIC color (GateArray Color)'
      Color = clWhite
      ParentBackground = False
      TabOrder = 57
      OnClick = PanelInkClick
    end
    object PanelInkColor13: TPanel
      Tag = 13
      Left = 449
      Top = 162
      Width = 33
      Height = 21
      Color = clWhite
      ParentBackground = False
      TabOrder = 58
      OnClick = PanelInkColorClick
    end
    object PanelInkName13: TPanel
      Tag = 13
      Left = 488
      Top = 162
      Width = 113
      Height = 21
      Alignment = taLeftJustify
      BevelOuter = bvNone
      ParentColor = True
      TabOrder = 59
      OnClick = PanelInkColorClick
    end
    object RadioButtonInk14: TRadioButton
      Left = 312
      Top = 186
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 60
    end
    object PanelInk14: TPanel
      Tag = 14
      Left = 384
      Top = 186
      Width = 57
      Height = 21
      Hint = 'BASIC color (GateArray Color)'
      Color = clWhite
      ParentBackground = False
      TabOrder = 61
      OnClick = PanelInkClick
    end
    object PanelInkColor14: TPanel
      Tag = 14
      Left = 449
      Top = 186
      Width = 33
      Height = 21
      Color = clWhite
      ParentBackground = False
      TabOrder = 62
      OnClick = PanelInkColorClick
    end
    object PanelInkName14: TPanel
      Tag = 14
      Left = 488
      Top = 186
      Width = 113
      Height = 21
      Alignment = taLeftJustify
      BevelOuter = bvNone
      ParentColor = True
      TabOrder = 63
      OnClick = PanelInkColorClick
    end
    object RadioButtonInk15: TRadioButton
      Left = 312
      Top = 210
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 64
    end
    object PanelInk15: TPanel
      Tag = 15
      Left = 384
      Top = 210
      Width = 57
      Height = 21
      Hint = 'BASIC color (GateArray Color)'
      Color = clWhite
      ParentBackground = False
      TabOrder = 65
      OnClick = PanelInkClick
    end
    object PanelInkColor15: TPanel
      Tag = 15
      Left = 449
      Top = 210
      Width = 33
      Height = 21
      Color = clWhite
      ParentBackground = False
      TabOrder = 66
      OnClick = PanelInkColorClick
    end
    object PanelInkName15: TPanel
      Tag = 15
      Left = 488
      Top = 210
      Width = 113
      Height = 21
      Alignment = taLeftJustify
      BevelOuter = bvNone
      ParentColor = True
      TabOrder = 67
      OnClick = PanelInkClick
    end
  end
  object GroupBoxModes: TGroupBox
    Left = 8
    Top = 8
    Width = 153
    Height = 73
    Caption = ' Screen Mode '
    TabOrder = 1
    object LabelCurrentMode: TLabel
      Left = 16
      Top = 18
      Width = 73
      Height = 21
      AutoSize = False
      Caption = 'Current'
      Layout = tlCenter
    end
    object LabelRequestedMode: TLabel
      Left = 16
      Top = 42
      Width = 81
      Height = 21
      AutoSize = False
      Caption = 'Requested'
      Layout = tlCenter
    end
    object PanelCurrentMode: TPanel
      Left = 104
      Top = 18
      Width = 42
      Height = 21
      ParentBackground = False
      ParentColor = True
      TabOrder = 0
    end
    object PanelRequestedMode: TPanel
      Left = 104
      Top = 42
      Width = 42
      Height = 21
      Color = clWhite
      ParentBackground = False
      TabOrder = 1
      OnClick = PanelRequestedModeClick
    end
  end
  object GroupBoxRAMState: TGroupBox
    Left = 8
    Top = 88
    Width = 153
    Height = 49
    Caption = ' RAM settings '
    TabOrder = 2
    object LabelStateRAMconfig: TLabel
      Left = 8
      Top = 18
      Width = 57
      Height = 21
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'RAM config'
      Layout = tlCenter
    end
    object PanelRAMConfig: TPanel
      Left = 105
      Top = 18
      Width = 40
      Height = 21
      ParentBackground = False
      ParentColor = True
      TabOrder = 0
    end
  end
  object GroupBoxROMState: TGroupBox
    Left = 168
    Top = 8
    Width = 161
    Height = 129
    Caption = 'ROM settings'
    TabOrder = 3
    object LabelStateROMConfig: TLabel
      Left = 8
      Top = 21
      Width = 89
      Height = 17
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'ROM config'
    end
    object LabelUpperROM: TLabel
      Left = 8
      Top = 45
      Width = 89
      Height = 17
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Upper ROM'
    end
    object LabelLowerROMEnabled: TLabel
      Left = 8
      Top = 78
      Width = 89
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Lower ROM'
      Layout = tlCenter
    end
    object LabelUpperROMEnabled: TLabel
      Left = 8
      Top = 102
      Width = 89
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Upper ROM'
      Layout = tlCenter
    end
    object CheckBoxLowerROMEnabled: TCheckBox
      Left = 120
      Top = 77
      Width = 17
      Height = 17
      Enabled = False
      TabOrder = 0
    end
    object CheckBoxUpperROMEnabled: TCheckBox
      Left = 120
      Top = 101
      Width = 17
      Height = 17
      Enabled = False
      TabOrder = 1
    end
    object PanelROMConfig: TPanel
      Left = 105
      Top = 18
      Width = 48
      Height = 21
      ParentBackground = False
      ParentColor = True
      TabOrder = 2
    end
    object PanelUpperROM: TPanel
      Left = 105
      Top = 42
      Width = 48
      Height = 21
      ParentBackground = False
      ParentColor = True
      TabOrder = 3
    end
  end
  object GroupBoxReadBanks: TGroupBox
    Left = 336
    Top = 8
    Width = 137
    Height = 129
    Caption = ' Read banks '
    TabOrder = 4
    object LabelReadBank0: TLabel
      Left = 8
      Top = 26
      Width = 17
      Height = 21
      Alignment = taRightJustify
      AutoSize = False
      Caption = '0'
      Layout = tlCenter
    end
    object LabelReadBank1: TLabel
      Left = 8
      Top = 50
      Width = 17
      Height = 21
      Alignment = taRightJustify
      AutoSize = False
      Caption = '1'
      Layout = tlCenter
    end
    object LabelReadBank2: TLabel
      Left = 8
      Top = 74
      Width = 17
      Height = 21
      Alignment = taRightJustify
      AutoSize = False
      Caption = '2'
      Layout = tlCenter
    end
    object LabelReadBank3: TLabel
      Left = 8
      Top = 98
      Width = 17
      Height = 21
      Alignment = taRightJustify
      AutoSize = False
      Caption = '3'
      Layout = tlCenter
    end
    object PanelReadBank0: TPanel
      Left = 40
      Top = 26
      Width = 81
      Height = 21
      ParentBackground = False
      ParentColor = True
      TabOrder = 0
    end
    object PanelReadBank1: TPanel
      Left = 40
      Top = 50
      Width = 81
      Height = 21
      ParentBackground = False
      ParentColor = True
      TabOrder = 1
    end
    object PanelReadBank2: TPanel
      Left = 40
      Top = 74
      Width = 81
      Height = 21
      ParentBackground = False
      ParentColor = True
      TabOrder = 2
    end
    object PanelReadBank3: TPanel
      Left = 40
      Top = 98
      Width = 81
      Height = 21
      ParentBackground = False
      ParentColor = True
      TabOrder = 3
    end
  end
  object GroupBoxWriteBanks: TGroupBox
    Left = 480
    Top = 8
    Width = 137
    Height = 129
    Caption = ' Write banks '
    TabOrder = 5
    object LabelWriteBank0: TLabel
      Left = 8
      Top = 26
      Width = 17
      Height = 21
      Alignment = taRightJustify
      AutoSize = False
      Caption = '0'
      Layout = tlCenter
    end
    object LabelWriteBank1: TLabel
      Left = 8
      Top = 50
      Width = 17
      Height = 21
      Alignment = taRightJustify
      AutoSize = False
      Caption = '1'
      Layout = tlCenter
    end
    object LabelWriteBank2: TLabel
      Left = 8
      Top = 74
      Width = 17
      Height = 21
      Alignment = taRightJustify
      AutoSize = False
      Caption = '2'
      Layout = tlCenter
    end
    object LabelWriteBank3: TLabel
      Left = 8
      Top = 98
      Width = 17
      Height = 21
      Alignment = taRightJustify
      AutoSize = False
      Caption = '3'
      Layout = tlCenter
    end
    object PanelWriteBank0: TPanel
      Left = 40
      Top = 26
      Width = 81
      Height = 21
      ParentBackground = False
      ParentColor = True
      TabOrder = 0
    end
    object PanelWriteBank1: TPanel
      Left = 40
      Top = 50
      Width = 81
      Height = 21
      ParentBackground = False
      ParentColor = True
      TabOrder = 1
    end
    object PanelWriteBank2: TPanel
      Left = 40
      Top = 74
      Width = 81
      Height = 21
      ParentBackground = False
      ParentColor = True
      TabOrder = 2
    end
    object PanelWriteBank3: TPanel
      Left = 40
      Top = 98
      Width = 81
      Height = 21
      ParentBackground = False
      ParentColor = True
      TabOrder = 3
    end
  end
end
