object FormDebugFDC: TFormDebugFDC
  Left = 385
  Top = 349
  BorderIcons = [biSystemMenu]
  Caption = 'FDC'
  ClientHeight = 249
  ClientWidth = 721
  Color = clBtnFace
  Constraints.MinHeight = 307
  Constraints.MinWidth = 737
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  Position = poMainFormCenter
  OnHide = FormHide
  OnResize = FormResize
  OnShow = FormShow
  DesignSize = (
    721
    249)
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBoxCurrentState: TGroupBox
    Left = 8
    Top = 8
    Width = 201
    Height = 233
    Caption = ' Status '
    TabOrder = 1
    object LabelStateMotor: TLabel
      Left = 16
      Top = 23
      Width = 105
      Height = 21
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Motor'
      Layout = tlCenter
    end
    object LabelStateNbTracks: TLabel
      Left = 8
      Top = 48
      Width = 113
      Height = 21
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Nb tracks'
      Layout = tlCenter
    end
    object LabelStateCurrentTrack: TLabel
      Left = 8
      Top = 80
      Width = 113
      Height = 21
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Current track'
      Layout = tlCenter
    end
    object LabelStateNbSectors: TLabel
      Left = 8
      Top = 104
      Width = 113
      Height = 21
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Nb sectors'
      Layout = tlCenter
    end
    object PanelStateMotor: TPanel
      Left = 128
      Top = 23
      Width = 57
      Height = 21
      Hint = 'Horizontal Total'
      ParentBackground = False
      ParentColor = True
      TabOrder = 0
    end
    object PanelFormatStateNbTracks: TPanel
      Left = 128
      Top = 48
      Width = 17
      Height = 21
      Color = 16773087
      ParentBackground = False
      TabOrder = 1
      OnClick = PanelFormatStateNbTracksClick
      OnDblClick = PanelFormatStateNbTracksClick
    end
    object PanelStateNbTracks: TPanel
      Left = 145
      Top = 48
      Width = 40
      Height = 21
      Hint = 'Horizontal Total'
      ParentBackground = False
      ParentColor = True
      TabOrder = 2
    end
    object PanelFormatStateCurrentTrack: TPanel
      Left = 128
      Top = 80
      Width = 17
      Height = 21
      Color = 16773087
      ParentBackground = False
      TabOrder = 3
      OnClick = PanelFormatStateCurrentTrackClick
      OnDblClick = PanelFormatStateCurrentTrackClick
    end
    object PanelStateCurrentTrack: TPanel
      Left = 145
      Top = 80
      Width = 40
      Height = 21
      Hint = 'Horizontal Total'
      ParentBackground = False
      ParentColor = True
      TabOrder = 4
    end
    object PanelFormatStateNbSectors: TPanel
      Left = 128
      Top = 104
      Width = 17
      Height = 21
      Color = 16773087
      ParentBackground = False
      TabOrder = 5
      OnClick = PanelFormatStateNbSectorsClick
      OnDblClick = PanelFormatStateNbSectorsClick
    end
    object PanelStateNbSectors: TPanel
      Left = 145
      Top = 104
      Width = 40
      Height = 21
      Hint = 'Horizontal Total'
      ParentBackground = False
      ParentColor = True
      TabOrder = 6
    end
    object GroupBoxCurrentSector: TGroupBox
      Left = 8
      Top = 144
      Width = 185
      Height = 81
      Caption = ' Current sector '
      TabOrder = 7
      object LabelStateCurrentSectorIndex: TLabel
        Left = 16
        Top = 20
        Width = 97
        Height = 21
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'Index'
        Layout = tlCenter
      end
      object LabelStateCurrentSector: TLabel
        Left = 16
        Top = 44
        Width = 97
        Height = 21
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'ID'
        Layout = tlCenter
      end
      object PanelFormatStateCurrentSectorIndex: TPanel
        Left = 120
        Top = 20
        Width = 17
        Height = 21
        Color = 16773087
        ParentBackground = False
        TabOrder = 0
        OnClick = PanelFormatStateCurrentSectorIndexClick
        OnDblClick = PanelFormatStateCurrentSectorIndexClick
      end
      object PanelStateCurrentSectorIndex: TPanel
        Left = 137
        Top = 20
        Width = 40
        Height = 21
        Hint = 'Horizontal Total'
        ParentBackground = False
        ParentColor = True
        TabOrder = 1
      end
      object PanelFormatStateCurrentSector: TPanel
        Left = 120
        Top = 44
        Width = 17
        Height = 21
        Caption = '&&'
        Color = 16773087
        ParentBackground = False
        TabOrder = 2
        OnClick = PanelFormatStateCurrentSectorClick
        OnDblClick = PanelFormatStateCurrentSectorClick
      end
      object PanelStateCurrentSector: TPanel
        Left = 137
        Top = 44
        Width = 40
        Height = 21
        Hint = 'Horizontal Total'
        ParentBackground = False
        ParentColor = True
        TabOrder = 3
      end
    end
  end
  object GroupBoxHistory: TGroupBox
    Left = 216
    Top = 8
    Width = 497
    Height = 233
    Anchors = [akLeft, akTop, akRight, akBottom]
    Caption = ' Monitor '
    TabOrder = 0
    object PanelHistoryControls: TPanel
      Left = 2
      Top = 15
      Width = 493
      Height = 98
      Align = alTop
      BevelOuter = bvNone
      ParentColor = True
      TabOrder = 0
      DesignSize = (
        493
        98)
      object ButtonHistoryStartStop: TButton
        Left = 8
        Top = 8
        Width = 380
        Height = 25
        Anchors = [akLeft, akTop, akRight]
        Caption = 'Start'
        TabOrder = 0
        OnClick = ButtonHistoryStartStopClick
      end
      object ButtonHistoryClear: TButton
        Left = 395
        Top = 8
        Width = 89
        Height = 25
        Anchors = [akTop, akRight]
        Caption = 'Clear'
        TabOrder = 1
        OnClick = ButtonHistoryClearClick
      end
      object CheckBoxShowReadStatus: TCheckBox
        Left = 8
        Top = 40
        Width = 153
        Height = 17
        Caption = 'Show Read Status'
        TabOrder = 2
      end
      object CheckBoxShowReadData: TCheckBox
        Left = 8
        Top = 56
        Width = 153
        Height = 17
        Caption = 'Show Read Data'
        TabOrder = 3
      end
      object CheckBoxShowWriteData: TCheckBox
        Left = 8
        Top = 72
        Width = 153
        Height = 17
        Caption = 'Show Write Data'
        TabOrder = 4
      end
      object CheckBoxShowDiskIndex: TCheckBox
        Left = 168
        Top = 41
        Width = 145
        Height = 17
        Caption = 'Show Disk Index'
        TabOrder = 5
      end
      object CheckBoxShowDiskSector: TCheckBox
        Left = 168
        Top = 73
        Width = 145
        Height = 17
        Caption = 'Show Disk Sector'
        TabOrder = 6
      end
      object CheckBoxShowDiskTrack: TCheckBox
        Left = 168
        Top = 57
        Width = 145
        Height = 17
        Caption = 'Show Disk Track'
        TabOrder = 7
      end
      object CheckBoxAddTimestamp: TCheckBox
        Left = 320
        Top = 41
        Width = 161
        Height = 17
        Caption = 'Add timestamp ('#181's)'
        TabOrder = 8
      end
      object CheckBoxAddInterval: TCheckBox
        Left = 320
        Top = 57
        Width = 161
        Height = 17
        Caption = 'Add interval ('#181's)'
        TabOrder = 9
      end
    end
    object ListBoxHistory: TListBox
      Left = 2
      Top = 113
      Width = 493
      Height = 117
      Align = alClient
      BorderStyle = bsNone
      IntegralHeight = True
      ItemHeight = 13
      ParentColor = True
      PopupMenu = PopupMenuHistory
      TabOrder = 1
      TabWidth = 30
    end
  end
  object MainMenu1: TMainMenu
    Left = 241
    Top = 144
    object HistoryMenu: TMenuItem
      Caption = 'History'
      object CopytoclipboardMenu: TMenuItem
        Caption = 'Copy to clipboard'
        OnClick = CopytoclipboardMenuClick
      end
    end
  end
  object PopupMenuHistory: TPopupMenu
    Left = 273
    Top = 144
    object Copytoclipboard1: TMenuItem
      Caption = 'Copy to clipboard'
      OnClick = Copytoclipboard1Click
    end
  end
end
