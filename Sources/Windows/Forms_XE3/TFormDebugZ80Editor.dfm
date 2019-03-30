object FormDebugZ80Editor: TFormDebugZ80Editor
  Left = 430
  Top = 201
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Z80 editor'
  ClientHeight = 545
  ClientWidth = 528
  Color = clBtnFace
  Constraints.MaxHeight = 573
  Constraints.MaxWidth = 544
  Constraints.MinHeight = 563
  Constraints.MinWidth = 534
  DoubleBuffered = True
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  ShowHint = True
  OnHide = FormHide
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBoxRegisters: TGroupBox
    Left = 8
    Top = 64
    Width = 513
    Height = 121
    Caption = ' Registers '
    TabOrder = 0
    object LabelRegAF: TLabel
      Left = 8
      Top = 18
      Width = 25
      Height = 21
      Hint = 'Horizontal Total'
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'AF'
      Layout = tlCenter
    end
    object LabelRegBC: TLabel
      Left = 8
      Top = 42
      Width = 25
      Height = 21
      Hint = 'Horizontal Total'
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'BC'
      Layout = tlCenter
    end
    object LabelRegDE: TLabel
      Left = 8
      Top = 66
      Width = 25
      Height = 21
      Hint = 'Horizontal Total'
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'DE'
      Layout = tlCenter
    end
    object LabelRegHL: TLabel
      Left = 8
      Top = 90
      Width = 25
      Height = 21
      Hint = 'Horizontal Total'
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'HL'
      Layout = tlCenter
    end
    object LabelRegAFAlt: TLabel
      Left = 144
      Top = 18
      Width = 25
      Height = 21
      Hint = 'Horizontal Total'
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'AF'#39
      Layout = tlCenter
    end
    object LabelRegBCAlt: TLabel
      Left = 144
      Top = 42
      Width = 25
      Height = 21
      Hint = 'Horizontal Total'
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'BC'#39
      Layout = tlCenter
    end
    object LabelRegIX: TLabel
      Left = 280
      Top = 18
      Width = 25
      Height = 21
      Hint = 'Horizontal Total'
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'IX'
      Layout = tlCenter
    end
    object LabelRegIY: TLabel
      Left = 280
      Top = 42
      Width = 25
      Height = 21
      Hint = 'Horizontal Total'
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'IY'
      Layout = tlCenter
    end
    object LabelRegI: TLabel
      Left = 416
      Top = 18
      Width = 25
      Height = 21
      Hint = 'Horizontal Total'
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'I'
      Layout = tlCenter
    end
    object LabelRegR: TLabel
      Left = 416
      Top = 42
      Width = 25
      Height = 21
      Hint = 'Horizontal Total'
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'R'
      Layout = tlCenter
    end
    object LabelRegDEAlt: TLabel
      Left = 144
      Top = 66
      Width = 25
      Height = 21
      Hint = 'Horizontal Total'
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'DE'#39
      Layout = tlCenter
    end
    object LabelRegHLAlt: TLabel
      Left = 144
      Top = 90
      Width = 25
      Height = 21
      Hint = 'Horizontal Total'
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'HL'#39
      Layout = tlCenter
    end
    object LabelRegPC: TLabel
      Left = 280
      Top = 90
      Width = 25
      Height = 21
      Hint = 'Horizontal Total'
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'PC'
      Layout = tlCenter
    end
    object LabelRegSP: TLabel
      Left = 280
      Top = 66
      Width = 25
      Height = 21
      Hint = 'Horizontal Total'
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'SP'
      Layout = tlCenter
    end
    object PanelFormatRegAF: TPanel
      Left = 40
      Top = 18
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 0
      OnClick = PanelFormatRegAFClick
      OnDblClick = PanelFormatRegAFClick
    end
    object PanelFormatRegBC: TPanel
      Tag = 1
      Left = 40
      Top = 42
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 1
      OnClick = PanelFormatRegBCClick
      OnDblClick = PanelFormatRegBCClick
    end
    object PanelRegA: TPanel
      Left = 57
      Top = 18
      Width = 33
      Height = 21
      Hint = 'Register A'
      Color = clWhite
      ParentBackground = False
      TabOrder = 2
      OnClick = PanelRegAClick
    end
    object PanelRegB: TPanel
      Tag = 1
      Left = 57
      Top = 42
      Width = 33
      Height = 21
      Hint = 'Register B'
      Color = clWhite
      ParentBackground = False
      TabOrder = 3
      OnClick = PanelRegBClick
    end
    object PanelRegF: TPanel
      Left = 90
      Top = 18
      Width = 33
      Height = 21
      Hint = 'Register F'
      Color = clWhite
      ParentBackground = False
      TabOrder = 4
      OnClick = PanelRegFClick
    end
    object PanelRegC: TPanel
      Left = 90
      Top = 42
      Width = 33
      Height = 21
      Hint = 'Register C'
      Color = clWhite
      ParentBackground = False
      TabOrder = 5
      OnClick = PanelRegCClick
    end
    object PanelFormatRegAFAlt: TPanel
      Left = 176
      Top = 18
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 6
      OnClick = PanelFormatRegAFAltClick
      OnDblClick = PanelFormatRegAFAltClick
    end
    object PanelRegAAlt: TPanel
      Left = 193
      Top = 18
      Width = 33
      Height = 21
      Hint = 'Register A'#39
      Color = clWhite
      ParentBackground = False
      TabOrder = 7
      OnClick = PanelRegAAltClick
    end
    object PanelRegFAlt: TPanel
      Left = 226
      Top = 18
      Width = 33
      Height = 21
      Hint = 'Register F'#39
      Color = clWhite
      ParentBackground = False
      TabOrder = 8
      OnClick = PanelRegFAltClick
    end
    object PanelFormatRegBCAlt: TPanel
      Tag = 1
      Left = 176
      Top = 42
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 9
      OnClick = PanelFormatRegBCAltClick
      OnDblClick = PanelFormatRegBCAltClick
    end
    object PanelRegBAlt: TPanel
      Tag = 1
      Left = 193
      Top = 42
      Width = 33
      Height = 21
      Hint = 'Register B'#39
      Color = clWhite
      ParentBackground = False
      TabOrder = 10
      OnClick = PanelRegBAltClick
    end
    object PanelRegCAlt: TPanel
      Left = 226
      Top = 42
      Width = 33
      Height = 21
      Hint = 'Register C'#39
      Color = clWhite
      ParentBackground = False
      TabOrder = 11
      OnClick = PanelRegCAltClick
    end
    object PanelFormatRegIX: TPanel
      Left = 312
      Top = 18
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 12
      OnClick = PanelFormatRegIXClick
      OnDblClick = PanelFormatRegIXClick
    end
    object PanelFormatRegIY: TPanel
      Left = 312
      Top = 42
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 13
      OnClick = PanelFormatRegIYClick
      OnDblClick = PanelFormatRegIYClick
    end
    object PanelFormatRegDE: TPanel
      Tag = 1
      Left = 40
      Top = 66
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 14
      OnClick = PanelFormatRegDEClick
      OnDblClick = PanelFormatRegDEClick
    end
    object PanelRegD: TPanel
      Tag = 1
      Left = 57
      Top = 66
      Width = 33
      Height = 21
      Hint = 'Register D'
      Color = clWhite
      ParentBackground = False
      TabOrder = 15
      OnClick = PanelRegDClick
    end
    object PanelRegE: TPanel
      Left = 90
      Top = 66
      Width = 33
      Height = 21
      Hint = 'Register E'
      Color = clWhite
      ParentBackground = False
      TabOrder = 16
      OnClick = PanelRegEClick
    end
    object PanelFormatRegHL: TPanel
      Tag = 1
      Left = 40
      Top = 90
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 17
      OnClick = PanelFormatRegHLClick
      OnDblClick = PanelFormatRegHLClick
    end
    object PanelRegH: TPanel
      Tag = 1
      Left = 57
      Top = 90
      Width = 33
      Height = 21
      Hint = 'Register H'
      Color = clWhite
      ParentBackground = False
      TabOrder = 18
      OnClick = PanelRegHClick
    end
    object PanelRegL: TPanel
      Left = 90
      Top = 90
      Width = 33
      Height = 21
      Hint = 'Register L'
      Color = clWhite
      ParentBackground = False
      TabOrder = 19
      OnClick = PanelRegLClick
    end
    object PanelFormatRegI: TPanel
      Left = 448
      Top = 18
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 20
      OnClick = PanelFormatRegIClick
      OnDblClick = PanelFormatRegIClick
    end
    object PanelRegI: TPanel
      Left = 465
      Top = 18
      Width = 33
      Height = 21
      Hint = 'Register I'
      Color = clWhite
      ParentBackground = False
      TabOrder = 21
      OnClick = PanelRegIClick
    end
    object PanelFormatRegR: TPanel
      Left = 448
      Top = 42
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 22
      OnClick = PanelFormatRegRClick
      OnDblClick = PanelFormatRegRClick
    end
    object PanelRegR: TPanel
      Left = 465
      Top = 42
      Width = 33
      Height = 21
      Hint = 'Register R'
      Color = clWhite
      ParentBackground = False
      TabOrder = 23
      OnClick = PanelRegRClick
    end
    object PanelFormatRegDEAlt: TPanel
      Tag = 1
      Left = 176
      Top = 66
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 24
      OnClick = PanelFormatRegDEAltClick
      OnDblClick = PanelFormatRegDEAltClick
    end
    object PanelRegDAlt: TPanel
      Tag = 1
      Left = 193
      Top = 66
      Width = 33
      Height = 21
      Hint = 'Register D'#39
      Color = clWhite
      ParentBackground = False
      TabOrder = 25
      OnClick = PanelRegDAltClick
    end
    object PanelRegEAlt: TPanel
      Left = 226
      Top = 66
      Width = 33
      Height = 21
      Hint = 'Register E'#39
      Color = clWhite
      ParentBackground = False
      TabOrder = 26
      OnClick = PanelRegEAltClick
    end
    object PanelFormatRegHLAlt: TPanel
      Tag = 1
      Left = 176
      Top = 90
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 27
      OnClick = PanelFormatRegHLAltClick
      OnDblClick = PanelFormatRegHLAltClick
    end
    object PanelRegHAlt: TPanel
      Tag = 1
      Left = 193
      Top = 90
      Width = 33
      Height = 21
      Hint = 'Register H'#39
      Color = clWhite
      ParentBackground = False
      TabOrder = 28
      OnClick = PanelRegHAltClick
    end
    object PanelRegLAlt: TPanel
      Left = 226
      Top = 90
      Width = 33
      Height = 21
      Hint = 'Register L'#39
      Color = clWhite
      ParentBackground = False
      TabOrder = 29
      OnClick = PanelRegLAltClick
    end
    object PanelFormatRegPC: TPanel
      Left = 312
      Top = 90
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 30
      OnClick = PanelFormatRegPCClick
      OnDblClick = PanelFormatRegPCClick
    end
    object PanelRegPC: TPanel
      Left = 329
      Top = 90
      Width = 66
      Height = 21
      Hint = 'Register PC'
      Color = clWhite
      ParentBackground = False
      TabOrder = 31
      OnClick = PanelRegPCClick
    end
    object PanelRegIY: TPanel
      Left = 329
      Top = 42
      Width = 66
      Height = 21
      Hint = 'Register IY'
      Color = clWhite
      ParentBackground = False
      TabOrder = 32
      OnClick = PanelRegIYClick
    end
    object PanelRegIX: TPanel
      Left = 329
      Top = 18
      Width = 66
      Height = 21
      Hint = 'Register IX'
      Color = clWhite
      ParentBackground = False
      TabOrder = 33
      OnClick = PanelRegIXClick
    end
    object PanelFormatRegSP: TPanel
      Left = 312
      Top = 66
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 34
      OnClick = PanelFormatRegSPClick
      OnDblClick = PanelFormatRegSPClick
    end
    object PanelRegSP: TPanel
      Left = 329
      Top = 66
      Width = 66
      Height = 21
      Hint = 'Register SP'
      Color = clWhite
      ParentBackground = False
      TabOrder = 35
      OnClick = PanelRegSPClick
    end
  end
  object GroupBoxInstructions: TGroupBox
    Left = 8
    Top = 248
    Width = 393
    Height = 289
    Caption = ' Instructions '
    ParentBackground = False
    TabOrder = 1
    DesignSize = (
      393
      289)
    object StringGridInstructions: TStringGrid
      Left = 2
      Top = 16
      Width = 389
      Height = 271
      Anchors = [akLeft, akTop, akRight, akBottom]
      BorderStyle = bsNone
      ColCount = 4
      DefaultRowHeight = 19
      DrawingStyle = gdsGradient
      Enabled = False
      FixedCols = 0
      FixedRows = 0
      GridLineWidth = 0
      Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRowSelect]
      ParentColor = True
      ScrollBars = ssNone
      TabOrder = 0
      OnDrawCell = StringGridInstructionsDrawCell
    end
  end
  object GroupBoxStack: TGroupBox
    Left = 408
    Top = 248
    Width = 113
    Height = 289
    Caption = ' Stack '
    TabOrder = 2
    DesignSize = (
      113
      289)
    object StringGridStack: TStringGrid
      Left = 2
      Top = 16
      Width = 109
      Height = 271
      Anchors = [akLeft, akTop, akRight, akBottom]
      BorderStyle = bsNone
      ColCount = 2
      DefaultRowHeight = 19
      DrawingStyle = gdsGradient
      Enabled = False
      FixedCols = 0
      RowCount = 8
      FixedRows = 0
      GridLineWidth = 0
      Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goRowSelect]
      ParentColor = True
      ScrollBars = ssVertical
      TabOrder = 0
      OnDrawCell = StringGridInstructionsDrawCell
    end
  end
  object GroupBoxFlags: TGroupBox
    Left = 8
    Top = 192
    Width = 513
    Height = 49
    Caption = ' Flags '
    TabOrder = 3
    object ComboBoxFlag7: TComboBox
      Tag = 128
      Left = 8
      Top = 16
      Width = 49
      Height = 22
      Hint = 'Sign flag (bit 7) : M=Negative, P=Positive'
      Style = csOwnerDrawFixed
      TabOrder = 0
      OnChange = ComboBoxFlagChange
      OnDrawItem = ComboBoxFlag7DrawItem
      Items.Strings = (
        'P'
        'M')
    end
    object ComboBoxFlag6: TComboBox
      Tag = 64
      Left = 72
      Top = 16
      Width = 49
      Height = 22
      Hint = 'Zero flag (bit 6) : Z=Zero, NZ=Non zero'
      Style = csOwnerDrawFixed
      TabOrder = 1
      OnChange = ComboBoxFlagChange
      OnDrawItem = ComboBoxFlag6DrawItem
      Items.Strings = (
        'NZ'
        'Z')
    end
    object ComboBoxFlag5: TComboBox
      Tag = 32
      Left = 136
      Top = 16
      Width = 49
      Height = 22
      Hint = 'Y flag (bit 5)'
      Style = csOwnerDrawFixed
      TabOrder = 2
      OnChange = ComboBoxFlagChange
      OnDrawItem = ComboBoxFlag5DrawItem
      Items.Strings = (
        '0'
        '1')
    end
    object ComboBoxFlag4: TComboBox
      Tag = 16
      Left = 200
      Top = 16
      Width = 49
      Height = 22
      Hint = 'Half-Carry flag (bit 4) : H=Half-carry, NH=No half-carry'
      Style = csOwnerDrawFixed
      TabOrder = 3
      OnChange = ComboBoxFlagChange
      OnDrawItem = ComboBoxFlag4DrawItem
      Items.Strings = (
        'NH'
        'H')
    end
    object ComboBoxFlag3: TComboBox
      Tag = 8
      Left = 264
      Top = 16
      Width = 49
      Height = 22
      Hint = 'X flag (bit 3)'
      Style = csOwnerDrawFixed
      TabOrder = 4
      OnChange = ComboBoxFlagChange
      OnDrawItem = ComboBoxFlag3DrawItem
      Items.Strings = (
        '0'
        '1')
    end
    object ComboBoxFlag2: TComboBox
      Tag = 4
      Left = 328
      Top = 16
      Width = 49
      Height = 22
      Hint = 
        'Parity/Overflow flag (bit 2) : PE=Parity even or Overflow, PO=Pa' +
        'rity odd or No overflow'
      Style = csOwnerDrawFixed
      TabOrder = 5
      OnChange = ComboBoxFlagChange
      OnDrawItem = ComboBoxFlag2DrawItem
      Items.Strings = (
        'PO'
        'PE')
    end
    object ComboBoxFlag1: TComboBox
      Tag = 2
      Left = 392
      Top = 16
      Width = 49
      Height = 22
      Hint = 'N flag (bit 1): N=Subtract, NN=No subtract'
      Style = csOwnerDrawFixed
      TabOrder = 6
      OnChange = ComboBoxFlagChange
      OnDrawItem = ComboBoxFlag1DrawItem
      Items.Strings = (
        'NN'
        'N')
    end
    object ComboBoxFlag0: TComboBox
      Tag = 1
      Left = 456
      Top = 16
      Width = 49
      Height = 22
      Hint = 'Carry flag (bit 0) : C=Carry, NC=No Carry'
      Style = csOwnerDrawFixed
      TabOrder = 7
      OnChange = ComboBoxFlagChange
      OnDrawItem = ComboBoxFlag0DrawItem
      Items.Strings = (
        'NC'
        'C')
    end
  end
  object GroupBoxInterrupt: TGroupBox
    Left = 8
    Top = 8
    Width = 241
    Height = 49
    Caption = ' Interrupt '
    TabOrder = 4
    object LabelIFF1: TLabel
      Left = 8
      Top = 18
      Width = 33
      Height = 21
      Hint = 'Horizontal Total'
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'IFF1'
      Layout = tlCenter
    end
    object LabelIFF2: TLabel
      Left = 64
      Top = 18
      Width = 33
      Height = 21
      Hint = 'Horizontal Total'
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'IFF2'
      Layout = tlCenter
    end
    object LabelITMode: TLabel
      Left = 123
      Top = 18
      Width = 41
      Height = 21
      Hint = 'Horizontal Total'
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Mode'
      Layout = tlCenter
    end
    object PanelIFF1: TPanel
      Left = 43
      Top = 18
      Width = 18
      Height = 21
      ParentColor = True
      TabOrder = 0
    end
    object PanelIFF2: TPanel
      Left = 100
      Top = 18
      Width = 18
      Height = 21
      ParentColor = True
      TabOrder = 1
    end
    object ComboBoxITMode: TComboBox
      Tag = 32
      Left = 168
      Top = 18
      Width = 65
      Height = 22
      Style = csOwnerDrawFixed
      TabOrder = 2
      OnChange = ComboBoxITModeChange
      OnDrawItem = ComboBoxITModeDrawItem
      Items.Strings = (
        'IM 0'
        'IM 1'
        'IM 2')
    end
  end
  object GroupBoxBreakpoint1: TGroupBox
    Left = 264
    Top = 8
    Width = 113
    Height = 49
    Caption = ' Breakpoint '
    TabOrder = 5
    object CheckBoxBreakpoint1: TCheckBox
      Left = 12
      Top = 20
      Width = 17
      Height = 17
      Hint = 'Activate breakpoint'
      TabOrder = 0
      OnClick = CheckBoxBreakpoint1Click
    end
    object PanelFormatBreakpoint1: TPanel
      Left = 30
      Top = 18
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 1
      OnClick = PanelFormatBreakpoint1Click
      OnDblClick = PanelFormatBreakpoint1Click
    end
    object PanelBreakpoint1: TPanel
      Left = 47
      Top = 18
      Width = 56
      Height = 21
      Color = clWhite
      ParentBackground = False
      TabOrder = 2
      OnClick = PanelBreakpoint1Click
    end
  end
end
