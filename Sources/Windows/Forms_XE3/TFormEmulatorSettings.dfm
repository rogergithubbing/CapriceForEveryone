object FormEmulatorSettings: TFormEmulatorSettings
  Left = 525
  Top = 233
  BorderIcons = []
  Caption = 'Settings'
  ClientHeight = 361
  ClientWidth = 649
  Color = clBtnFace
  Constraints.MinHeight = 399
  Constraints.MinWidth = 665
  DoubleBuffered = True
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnHide = FormHide
  OnResize = FormResize
  OnShow = FormShow
  DesignSize = (
    649
    361)
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBoxMachine: TGroupBox
    Left = 8
    Top = 8
    Width = 257
    Height = 309
    Anchors = [akLeft, akTop, akBottom]
    Caption = ' Machine '
    TabOrder = 1
    DesignSize = (
      257
      309)
    object LabelDevice: TLabel
      Left = 16
      Top = 16
      Width = 73
      Height = 21
      AutoSize = False
      Caption = 'Device'
      Layout = tlCenter
    end
    object LabelBrand: TLabel
      Left = 16
      Top = 48
      Width = 89
      Height = 21
      AutoSize = False
      Caption = 'Brand'
      Layout = tlCenter
    end
    object LabelCRTCType: TLabel
      Left = 16
      Top = 80
      Width = 81
      Height = 21
      AutoSize = False
      Caption = 'CRTC type'
      Layout = tlCenter
    end
    object ComboBoxDevices: TComboBox
      Left = 96
      Top = 16
      Width = 145
      Height = 22
      Style = csOwnerDrawFixed
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 0
      OnChange = ComboBoxDevicesChange
      OnDrawItem = ComboBoxDevicesDrawItem
      Items.Strings = (
        'CPC 464 UK'
        'CPC 664 UK'
        'CPC 6128 UK'
        'CPC 464 FR'
        'CPC 664 FR'
        'CPC 6128 FR'
        'CPC 464 SP'
        'CPC 6128 SP')
    end
    object ComboBoxBrands: TComboBox
      Left = 112
      Top = 48
      Width = 129
      Height = 22
      Style = csOwnerDrawFixed
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 1
      OnDrawItem = ComboBoxBrandsDrawItem
      Items.Strings = (
        'Amstrad'
        'Orion'
        'Schneider'
        'Awa'
        'Solavox'
        'Saisho'
        'Triumph'
        'Isp')
    end
    object CheckBoxUse64kMemoryExtension: TCheckBox
      Left = 16
      Top = 176
      Width = 233
      Height = 25
      Caption = '64K Memory expansion'
      TabOrder = 5
    end
    object CheckBoxUse256kMemoryExtension: TCheckBox
      Left = 16
      Top = 200
      Width = 233
      Height = 25
      Caption = '256K Memory expansion'
      TabOrder = 6
    end
    object CheckBoxUse256kSiliconDisc: TCheckBox
      Left = 16
      Top = 224
      Width = 233
      Height = 25
      Caption = '256K Silicon Disc'
      TabOrder = 7
      OnClick = CheckBoxUse256kSiliconDiscClick
    end
    object CheckBoxUseTechniMusique: TCheckBox
      Left = 16
      Top = 248
      Width = 233
      Height = 25
      Caption = 'TMPI speech synthesizer'
      TabOrder = 8
    end
    object ComboBoxCRTCType: TComboBox
      Left = 104
      Top = 80
      Width = 137
      Height = 22
      BevelInner = bvNone
      BevelOuter = bvNone
      Style = csOwnerDrawFixed
      TabOrder = 2
      OnChange = ComboBoxCRTCTypeChange
      OnDrawItem = ComboBoxCRTCTypeDrawItem
      Items.Strings = (
        '0 - HD6845S'
        '1 - UM6845R')
    end
    object CheckBoxUseFloppyDrive: TCheckBox
      Left = 16
      Top = 128
      Width = 225
      Height = 25
      Caption = 'Floppy Drive'
      TabOrder = 3
      OnClick = CheckBoxUseFloppyDriveClick
    end
    object CheckBoxUseFastFDC: TCheckBox
      Left = 16
      Top = 152
      Width = 209
      Height = 25
      Caption = 'Fast FDC'
      TabOrder = 4
      OnClick = CheckBoxUseFloppyDriveClick
    end
    object CheckBoxUsePlayCity: TCheckBox
      Left = 16
      Top = 272
      Width = 201
      Height = 25
      Caption = 'PlayCity'
      TabOrder = 9
    end
  end
  object PanelButtons: TPanel
    Left = 0
    Top = 320
    Width = 649
    Height = 41
    Align = alBottom
    BevelOuter = bvNone
    ParentColor = True
    TabOrder = 0
    DesignSize = (
      649
      41)
    object ButtonOK: TButton
      Left = 432
      Top = 8
      Width = 121
      Height = 25
      Anchors = [akRight, akBottom]
      Caption = 'Apply'
      ModalResult = 1
      TabOrder = 1
      OnClick = ButtonOKClick
    end
    object ButtonCancel: TButton
      Left = 560
      Top = 8
      Width = 81
      Height = 25
      Anchors = [akRight, akBottom]
      Cancel = True
      Caption = 'Cancel'
      Default = True
      ModalResult = 2
      TabOrder = 0
    end
    object ButtonDefaultROMMapping: TButton
      Left = 8
      Top = 8
      Width = 257
      Height = 25
      Caption = 'Restore default ROM mapping'
      TabOrder = 2
      OnClick = ButtonDefaultROMMappingClick
    end
  end
  object GroupBoxROMMapping: TGroupBox
    Left = 272
    Top = 8
    Width = 369
    Height = 309
    Anchors = [akLeft, akTop, akRight, akBottom]
    Caption = ' ROM mapping '
    TabOrder = 2
    DesignSize = (
      369
      309)
    object StringGridROMMapping: TStringGrid
      Left = 2
      Top = 16
      Width = 365
      Height = 254
      Anchors = [akLeft, akTop, akRight, akBottom]
      BevelEdges = [beBottom]
      BevelInner = bvNone
      BevelKind = bkSoft
      BorderStyle = bsNone
      ColCount = 2
      DefaultColWidth = 72
      DefaultRowHeight = 21
      RowCount = 17
      FixedRows = 0
      Options = [goRowSelect, goThumbTracking]
      ParentColor = True
      ScrollBars = ssVertical
      TabOrder = 0
      OnDrawCell = StringGridROMMappingDrawCell
      OnKeyDown = StringGridROMMappingKeyDown
      OnMouseDown = StringGridROMMappingMouseDown
      OnSelectCell = StringGridROMMappingSelectCell
      RowHeights = (
        21
        21
        21
        21
        21
        21
        21
        21
        21
        21
        21
        21
        21
        21
        21
        21
        21)
    end
    object PanelROMSettings: TPanel
      Left = 2
      Top = 270
      Width = 365
      Height = 37
      Align = alBottom
      BevelOuter = bvNone
      ParentColor = True
      TabOrder = 1
      DesignSize = (
        365
        37)
      object LabelSelectROM: TLabel
        Left = 8
        Top = 10
        Width = 97
        Height = 18
        AutoSize = False
        Caption = 'Select ROM'
        Layout = tlCenter
      end
      object ComboBoxAvailableROM: TComboBox
        Left = 112
        Top = 10
        Width = 243
        Height = 18
        Style = csOwnerDrawFixed
        Anchors = [akLeft, akTop, akRight]
        ItemHeight = 12
        TabOrder = 0
        OnChange = ComboBoxAvailableROMChange
        OnDrawItem = ComboBoxAvailableROMDrawItem
      end
    end
  end
end
