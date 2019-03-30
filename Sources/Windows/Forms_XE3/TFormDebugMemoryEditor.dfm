object FormDebugMemoryEditor: TFormDebugMemoryEditor
  Left = 614
  Top = 310
  BorderIcons = [biSystemMenu]
  Caption = 'Memory editor'
  ClientHeight = 356
  ClientWidth = 592
  Color = clBtnFace
  Constraints.MaxWidth = 608
  Constraints.MinHeight = 394
  Constraints.MinWidth = 608
  DoubleBuffered = True
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnDestroy = FormDestroy
  OnHide = FormHide
  OnResize = FormResize
  OnShow = FormShow
  DesignSize = (
    592
    356)
  PixelsPerInch = 96
  TextHeight = 13
  object TabControlMemoryContent: TTabControl
    Left = 8
    Top = 160
    Width = 578
    Height = 190
    Anchors = [akLeft, akTop, akRight, akBottom]
    OwnerDraw = True
    TabHeight = 20
    TabOrder = 0
    Tabs.Strings = (
      'Banked Memory'
      'RAM'
      'Lower ROM'
      'Upper ROM')
    TabIndex = 0
    OnChange = TabControlMemoryContentChange
    OnDrawTab = TabControlMemoryContentDrawTab
    OnResize = TabControlMemoryContentResize
    object ScrollBarDump: TScrollBar
      Left = 557
      Top = 26
      Width = 17
      Height = 160
      Align = alRight
      Kind = sbVertical
      LargeChange = 256
      Max = 65520
      PageSize = 0
      SmallChange = 16
      TabOrder = 0
      OnChange = ScrollBarDumpChange
    end
    object StringGridDump: TStringGrid
      Left = 4
      Top = 26
      Width = 553
      Height = 160
      Align = alClient
      BorderStyle = bsNone
      ColCount = 18
      DefaultColWidth = 22
      DefaultRowHeight = 20
      FixedColor = clWindow
      RowCount = 8
      FixedRows = 0
      GridLineWidth = 0
      Options = [goDrawFocusSelected]
      ParentColor = True
      PopupMenu = PopupMenuMemoryContent
      ScrollBars = ssNone
      TabOrder = 1
      OnClick = StringGridDumpClick
      OnDblClick = StringGridDumpDblClick
      OnDrawCell = StringGridDumpDrawCell
      OnKeyDown = StringGridDumpKeyDown
      OnKeyPress = StringGridDumpKeyPress
      OnMouseDown = StringGridDumpMouseDown
      OnMouseWheelDown = StringGridDumpMouseWheelDown
      OnMouseWheelUp = StringGridDumpMouseWheelUp
      OnSelectCell = StringGridDumpSelectCell
      RowHeights = (
        20
        20
        20
        20
        20
        20
        20
        20)
    end
  end
  object PageControlOperations: TPageControl
    Left = 8
    Top = 8
    Width = 578
    Height = 105
    ActivePage = TabSheetBanks
    Anchors = [akLeft, akTop, akRight]
    OwnerDraw = True
    TabHeight = 20
    TabOrder = 1
    OnChange = PageControlOperationsChange
    OnDrawTab = PageControlOperationsDrawTab
    object TabSheetAddress: TTabSheet
      Caption = 'Address'
      DesignSize = (
        570
        75)
      object LabelAddress1: TLabel
        Left = 8
        Top = 10
        Width = 81
        Height = 21
        Hint = 'Horizontal Total'
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'Address 1'
        Layout = tlCenter
      end
      object LabelRegisters: TLabel
        Left = 297
        Top = 10
        Width = 81
        Height = 21
        Hint = 'Horizontal Total'
        Alignment = taRightJustify
        Anchors = [akTop, akRight]
        AutoSize = False
        Caption = 'Registers'
        Layout = tlCenter
        ExplicitLeft = 272
      end
      object LabelAddress2: TLabel
        Left = 8
        Top = 42
        Width = 81
        Height = 21
        Hint = 'Horizontal Total'
        Alignment = taRightJustify
        AutoSize = False
        Caption = '2'
        Layout = tlCenter
      end
      object PanelFormatAddress1: TPanel
        Left = 96
        Top = 10
        Width = 17
        Height = 21
        Caption = '&&'
        Color = 16773087
        ParentBackground = False
        TabOrder = 0
        OnClick = PanelFormatAddress1Click
        OnDblClick = PanelFormatAddress1Click
      end
      object PanelAddress1: TPanel
        Left = 113
        Top = 10
        Width = 66
        Height = 21
        Color = clWhite
        ParentBackground = False
        TabOrder = 1
        OnClick = PanelAddress1Click
      end
      object PanelFormatAddress2: TPanel
        Left = 96
        Top = 42
        Width = 17
        Height = 21
        Caption = '&&'
        Color = 16773087
        ParentBackground = False
        TabOrder = 2
        OnClick = PanelFormatAddress2Click
        OnDblClick = PanelFormatAddress2Click
      end
      object PanelAddress2: TPanel
        Left = 113
        Top = 42
        Width = 66
        Height = 21
        Color = clWhite
        ParentBackground = False
        TabOrder = 3
        OnClick = PanelAddress2Click
      end
      object ButtonGoToAddress1: TButton
        Left = 179
        Top = 10
        Width = 38
        Height = 21
        Hint = 'Go to address 1'
        Caption = 'Go'
        TabOrder = 4
        OnClick = ButtonGoToAddress1Click
      end
      object ButtonGoToAddress2: TButton
        Left = 179
        Top = 42
        Width = 38
        Height = 21
        Hint = 'Go to address 2'
        Caption = 'Go'
        TabOrder = 5
        OnClick = ButtonGoToAddress2Click
      end
      object ButtonRegBC: TButton
        Left = 386
        Top = 10
        Width = 32
        Height = 23
        Anchors = [akTop, akRight]
        Caption = 'BC'
        TabOrder = 6
        OnClick = ButtonRegBCClick
      end
      object ButtonRegDE: TButton
        Left = 418
        Top = 10
        Width = 32
        Height = 23
        Anchors = [akTop, akRight]
        Caption = 'DE'
        TabOrder = 7
        OnClick = ButtonRegDEClick
      end
      object ButtonRegHL: TButton
        Left = 450
        Top = 10
        Width = 32
        Height = 23
        Anchors = [akTop, akRight]
        Caption = 'HL'
        TabOrder = 8
        OnClick = ButtonRegHLClick
      end
      object ButtonRegBCAlt: TButton
        Left = 386
        Top = 42
        Width = 32
        Height = 23
        Anchors = [akTop, akRight]
        Caption = 'BC'#39
        TabOrder = 9
        OnClick = ButtonRegBCAltClick
      end
      object ButtonRegDEAlt: TButton
        Left = 418
        Top = 42
        Width = 32
        Height = 23
        Anchors = [akTop, akRight]
        Caption = 'DE'#39
        TabOrder = 10
        OnClick = ButtonRegDEAltClick
      end
      object ButtonRegHLAlt: TButton
        Left = 450
        Top = 42
        Width = 32
        Height = 23
        Anchors = [akTop, akRight]
        Caption = 'HL'#39
        TabOrder = 11
        OnClick = ButtonRegHLAltClick
      end
      object ButtonRegSP: TButton
        Left = 490
        Top = 10
        Width = 32
        Height = 23
        Anchors = [akTop, akRight]
        Caption = 'SP'
        TabOrder = 12
        OnClick = ButtonRegSPClick
      end
      object ButtonRegPC: TButton
        Left = 522
        Top = 10
        Width = 32
        Height = 23
        Anchors = [akTop, akRight]
        Caption = 'PC'
        TabOrder = 13
        OnClick = ButtonRegPCClick
      end
      object ButtonRegIX: TButton
        Left = 490
        Top = 42
        Width = 32
        Height = 23
        Anchors = [akTop, akRight]
        Caption = 'IX'
        TabOrder = 14
        OnClick = ButtonRegIXClick
      end
      object ButtonRegIY: TButton
        Left = 522
        Top = 42
        Width = 32
        Height = 23
        Anchors = [akTop, akRight]
        Caption = 'IY'
        TabOrder = 15
        OnClick = ButtonRegIYClick
      end
    end
    object TabSheetBanks: TTabSheet
      Caption = 'Banks'
      ImageIndex = 4
      object LabelBanksRead: TLabel
        Left = 8
        Top = 26
        Width = 57
        Height = 21
        Hint = 'Horizontal Total'
        AutoSize = False
        Caption = 'Read'
        Layout = tlCenter
      end
      object LabelBanksWrite: TLabel
        Left = 8
        Top = 50
        Width = 57
        Height = 21
        Hint = 'Horizontal Total'
        AutoSize = False
        Caption = 'Write'
        Layout = tlCenter
      end
      object LabelBanksC000: TLabel
        Left = 432
        Top = 2
        Width = 105
        Height = 21
        Hint = 'Horizontal Total'
        Alignment = taCenter
        AutoSize = False
        Caption = '&&C000'
        Layout = tlCenter
      end
      object LabelBanks8000: TLabel
        Left = 320
        Top = 2
        Width = 89
        Height = 21
        Hint = 'Horizontal Total'
        Alignment = taCenter
        AutoSize = False
        Caption = '&&8000'
        Layout = tlCenter
      end
      object LabelBanks4000: TLabel
        Left = 208
        Top = 2
        Width = 89
        Height = 21
        Hint = 'Horizontal Total'
        Alignment = taCenter
        AutoSize = False
        Caption = '&&4000'
        Layout = tlCenter
      end
      object LabelBanks0000: TLabel
        Left = 80
        Top = 2
        Width = 105
        Height = 21
        Hint = 'Horizontal Total'
        Alignment = taCenter
        AutoSize = False
        Caption = '&&0000'
        Layout = tlCenter
      end
      object LabelBanksBank: TLabel
        Left = 8
        Top = 2
        Width = 57
        Height = 21
        Hint = 'Horizontal Total'
        AutoSize = False
        Caption = 'Bank'
        Layout = tlCenter
      end
      object PanelBankRead4000: TPanel
        Left = 208
        Top = 26
        Width = 89
        Height = 21
        BevelInner = bvLowered
        ParentColor = True
        TabOrder = 0
      end
      object PanelBankWrite4000: TPanel
        Left = 208
        Top = 50
        Width = 89
        Height = 21
        BevelInner = bvLowered
        ParentColor = True
        TabOrder = 1
      end
      object PanelBankRead8000: TPanel
        Left = 320
        Top = 26
        Width = 89
        Height = 21
        BevelInner = bvLowered
        ParentColor = True
        TabOrder = 2
      end
      object PanelBankWrite8000: TPanel
        Left = 320
        Top = 50
        Width = 89
        Height = 21
        BevelInner = bvLowered
        ParentColor = True
        TabOrder = 3
      end
      object PanelBankWrite0000: TPanel
        Left = 80
        Top = 50
        Width = 105
        Height = 21
        BevelInner = bvLowered
        ParentColor = True
        TabOrder = 4
      end
      object PanelBankWriteC000: TPanel
        Left = 432
        Top = 50
        Width = 105
        Height = 21
        BevelInner = bvLowered
        ParentColor = True
        TabOrder = 5
      end
      object PanelBankRead0000: TPanel
        Left = 80
        Top = 26
        Width = 105
        Height = 21
        BevelInner = bvLowered
        ParentColor = True
        TabOrder = 6
      end
      object PanelBankReadC000: TPanel
        Left = 432
        Top = 26
        Width = 105
        Height = 21
        BevelInner = bvLowered
        ParentColor = True
        TabOrder = 7
      end
    end
    object TabSheetSearch: TTabSheet
      Caption = 'Search'
      ImageIndex = 1
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object LabelSearch: TLabel
        Left = 8
        Top = 16
        Width = 65
        Height = 21
        Hint = 'Horizontal Total'
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'Text'
        Layout = tlCenter
      end
      object PanelFormatSearch: TPanel
        Left = 80
        Top = 16
        Width = 17
        Height = 21
        Caption = '$'
        Color = 16773087
        ParentBackground = False
        TabOrder = 0
        OnClick = PanelFormatSearchClick
        OnDblClick = PanelFormatSearchClick
      end
      object EditSearch: TEdit
        Left = 96
        Top = 16
        Width = 281
        Height = 22
        Hint = 'Hex data can be "11AA02" or "11 AA 02" or "11.AA.02"...'
        AutoSize = False
        TabOrder = 1
      end
      object ButtonSearchNext: TButton
        Left = 400
        Top = 16
        Width = 81
        Height = 47
        Caption = 'Next'
        TabOrder = 2
        OnClick = ButtonSearchNextClick
      end
      object CheckBoxStringCaseSensitive: TCheckBox
        Left = 96
        Top = 46
        Width = 281
        Height = 17
        Caption = 'Case sensitive'
        TabOrder = 3
      end
    end
    object TabSheetFill: TTabSheet
      Caption = 'Fill'
      ImageIndex = 2
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object LabelFillRAMAddress: TLabel
        Left = 0
        Top = 10
        Width = 105
        Height = 21
        Hint = 'Horizontal Total'
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'RAM Address'
        Layout = tlCenter
      end
      object LabelFillFrom: TLabel
        Left = 112
        Top = 10
        Width = 41
        Height = 21
        Hint = 'Horizontal Total'
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'From'
        Layout = tlCenter
      end
      object LabelFillTo: TLabel
        Left = 112
        Top = 42
        Width = 41
        Height = 21
        Hint = 'Horizontal Total'
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'To'
        Layout = tlCenter
      end
      object LabelFillData: TLabel
        Left = 256
        Top = 10
        Width = 49
        Height = 21
        Hint = 'Horizontal Total'
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'Data'
        Layout = tlCenter
      end
      object PanelFormatFillFrom: TPanel
        Left = 160
        Top = 10
        Width = 17
        Height = 21
        Caption = '&&'
        Color = 16773087
        ParentBackground = False
        TabOrder = 0
        OnClick = PanelFormatFillFromClick
        OnDblClick = PanelFormatFillFromClick
      end
      object PanelFillFrom: TPanel
        Left = 177
        Top = 10
        Width = 66
        Height = 21
        Color = clWhite
        ParentBackground = False
        TabOrder = 1
        OnClick = PanelFillFromClick
      end
      object PanelFormatFillTo: TPanel
        Left = 160
        Top = 42
        Width = 17
        Height = 21
        Caption = '&&'
        Color = 16773087
        ParentBackground = False
        TabOrder = 2
        OnClick = PanelFormatFillToClick
        OnDblClick = PanelFormatFillToClick
      end
      object PanelFillTo: TPanel
        Left = 177
        Top = 42
        Width = 66
        Height = 21
        Color = clWhite
        ParentBackground = False
        TabOrder = 3
        OnClick = PanelFillToClick
      end
      object PanelFormatFillData: TPanel
        Left = 312
        Top = 10
        Width = 17
        Height = 21
        Caption = '&&'
        Color = 16773087
        ParentBackground = False
        TabOrder = 4
        OnClick = PanelFormatFillDataClick
        OnDblClick = PanelFormatFillDataClick
      end
      object PanelFillData: TPanel
        Left = 329
        Top = 10
        Width = 32
        Height = 21
        Color = clWhite
        ParentBackground = False
        TabOrder = 5
        OnClick = PanelFillDataClick
      end
      object ButtonFill: TButton
        Left = 400
        Top = 16
        Width = 81
        Height = 47
        Caption = 'Fill'
        TabOrder = 6
        TabStop = False
        OnClick = ButtonFillClick
      end
    end
    object TabSheetDump: TTabSheet
      Caption = 'Dump'
      ImageIndex = 3
      DesignSize = (
        570
        75)
      object LabelDumpFrom: TLabel
        Left = 144
        Top = 10
        Width = 41
        Height = 21
        Hint = 'Horizontal Total'
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'From'
        Layout = tlCenter
      end
      object LabelDumpTo: TLabel
        Left = 144
        Top = 42
        Width = 41
        Height = 21
        Hint = 'Horizontal Total'
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'To'
        Layout = tlCenter
      end
      object PanelFormatExportFrom: TPanel
        Left = 192
        Top = 10
        Width = 17
        Height = 21
        Caption = '&&'
        Color = 16773087
        ParentBackground = False
        TabOrder = 0
        OnClick = PanelFormatExportFromClick
        OnDblClick = PanelFormatExportFromClick
      end
      object PanelExportFrom: TPanel
        Left = 209
        Top = 10
        Width = 66
        Height = 21
        Color = clWhite
        ParentBackground = False
        TabOrder = 1
        OnClick = PanelExportFromClick
      end
      object PanelFormatExportTo: TPanel
        Left = 192
        Top = 42
        Width = 17
        Height = 21
        Caption = '&&'
        Color = 16773087
        ParentBackground = False
        TabOrder = 2
        OnClick = PanelFormatExportToClick
        OnDblClick = PanelFormatExportToClick
      end
      object PanelExportTo: TPanel
        Left = 209
        Top = 42
        Width = 66
        Height = 21
        Color = clWhite
        ParentBackground = False
        TabOrder = 3
        OnClick = PanelExportToClick
      end
      object ButtonDumpExportText: TButton
        Left = 305
        Top = 8
        Width = 129
        Height = 25
        Anchors = [akTop, akRight]
        Caption = 'Export text'
        TabOrder = 4
        TabStop = False
        OnClick = ButtonDumpExportTextClick
      end
      object ButtonDumpSetFullLimits: TButton
        Left = 8
        Top = 10
        Width = 129
        Height = 23
        Caption = 'Set full limits'
        TabOrder = 5
        TabStop = False
        OnClick = ButtonDumpSetFullLimitsClick
      end
      object ButtonDumpImportBinary: TButton
        Left = 441
        Top = 43
        Width = 121
        Height = 25
        Anchors = [akTop, akRight]
        Caption = 'Import binary'
        TabOrder = 6
        TabStop = False
        OnClick = ButtonDumpImportBinaryClick
      end
      object ButtonDumpExportBinary: TButton
        Left = 305
        Top = 43
        Width = 129
        Height = 25
        Anchors = [akTop, akRight]
        Caption = 'Export binary'
        TabOrder = 7
        TabStop = False
        OnClick = ButtonDumpExportBinaryClick
      end
    end
  end
  object PanelStatus: TPanel
    Left = 8
    Top = 120
    Width = 585
    Height = 33
    Anchors = [akLeft, akTop, akRight]
    BevelOuter = bvNone
    ParentColor = True
    TabOrder = 2
    DesignSize = (
      585
      33)
    object LabelCurrentAddress: TLabel
      Left = 0
      Top = 4
      Width = 137
      Height = 21
      Hint = 'Horizontal Total'
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Current address'
      Layout = tlCenter
    end
    object PanelFormatCurrentAddress: TPanel
      Left = 144
      Top = 4
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 0
      OnClick = PanelFormatCurrentAddressClick
      OnDblClick = PanelFormatCurrentAddressClick
    end
    object PanelCurrentAddress: TPanel
      Left = 161
      Top = 4
      Width = 66
      Height = 21
      Color = clWhite
      ParentBackground = False
      TabOrder = 1
      OnClick = PanelCurrentAddressClick
    end
    object CheckBoxASCII7Bits: TCheckBox
      Left = 248
      Top = 8
      Width = 129
      Height = 17
      Anchors = [akTop, akRight]
      Caption = 'ASCII 7-bits'
      TabOrder = 2
      OnClick = CheckBoxASCII7BitsClick
    end
    object ButtonRefresh: TButton
      Left = 392
      Top = 4
      Width = 89
      Height = 25
      Anchors = [akTop, akRight]
      Caption = 'Refresh'
      TabOrder = 3
      OnClick = ButtonRefreshClick
    end
    object ButtonMonitoring: TButton
      Left = 487
      Top = 4
      Width = 89
      Height = 25
      Anchors = [akTop, akRight]
      Caption = 'Start'
      TabOrder = 4
      OnClick = ButtonMonitoringClick
    end
  end
  object PopupMenuMemoryContent: TPopupMenu
    Left = 376
    Top = 192
    object SetAsAddress1Menu: TMenuItem
      Caption = 'Set as Address 1'
      OnClick = SetAsAddress1MenuClick
    end
    object SetAsAddress2Menu: TMenuItem
      Caption = 'Set as Address 2'
      OnClick = SetAsAddress2MenuClick
    end
    object N1: TMenuItem
      Caption = '-'
    end
    object SetAsFillStartMenu: TMenuItem
      Caption = 'Set as Fill From'
      OnClick = SetAsFillStartMenuClick
    end
    object SetAsFillEndMenu: TMenuItem
      Caption = 'Set as Fill To'
      OnClick = SetAsFillEndMenuClick
    end
    object SetAsFillDataMenu: TMenuItem
      Caption = 'Set as Fill Data'
      OnClick = SetAsFillDataMenuClick
    end
    object N2: TMenuItem
      Caption = '-'
    end
    object SetAsDumpFromMenu: TMenuItem
      Caption = 'Set as Dump From'
      OnClick = SetAsDumpFromMenuClick
    end
    object SetAsDumpToMenu: TMenuItem
      Caption = 'Set as Dump To'
      OnClick = SetAsDumpToMenuClick
    end
  end
  object SaveDialog: TSaveDialog
    Options = [ofOverwritePrompt, ofHideReadOnly, ofPathMustExist, ofEnableSizing]
    Left = 436
    Top = 144
  end
  object OpenDialog: TOpenDialog
    Options = [ofHideReadOnly, ofFileMustExist, ofEnableSizing]
    Left = 372
    Top = 144
  end
end
