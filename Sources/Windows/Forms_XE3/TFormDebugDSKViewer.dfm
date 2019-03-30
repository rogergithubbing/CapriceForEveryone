object FormDebugDSKViewer: TFormDebugDSKViewer
  Left = 437
  Top = 180
  BorderIcons = [biSystemMenu]
  Caption = 'Disk viewer'
  ClientHeight = 325
  ClientWidth = 664
  Color = clBtnFace
  Constraints.MinHeight = 363
  Constraints.MinWidth = 680
  DoubleBuffered = True
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
  object PanelSectorData: TPanel
    Left = 0
    Top = 0
    Width = 416
    Height = 325
    Align = alLeft
    BevelOuter = bvNone
    ParentColor = True
    TabOrder = 0
    object PanelDrive: TPanel
      Left = 0
      Top = 0
      Width = 416
      Height = 41
      Align = alTop
      BevelOuter = bvNone
      ParentColor = True
      TabOrder = 0
      DesignSize = (
        416
        41)
      object ComboBoxDrive: TComboBox
        Left = 8
        Top = 8
        Width = 89
        Height = 22
        Style = csOwnerDrawFixed
        TabOrder = 0
        OnChange = ComboBoxDriveChange
        OnDrawItem = ComboBoxDriveDrawItem
        Items.Strings = (
          'Drive A'
          'Drive B')
      end
      object ButtonMonitoring: TButton
        Left = 184
        Top = 8
        Width = 113
        Height = 25
        Anchors = [akLeft, akTop, akRight]
        Caption = 'Start'
        TabOrder = 1
        OnClick = ButtonMonitoringClick
      end
      object PanelSectorState: TPanel
        Left = 304
        Top = 8
        Width = 105
        Height = 25
        Anchors = [akTop, akRight]
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentBackground = False
        ParentFont = False
        TabOrder = 2
      end
      object ComboBoxSides: TComboBox
        Left = 96
        Top = 8
        Width = 81
        Height = 22
        Style = csOwnerDrawFixed
        TabOrder = 3
        OnChange = ComboBoxSidesChange
        OnDrawItem = ComboBoxSidesDrawItem
        Items.Strings = (
          'Drive A'
          'Drive B')
      end
    end
    object PanelInfos: TPanel
      Left = 0
      Top = 41
      Width = 416
      Height = 112
      Align = alTop
      BevelOuter = bvNone
      ParentColor = True
      TabOrder = 1
      DesignSize = (
        416
        112)
      object GroupBoxTrack: TGroupBox
        Left = 216
        Top = 0
        Width = 193
        Height = 105
        Anchors = [akLeft, akTop, akRight]
        Caption = ' Track '
        TabOrder = 0
        DesignSize = (
          193
          105)
        object MemoTrackInfo: TMemo
          Left = 8
          Top = 15
          Width = 177
          Height = 82
          Anchors = [akLeft, akTop, akRight, akBottom]
          BorderStyle = bsNone
          ParentColor = True
          PopupMenu = PopupMenuClipboard
          ReadOnly = True
          ScrollBars = ssVertical
          TabOrder = 0
        end
      end
      object GroupBoxDisk: TGroupBox
        Left = 8
        Top = 0
        Width = 201
        Height = 105
        Caption = ' Disk '
        TabOrder = 1
        DesignSize = (
          201
          105)
        object MemoDiskInfo: TMemo
          Left = 8
          Top = 15
          Width = 185
          Height = 82
          Anchors = [akLeft, akTop, akRight, akBottom]
          BorderStyle = bsNone
          ParentColor = True
          PopupMenu = PopupMenuClipboard
          ReadOnly = True
          TabOrder = 0
        end
      end
    end
    object PageControlData: TPageControl
      Left = 0
      Top = 153
      Width = 416
      Height = 172
      ActivePage = TabSheetDataDeclared
      Align = alClient
      OwnerDraw = True
      TabHeight = 20
      TabOrder = 2
      OnChange = PageControlDataChange
      OnDrawTab = PageControlDataDrawTab
      object TabSheetSector: TTabSheet
        Caption = 'Sector'
        ImageIndex = 3
        DesignSize = (
          408
          142)
        object MemoSectorInfo: TMemo
          Left = 8
          Top = 8
          Width = 400
          Height = 129
          Cursor = crArrow
          Anchors = [akLeft, akTop, akRight, akBottom]
          BorderStyle = bsNone
          ParentColor = True
          PopupMenu = PopupMenuClipboard
          ReadOnly = True
          ScrollBars = ssVertical
          TabOrder = 0
        end
      end
      object TabSheetDataSector: TTabSheet
        Caption = 'Data'
        DesignSize = (
          408
          142)
        object MemoDataSector: TMemo
          Left = 8
          Top = 8
          Width = 400
          Height = 129
          Cursor = crArrow
          Anchors = [akLeft, akTop, akRight, akBottom]
          BorderStyle = bsNone
          ParentColor = True
          PopupMenu = PopupMenuData
          ReadOnly = True
          ScrollBars = ssVertical
          TabOrder = 0
          WordWrap = False
        end
      end
      object TabSheetDataGAP3: TTabSheet
        Caption = 'GAP3'
        ImageIndex = 1
        DesignSize = (
          408
          142)
        object MemoDataGAP3: TMemo
          Left = 8
          Top = 8
          Width = 400
          Height = 129
          Cursor = crArrow
          Anchors = [akLeft, akTop, akRight, akBottom]
          BorderStyle = bsNone
          ParentColor = True
          PopupMenu = PopupMenuData
          ReadOnly = True
          ScrollBars = ssVertical
          TabOrder = 0
          WordWrap = False
        end
      end
      object TabSheetDataDeclared: TTabSheet
        Caption = 'Declared data'
        ImageIndex = 2
        DesignSize = (
          408
          142)
        object MemoDataDeclared: TMemo
          Left = 8
          Top = 8
          Width = 400
          Height = 129
          Cursor = crArrow
          Anchors = [akLeft, akTop, akRight, akBottom]
          BorderStyle = bsNone
          ParentColor = True
          PopupMenu = PopupMenuData
          ReadOnly = True
          ScrollBars = ssVertical
          TabOrder = 0
          WordWrap = False
        end
      end
    end
  end
  object StringGridSectors: TStringGrid
    Left = 416
    Top = 0
    Width = 248
    Height = 325
    Align = alClient
    BevelInner = bvNone
    BevelOuter = bvNone
    BiDiMode = bdLeftToRight
    BorderStyle = bsNone
    Color = clGrayText
    Ctl3D = True
    DefaultColWidth = 20
    DefaultRowHeight = 20
    DoubleBuffered = False
    DrawingStyle = gdsGradient
    FixedColor = clWindow
    FixedCols = 0
    GridLineWidth = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goThumbTracking]
    ParentBiDiMode = False
    ParentCtl3D = False
    ParentDoubleBuffered = False
    ParentShowHint = False
    ShowHint = False
    TabOrder = 1
    OnDrawCell = StringGridSectorsDrawCell
    OnSelectCell = StringGridSectorsSelectCell
  end
  object PopupMenuClipboard: TPopupMenu
    Left = 449
    Top = 216
    object CopyToClipboardMenu: TMenuItem
      Caption = 'Copy to clipboard'
      OnClick = CopyToClipboardMenuClick
    end
  end
  object PopupMenuData: TPopupMenu
    Left = 481
    Top = 216
    object MenuItem1: TMenuItem
      Caption = 'Copy to clipboard'
      OnClick = CopyToClipboardMenuClick
    end
    object N1: TMenuItem
      Caption = '-'
    end
    object ASCII7bitsMenu: TMenuItem
      Caption = 'ASCII 7-bits'
      OnClick = ASCII7bitsMenuClick
    end
  end
end
