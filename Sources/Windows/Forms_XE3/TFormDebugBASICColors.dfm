object FormDebugBASICColors: TFormDebugBASICColors
  Left = 729
  Top = 360
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'BASIC colors'
  ClientHeight = 304
  ClientWidth = 609
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object PanelSelect0: TPanel
    Left = 8
    Top = 8
    Width = 193
    Height = 31
    BevelOuter = bvNone
    ParentColor = True
    TabOrder = 0
    object LabelColor0: TLabel
      Left = 8
      Top = 5
      Width = 25
      Height = 21
      AutoSize = False
      Caption = '0'
      Layout = tlCenter
    end
    object PanelColor0: TPanel
      Left = 33
      Top = 5
      Width = 33
      Height = 21
      Color = clBlack
      ParentBackground = False
      TabOrder = 0
      OnClick = PanelSelectClick
    end
    object PanelName0: TPanel
      Left = 72
      Top = 5
      Width = 113
      Height = 21
      Alignment = taLeftJustify
      BevelOuter = bvNone
      Caption = 'Black'
      ParentColor = True
      TabOrder = 1
      OnClick = PanelSelectClick
    end
  end
  object PanelSelect9: TPanel
    Tag = 9
    Left = 208
    Top = 8
    Width = 193
    Height = 31
    BevelOuter = bvNone
    ParentColor = True
    TabOrder = 1
    object LabelColor9: TLabel
      Tag = 9
      Left = 8
      Top = 5
      Width = 25
      Height = 21
      AutoSize = False
      Caption = '9'
      Layout = tlCenter
    end
    object PanelColor9: TPanel
      Tag = 9
      Left = 33
      Top = 5
      Width = 33
      Height = 21
      Color = clGreen
      ParentBackground = False
      TabOrder = 1
      OnClick = PanelSelectClick
    end
    object PanelName9: TPanel
      Tag = 9
      Left = 72
      Top = 5
      Width = 113
      Height = 21
      Alignment = taLeftJustify
      BevelOuter = bvNone
      Caption = 'Green'
      ParentColor = True
      TabOrder = 0
      OnClick = PanelSelectClick
    end
  end
  object PanelSelect1: TPanel
    Left = 8
    Top = 40
    Width = 193
    Height = 31
    BevelOuter = bvNone
    ParentColor = True
    TabOrder = 2
    object LabelColor1: TLabel
      Left = 8
      Top = 5
      Width = 25
      Height = 21
      AutoSize = False
      Caption = '1'
      Layout = tlCenter
    end
    object PanelColor1: TPanel
      Tag = 1
      Left = 33
      Top = 5
      Width = 33
      Height = 21
      Color = clNavy
      ParentBackground = False
      TabOrder = 0
      OnClick = PanelSelectClick
    end
    object PanelName1: TPanel
      Tag = 1
      Left = 72
      Top = 5
      Width = 113
      Height = 21
      Alignment = taLeftJustify
      BevelOuter = bvNone
      Caption = 'Blue'
      ParentColor = True
      TabOrder = 1
      OnClick = PanelSelectClick
    end
  end
  object PanelSelect18: TPanel
    Tag = 18
    Left = 408
    Top = 8
    Width = 193
    Height = 31
    BevelOuter = bvNone
    ParentColor = True
    TabOrder = 3
    object LabelColor18: TLabel
      Tag = 18
      Left = 8
      Top = 5
      Width = 25
      Height = 21
      AutoSize = False
      Caption = '18'
      Layout = tlCenter
    end
    object PanelColor18: TPanel
      Tag = 18
      Left = 33
      Top = 5
      Width = 33
      Height = 21
      Color = clLime
      ParentBackground = False
      TabOrder = 1
      OnClick = PanelSelectClick
    end
    object PanelName18: TPanel
      Tag = 18
      Left = 72
      Top = 5
      Width = 113
      Height = 21
      Alignment = taLeftJustify
      BevelOuter = bvNone
      Caption = 'Bright Green'
      ParentColor = True
      TabOrder = 0
      OnClick = PanelSelectClick
    end
  end
  object PanelSelect2: TPanel
    Left = 8
    Top = 72
    Width = 193
    Height = 31
    BevelOuter = bvNone
    ParentColor = True
    TabOrder = 4
    object Label1: TLabel
      Left = 8
      Top = 5
      Width = 25
      Height = 21
      AutoSize = False
      Caption = '2'
      Layout = tlCenter
    end
    object PanelColor2: TPanel
      Tag = 2
      Left = 33
      Top = 5
      Width = 33
      Height = 21
      Color = clBlue
      ParentBackground = False
      TabOrder = 0
      OnClick = PanelSelectClick
    end
    object PanelName2: TPanel
      Tag = 2
      Left = 72
      Top = 5
      Width = 113
      Height = 21
      Alignment = taLeftJustify
      BevelOuter = bvNone
      Caption = 'Bright Blue'
      ParentColor = True
      TabOrder = 1
      OnClick = PanelSelectClick
    end
  end
  object PanelSelect3: TPanel
    Left = 8
    Top = 104
    Width = 193
    Height = 31
    BevelOuter = bvNone
    ParentColor = True
    TabOrder = 5
    object Label2: TLabel
      Left = 8
      Top = 5
      Width = 25
      Height = 21
      AutoSize = False
      Caption = '3'
      Layout = tlCenter
    end
    object PanelColor3: TPanel
      Tag = 3
      Left = 33
      Top = 5
      Width = 33
      Height = 21
      Color = clMaroon
      ParentBackground = False
      TabOrder = 0
      OnClick = PanelSelectClick
    end
    object PanelName3: TPanel
      Tag = 3
      Left = 72
      Top = 5
      Width = 113
      Height = 21
      Alignment = taLeftJustify
      BevelOuter = bvNone
      Caption = 'Red'
      ParentColor = True
      TabOrder = 1
      OnClick = PanelSelectClick
    end
  end
  object PanelSelect4: TPanel
    Left = 8
    Top = 136
    Width = 193
    Height = 31
    BevelOuter = bvNone
    ParentColor = True
    TabOrder = 6
    object Label3: TLabel
      Left = 8
      Top = 5
      Width = 25
      Height = 21
      AutoSize = False
      Caption = '4'
      Layout = tlCenter
    end
    object PanelColor4: TPanel
      Tag = 4
      Left = 33
      Top = 5
      Width = 33
      Height = 21
      Color = clPurple
      ParentBackground = False
      TabOrder = 0
      OnClick = PanelSelectClick
    end
    object PanelName4: TPanel
      Tag = 4
      Left = 72
      Top = 5
      Width = 113
      Height = 21
      Alignment = taLeftJustify
      BevelOuter = bvNone
      Caption = 'Magenta'
      ParentColor = True
      TabOrder = 1
      OnClick = PanelSelectClick
    end
  end
  object PanelSelect5: TPanel
    Left = 8
    Top = 168
    Width = 193
    Height = 31
    BevelOuter = bvNone
    ParentColor = True
    TabOrder = 7
    object Label4: TLabel
      Left = 8
      Top = 5
      Width = 25
      Height = 21
      AutoSize = False
      Caption = '5'
      Layout = tlCenter
    end
    object PanelColor5: TPanel
      Tag = 5
      Left = 33
      Top = 5
      Width = 33
      Height = 21
      Color = 16711808
      ParentBackground = False
      TabOrder = 0
      OnClick = PanelSelectClick
    end
    object PanelName5: TPanel
      Tag = 5
      Left = 72
      Top = 5
      Width = 113
      Height = 21
      Alignment = taLeftJustify
      BevelOuter = bvNone
      Caption = 'Mauve'
      ParentColor = True
      TabOrder = 1
      OnClick = PanelSelectClick
    end
  end
  object PanelSelect6: TPanel
    Left = 8
    Top = 200
    Width = 193
    Height = 31
    BevelOuter = bvNone
    ParentColor = True
    TabOrder = 8
    object Label5: TLabel
      Left = 8
      Top = 5
      Width = 25
      Height = 21
      AutoSize = False
      Caption = '6'
      Layout = tlCenter
    end
    object PanelColor6: TPanel
      Tag = 6
      Left = 33
      Top = 5
      Width = 33
      Height = 21
      Color = clRed
      ParentBackground = False
      TabOrder = 0
      OnClick = PanelSelectClick
    end
    object PanelName6: TPanel
      Tag = 6
      Left = 72
      Top = 5
      Width = 113
      Height = 21
      Alignment = taLeftJustify
      BevelOuter = bvNone
      Caption = 'Bright Red'
      ParentColor = True
      TabOrder = 1
      OnClick = PanelSelectClick
    end
  end
  object PanelSelect7: TPanel
    Left = 8
    Top = 232
    Width = 193
    Height = 31
    BevelOuter = bvNone
    ParentColor = True
    TabOrder = 9
    object Label6: TLabel
      Left = 8
      Top = 5
      Width = 25
      Height = 21
      AutoSize = False
      Caption = '7'
      Layout = tlCenter
    end
    object PanelColor7: TPanel
      Tag = 7
      Left = 33
      Top = 5
      Width = 33
      Height = 21
      Color = 8388863
      ParentBackground = False
      TabOrder = 0
      OnClick = PanelSelectClick
    end
    object PanelName7: TPanel
      Tag = 7
      Left = 72
      Top = 5
      Width = 113
      Height = 21
      Alignment = taLeftJustify
      BevelOuter = bvNone
      Caption = 'Purple'
      ParentColor = True
      TabOrder = 1
      OnClick = PanelSelectClick
    end
  end
  object PanelSelect8: TPanel
    Left = 8
    Top = 264
    Width = 193
    Height = 31
    BevelOuter = bvNone
    ParentColor = True
    TabOrder = 10
    object Label7: TLabel
      Left = 8
      Top = 5
      Width = 25
      Height = 21
      AutoSize = False
      Caption = '8'
      Layout = tlCenter
    end
    object PanelColor8: TPanel
      Tag = 8
      Left = 33
      Top = 5
      Width = 33
      Height = 21
      Color = clFuchsia
      ParentBackground = False
      TabOrder = 0
      OnClick = PanelSelectClick
    end
    object PanelName8: TPanel
      Tag = 8
      Left = 72
      Top = 5
      Width = 113
      Height = 21
      Alignment = taLeftJustify
      BevelOuter = bvNone
      Caption = 'Bright Magenta'
      ParentColor = True
      TabOrder = 1
      OnClick = PanelSelectClick
    end
  end
  object PanelSelect10: TPanel
    Tag = 9
    Left = 208
    Top = 40
    Width = 193
    Height = 31
    BevelOuter = bvNone
    ParentColor = True
    TabOrder = 11
    object LabelColor10: TLabel
      Tag = 9
      Left = 8
      Top = 5
      Width = 25
      Height = 21
      AutoSize = False
      Caption = '10'
      Layout = tlCenter
    end
    object PanelColor10: TPanel
      Tag = 10
      Left = 33
      Top = 5
      Width = 33
      Height = 21
      Color = clTeal
      ParentBackground = False
      TabOrder = 1
      OnClick = PanelSelectClick
    end
    object PanelName10: TPanel
      Tag = 10
      Left = 72
      Top = 5
      Width = 113
      Height = 21
      Alignment = taLeftJustify
      BevelOuter = bvNone
      Caption = 'Cyan'
      ParentColor = True
      TabOrder = 0
      OnClick = PanelSelectClick
    end
  end
  object PanelSelect11: TPanel
    Tag = 9
    Left = 208
    Top = 72
    Width = 193
    Height = 31
    BevelOuter = bvNone
    ParentColor = True
    TabOrder = 12
    object LabelColor11: TLabel
      Tag = 9
      Left = 8
      Top = 5
      Width = 25
      Height = 21
      AutoSize = False
      Caption = '11'
      Layout = tlCenter
    end
    object PanelColor11: TPanel
      Tag = 11
      Left = 33
      Top = 5
      Width = 33
      Height = 21
      Color = 16744448
      ParentBackground = False
      TabOrder = 1
      OnClick = PanelSelectClick
    end
    object PanelName11: TPanel
      Tag = 11
      Left = 72
      Top = 5
      Width = 113
      Height = 21
      Alignment = taLeftJustify
      BevelOuter = bvNone
      Caption = 'Sky Blue'
      ParentColor = True
      TabOrder = 0
      OnClick = PanelSelectClick
    end
  end
  object PanelSelect12: TPanel
    Tag = 9
    Left = 208
    Top = 104
    Width = 193
    Height = 31
    BevelOuter = bvNone
    ParentColor = True
    TabOrder = 13
    object LabelColor12: TLabel
      Tag = 9
      Left = 8
      Top = 5
      Width = 25
      Height = 21
      AutoSize = False
      Caption = '12'
      Layout = tlCenter
    end
    object PanelColor12: TPanel
      Tag = 12
      Left = 33
      Top = 5
      Width = 33
      Height = 21
      Color = clOlive
      ParentBackground = False
      TabOrder = 1
      OnClick = PanelSelectClick
    end
    object PanelName12: TPanel
      Tag = 12
      Left = 72
      Top = 5
      Width = 113
      Height = 21
      Alignment = taLeftJustify
      BevelOuter = bvNone
      Caption = 'Yellow'
      ParentColor = True
      TabOrder = 0
      OnClick = PanelSelectClick
    end
  end
  object PanelSelect13: TPanel
    Tag = 9
    Left = 208
    Top = 136
    Width = 193
    Height = 31
    BevelOuter = bvNone
    ParentColor = True
    TabOrder = 14
    object LabelColor13: TLabel
      Tag = 9
      Left = 8
      Top = 5
      Width = 25
      Height = 21
      AutoSize = False
      Caption = '13'
      Layout = tlCenter
    end
    object PanelColor13: TPanel
      Tag = 13
      Left = 33
      Top = 5
      Width = 33
      Height = 21
      Color = clGray
      ParentBackground = False
      TabOrder = 1
      OnClick = PanelSelectClick
    end
    object PanelName13: TPanel
      Tag = 13
      Left = 72
      Top = 5
      Width = 113
      Height = 21
      Alignment = taLeftJustify
      BevelOuter = bvNone
      Caption = 'White'
      ParentColor = True
      TabOrder = 0
      OnClick = PanelSelectClick
    end
  end
  object PanelSelect14: TPanel
    Tag = 9
    Left = 208
    Top = 168
    Width = 193
    Height = 31
    BevelOuter = bvNone
    ParentColor = True
    TabOrder = 15
    object LabelColor14: TLabel
      Tag = 9
      Left = 8
      Top = 5
      Width = 25
      Height = 21
      AutoSize = False
      Caption = '14'
      Layout = tlCenter
    end
    object PanelColor14: TPanel
      Tag = 14
      Left = 33
      Top = 5
      Width = 33
      Height = 21
      Color = 16744576
      ParentBackground = False
      TabOrder = 1
      OnClick = PanelSelectClick
    end
    object PanelName14: TPanel
      Tag = 14
      Left = 72
      Top = 5
      Width = 113
      Height = 21
      Alignment = taLeftJustify
      BevelOuter = bvNone
      Caption = 'Pastel Blue'
      ParentColor = True
      TabOrder = 0
      OnClick = PanelSelectClick
    end
  end
  object PanelSelect15: TPanel
    Tag = 9
    Left = 208
    Top = 200
    Width = 193
    Height = 31
    BevelOuter = bvNone
    ParentColor = True
    TabOrder = 16
    object LabelColor15: TLabel
      Tag = 9
      Left = 8
      Top = 5
      Width = 25
      Height = 21
      AutoSize = False
      Caption = '15'
      Layout = tlCenter
    end
    object PanelColor15: TPanel
      Tag = 15
      Left = 33
      Top = 5
      Width = 33
      Height = 21
      Color = 33023
      ParentBackground = False
      TabOrder = 1
      OnClick = PanelSelectClick
    end
    object PanelName15: TPanel
      Tag = 15
      Left = 72
      Top = 5
      Width = 113
      Height = 21
      Alignment = taLeftJustify
      BevelOuter = bvNone
      Caption = 'Orange'
      ParentColor = True
      TabOrder = 0
      OnClick = PanelSelectClick
    end
  end
  object PanelSelect16: TPanel
    Tag = 9
    Left = 208
    Top = 232
    Width = 193
    Height = 31
    BevelOuter = bvNone
    ParentColor = True
    TabOrder = 17
    object LabelColor16: TLabel
      Tag = 9
      Left = 8
      Top = 5
      Width = 25
      Height = 21
      AutoSize = False
      Caption = '16'
      Layout = tlCenter
    end
    object PanelColor16: TPanel
      Tag = 16
      Left = 33
      Top = 5
      Width = 33
      Height = 21
      Color = 8421631
      ParentBackground = False
      TabOrder = 1
      OnClick = PanelSelectClick
    end
    object PanelName16: TPanel
      Tag = 16
      Left = 72
      Top = 5
      Width = 113
      Height = 21
      Alignment = taLeftJustify
      BevelOuter = bvNone
      Caption = 'Pink'
      ParentColor = True
      TabOrder = 0
      OnClick = PanelSelectClick
    end
  end
  object PanelSelect17: TPanel
    Tag = 9
    Left = 208
    Top = 264
    Width = 193
    Height = 31
    BevelOuter = bvNone
    ParentColor = True
    TabOrder = 18
    object LabelColor17: TLabel
      Tag = 9
      Left = 8
      Top = 5
      Width = 25
      Height = 21
      AutoSize = False
      Caption = '17'
      Layout = tlCenter
    end
    object PanelColor17: TPanel
      Tag = 17
      Left = 33
      Top = 5
      Width = 33
      Height = 21
      Color = 16744703
      ParentBackground = False
      TabOrder = 1
      OnClick = PanelSelectClick
    end
    object PanelName17: TPanel
      Tag = 17
      Left = 72
      Top = 5
      Width = 113
      Height = 21
      Alignment = taLeftJustify
      BevelOuter = bvNone
      Caption = 'Pastel Magenta'
      ParentColor = True
      TabOrder = 0
      OnClick = PanelSelectClick
    end
  end
  object PanelSelect19: TPanel
    Tag = 18
    Left = 408
    Top = 40
    Width = 193
    Height = 31
    BevelOuter = bvNone
    ParentColor = True
    TabOrder = 19
    object LabelColor19: TLabel
      Tag = 18
      Left = 8
      Top = 5
      Width = 25
      Height = 21
      AutoSize = False
      Caption = '19'
      Layout = tlCenter
    end
    object PanelColor19: TPanel
      Tag = 19
      Left = 33
      Top = 5
      Width = 33
      Height = 21
      Color = 8453888
      ParentBackground = False
      TabOrder = 1
      OnClick = PanelSelectClick
    end
    object PanelName19: TPanel
      Tag = 19
      Left = 72
      Top = 5
      Width = 113
      Height = 21
      Alignment = taLeftJustify
      BevelOuter = bvNone
      Caption = 'Sea Green'
      ParentColor = True
      TabOrder = 0
      OnClick = PanelSelectClick
    end
  end
  object PanelSelect20: TPanel
    Tag = 18
    Left = 408
    Top = 72
    Width = 193
    Height = 31
    BevelOuter = bvNone
    ParentColor = True
    TabOrder = 20
    object LabelColor20: TLabel
      Tag = 18
      Left = 8
      Top = 5
      Width = 25
      Height = 21
      AutoSize = False
      Caption = '20'
      Layout = tlCenter
    end
    object PanelColor20: TPanel
      Tag = 20
      Left = 33
      Top = 5
      Width = 33
      Height = 21
      Color = clAqua
      ParentBackground = False
      TabOrder = 1
      OnClick = PanelSelectClick
    end
    object PanelName20: TPanel
      Tag = 20
      Left = 72
      Top = 5
      Width = 113
      Height = 21
      Alignment = taLeftJustify
      BevelOuter = bvNone
      Caption = 'Bright Cyan'
      ParentColor = True
      TabOrder = 0
      OnClick = PanelSelectClick
    end
  end
  object PanelSelect21: TPanel
    Tag = 18
    Left = 408
    Top = 104
    Width = 193
    Height = 31
    BevelOuter = bvNone
    ParentColor = True
    TabOrder = 21
    object LabelColor21: TLabel
      Tag = 18
      Left = 8
      Top = 5
      Width = 25
      Height = 21
      AutoSize = False
      Caption = '21'
      Layout = tlCenter
    end
    object PanelColor21: TPanel
      Tag = 21
      Left = 33
      Top = 5
      Width = 33
      Height = 21
      Color = 65408
      ParentBackground = False
      TabOrder = 1
      OnClick = PanelSelectClick
    end
    object PanelName21: TPanel
      Tag = 21
      Left = 72
      Top = 5
      Width = 113
      Height = 21
      Alignment = taLeftJustify
      BevelOuter = bvNone
      Caption = 'Lime Green'
      ParentColor = True
      TabOrder = 0
      OnClick = PanelSelectClick
    end
  end
  object PanelSelect22: TPanel
    Tag = 18
    Left = 408
    Top = 136
    Width = 193
    Height = 31
    BevelOuter = bvNone
    ParentColor = True
    TabOrder = 22
    object LabelColor22: TLabel
      Tag = 18
      Left = 8
      Top = 5
      Width = 25
      Height = 21
      AutoSize = False
      Caption = '22'
      Layout = tlCenter
    end
    object PanelColor22: TPanel
      Tag = 22
      Left = 33
      Top = 5
      Width = 33
      Height = 21
      Color = 8454016
      ParentBackground = False
      TabOrder = 1
      OnClick = PanelSelectClick
    end
    object PanelName22: TPanel
      Tag = 22
      Left = 72
      Top = 5
      Width = 113
      Height = 21
      Alignment = taLeftJustify
      BevelOuter = bvNone
      Caption = 'Pastel Green'
      ParentColor = True
      TabOrder = 0
      OnClick = PanelSelectClick
    end
  end
  object PanelSelect23: TPanel
    Tag = 18
    Left = 408
    Top = 168
    Width = 193
    Height = 31
    BevelOuter = bvNone
    ParentColor = True
    TabOrder = 23
    object LabelColor23: TLabel
      Tag = 18
      Left = 8
      Top = 5
      Width = 25
      Height = 21
      AutoSize = False
      Caption = '23'
      Layout = tlCenter
    end
    object PanelColor23: TPanel
      Tag = 23
      Left = 33
      Top = 5
      Width = 33
      Height = 21
      Color = 16777088
      ParentBackground = False
      TabOrder = 1
      OnClick = PanelSelectClick
    end
    object PanelName23: TPanel
      Tag = 23
      Left = 72
      Top = 5
      Width = 113
      Height = 21
      Alignment = taLeftJustify
      BevelOuter = bvNone
      Caption = 'Pastel Cyan'
      ParentColor = True
      TabOrder = 0
      OnClick = PanelSelectClick
    end
  end
  object PanelSelect24: TPanel
    Tag = 18
    Left = 408
    Top = 200
    Width = 193
    Height = 31
    BevelOuter = bvNone
    ParentColor = True
    TabOrder = 24
    object LabelColor24: TLabel
      Tag = 18
      Left = 8
      Top = 5
      Width = 25
      Height = 21
      AutoSize = False
      Caption = '24'
      Layout = tlCenter
    end
    object PanelColor24: TPanel
      Tag = 24
      Left = 33
      Top = 5
      Width = 33
      Height = 21
      Color = clYellow
      ParentBackground = False
      TabOrder = 1
      OnClick = PanelSelectClick
    end
    object PanelName24: TPanel
      Tag = 24
      Left = 72
      Top = 5
      Width = 113
      Height = 21
      Alignment = taLeftJustify
      BevelOuter = bvNone
      Caption = 'Bright Yellow'
      ParentColor = True
      TabOrder = 0
      OnClick = PanelSelectClick
    end
  end
  object PanelSelect25: TPanel
    Tag = 18
    Left = 408
    Top = 232
    Width = 193
    Height = 31
    BevelOuter = bvNone
    ParentColor = True
    TabOrder = 25
    object LabelColor25: TLabel
      Tag = 18
      Left = 8
      Top = 5
      Width = 25
      Height = 21
      AutoSize = False
      Caption = '25'
      Layout = tlCenter
    end
    object PanelColor25: TPanel
      Tag = 25
      Left = 33
      Top = 5
      Width = 33
      Height = 21
      Color = 8454143
      ParentBackground = False
      TabOrder = 1
      OnClick = PanelSelectClick
    end
    object PanelName25: TPanel
      Tag = 25
      Left = 72
      Top = 5
      Width = 113
      Height = 21
      Alignment = taLeftJustify
      BevelOuter = bvNone
      Caption = 'Pastel Yellow'
      ParentColor = True
      TabOrder = 0
      OnClick = PanelSelectClick
    end
  end
  object PanelSelect26: TPanel
    Tag = 18
    Left = 408
    Top = 264
    Width = 193
    Height = 31
    BevelOuter = bvNone
    ParentColor = True
    TabOrder = 26
    object LabelColor26: TLabel
      Tag = 18
      Left = 8
      Top = 5
      Width = 25
      Height = 21
      AutoSize = False
      Caption = '26'
      Layout = tlCenter
    end
    object PanelColor26: TPanel
      Tag = 26
      Left = 33
      Top = 5
      Width = 33
      Height = 21
      Color = clWhite
      ParentBackground = False
      TabOrder = 1
      OnClick = PanelSelectClick
    end
    object PanelName26: TPanel
      Tag = 26
      Left = 72
      Top = 5
      Width = 113
      Height = 21
      Alignment = taLeftJustify
      BevelOuter = bvNone
      Caption = 'Bright White'
      ParentColor = True
      TabOrder = 0
      OnClick = PanelSelectClick
    end
  end
  object ButtonCancel: TButton
    Left = 168
    Top = 312
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'ButtonCancel'
    ModalResult = 2
    TabOrder = 27
  end
end
