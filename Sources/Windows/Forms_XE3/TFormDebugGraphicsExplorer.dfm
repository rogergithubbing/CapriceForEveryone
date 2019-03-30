object FormGraphicsExplorer: TFormGraphicsExplorer
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  Caption = 'Graphics explorer'
  ClientHeight = 338
  ClientWidth = 681
  Color = clBtnFace
  Constraints.MinHeight = 255
  Constraints.MinWidth = 132
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesigned
  OnDestroy = FormDestroy
  OnHide = FormHide
  OnResize = FormResize
  OnShow = FormShow
  DesignSize = (
    681
    338)
  PixelsPerInch = 96
  TextHeight = 13
  object LabelCursorAddress: TLabel
    Left = 496
    Top = 136
    Width = 57
    Height = 21
    Hint = 'Horizontal Total'
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'Cursor'
    Layout = tlCenter
  end
  object LabelTileAddress: TLabel
    Left = 288
    Top = 136
    Width = 41
    Height = 21
    Hint = 'Horizontal Total'
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'Tile'
    Layout = tlCenter
  end
  object LabelTopLeftAddress: TLabel
    Left = 112
    Top = 136
    Width = 73
    Height = 21
    Hint = 'Horizontal Total'
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'Top Left'
    Layout = tlCenter
  end
  object SpeedButtonEdit: TSpeedButton
    Left = 8
    Top = 104
    Width = 97
    Height = 25
    Caption = 'Edit Tile'
    OnClick = SpeedButtonEditClick
  end
  object StringGridTiles: TStringGrid
    Left = 8
    Top = 168
    Width = 665
    Height = 139
    Anchors = [akLeft, akTop, akRight, akBottom]
    BevelInner = bvNone
    BevelOuter = bvNone
    BiDiMode = bdLeftToRight
    BorderStyle = bsNone
    Color = clGrayText
    Ctl3D = True
    DefaultColWidth = 20
    DefaultRowHeight = 20
    DefaultDrawing = False
    DoubleBuffered = True
    DrawingStyle = gdsGradient
    FixedColor = clWindow
    FixedCols = 0
    FixedRows = 0
    GridLineWidth = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goThumbTracking]
    ParentBiDiMode = False
    ParentCtl3D = False
    ParentDoubleBuffered = False
    ParentShowHint = False
    PopupMenu = PopupMenuTiles
    ScrollBars = ssNone
    ShowHint = False
    TabOrder = 0
    OnDblClick = StringGridTilesDblClick
    OnDrawCell = StringGridTilesDrawCell
    OnMouseLeave = StringGridTilesMouseLeave
    OnMouseMove = StringGridTilesMouseMove
    OnSelectCell = StringGridTilesSelectCell
  end
  object ScrollBarAddress: TScrollBar
    Left = 48
    Top = 315
    Width = 585
    Height = 17
    Anchors = [akLeft, akRight, akBottom]
    DoubleBuffered = False
    Max = 65535
    PageSize = 100
    ParentDoubleBuffered = False
    TabOrder = 1
    OnChange = ScrollBarAddressChange
  end
  object ButtonByteDecrement: TButton
    Left = 8
    Top = 315
    Width = 33
    Height = 17
    Anchors = [akLeft, akBottom]
    Caption = '-1'
    TabOrder = 2
    OnClick = ButtonByteDecrementClick
  end
  object ButtonByteIncrement: TButton
    Left = 640
    Top = 315
    Width = 33
    Height = 17
    Anchors = [akRight, akBottom]
    Caption = '+1'
    TabOrder = 3
    OnClick = ButtonByteIncrementClick
  end
  object PanelInfo: TPanel
    Left = 8
    Top = 136
    Width = 97
    Height = 21
    Alignment = taLeftJustify
    BevelOuter = bvNone
    DoubleBuffered = True
    ParentColor = True
    ParentDoubleBuffered = False
    TabOrder = 4
  end
  object PageControlExplorer: TPageControl
    Left = 112
    Top = 8
    Width = 561
    Height = 121
    ActivePage = TabSheetMapper
    Anchors = [akLeft, akTop, akRight]
    DoubleBuffered = True
    OwnerDraw = True
    ParentDoubleBuffered = False
    TabHeight = 20
    TabOrder = 5
    OnChange = PageControlExplorerChange
    OnChanging = PageControlExplorerChanging
    OnDrawTab = PageControlExplorerDrawTab
    object TabSheetTiles: TTabSheet
      Caption = 'Tiles'
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object LabelMode: TLabel
        Left = 312
        Top = 8
        Width = 89
        Height = 21
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'Mode'
        Layout = tlCenter
      end
      object LabelAddressing: TLabel
        Left = 312
        Top = 32
        Width = 89
        Height = 21
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'Addressing'
        Layout = tlCenter
      end
      object LabelZoom: TLabel
        Left = 8
        Top = 8
        Width = 41
        Height = 21
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'Zoom'
        Layout = tlCenter
      end
      object LabelTileX: TLabel
        Left = 160
        Top = 8
        Width = 49
        Height = 21
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'Size X'
        Layout = tlCenter
      end
      object LabelTileY: TLabel
        Left = 160
        Top = 32
        Width = 49
        Height = 21
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'Y'
        Layout = tlCenter
      end
      object LabelMemory: TLabel
        Left = 312
        Top = 56
        Width = 89
        Height = 21
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'Memory'
        Layout = tlCenter
      end
      object ComboBoxMode: TComboBox
        Left = 408
        Top = 8
        Width = 129
        Height = 22
        Hint = 'Force graphic mode or keep current.'
        Style = csOwnerDrawFixed
        TabOrder = 0
        OnChange = ComboBoxModeChange
        OnDrawItem = ComboBoxDrawItem
        Items.Strings = (
          'Current'
          '0'
          '1'
          '2')
      end
      object ComboBoxAddressing: TComboBox
        Left = 408
        Top = 32
        Width = 129
        Height = 22
        Hint = 'Select addressing mode.'
        Style = csOwnerDrawFixed
        TabOrder = 1
        OnChange = ComboBoxAddressingChange
        OnDrawItem = ComboBoxDrawItem
        Items.Strings = (
          'Linear'
          'Screen')
      end
      object EditZoom: TEdit
        Left = 56
        Top = 8
        Width = 49
        Height = 21
        Hint = 'Display zoom'
        NumbersOnly = True
        TabOrder = 2
        Text = '1'
        OnChange = EditZoomChange
      end
      object UpDownZoom: TUpDown
        Left = 105
        Top = 8
        Width = 16
        Height = 21
        Associate = EditZoom
        Min = 1
        Position = 1
        TabOrder = 3
      end
      object EditTileX: TEdit
        Left = 216
        Top = 8
        Width = 57
        Height = 21
        Hint = 'Tile size X'
        NumbersOnly = True
        TabOrder = 4
        Text = '2'
        OnChange = EditTileXChange
      end
      object UpDownTileX: TUpDown
        Left = 273
        Top = 8
        Width = 16
        Height = 21
        Associate = EditTileX
        Min = 1
        Position = 2
        TabOrder = 5
      end
      object EditTileY: TEdit
        Left = 216
        Top = 32
        Width = 57
        Height = 21
        Hint = 'Tile size Y'
        NumbersOnly = True
        TabOrder = 6
        Text = '8'
        OnChange = EditTileYChange
      end
      object UpDownTileY: TUpDown
        Left = 273
        Top = 32
        Width = 16
        Height = 21
        Associate = EditTileY
        Min = 1
        Position = 8
        TabOrder = 7
      end
      object ComboBoxMemory: TComboBox
        Left = 408
        Top = 56
        Width = 129
        Height = 22
        Hint = 'Select memory content.'
        Style = csOwnerDrawFixed
        TabOrder = 8
        OnChange = ComboBoxMemoryChange
        OnDrawItem = ComboBoxDrawItem
        Items.Strings = (
          'Banked'
          'RAM')
      end
    end
    object TabSheetMapper: TTabSheet
      Caption = 'Mapper'
      ImageIndex = 1
      DesignSize = (
        553
        91)
      object LabelFirstTile: TLabel
        Left = 8
        Top = 8
        Width = 81
        Height = 21
        Hint = 'Horizontal Total'
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'First tile'
        Layout = tlCenter
      end
      object LabelLastTile: TLabel
        Left = 8
        Top = 32
        Width = 81
        Height = 21
        Hint = 'Horizontal Total'
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'Last tile'
        Layout = tlCenter
      end
      object LabelMapStart: TLabel
        Left = 200
        Top = 8
        Width = 89
        Height = 21
        Hint = 'Horizontal Total'
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'Map start'
        Layout = tlCenter
      end
      object LabelMapSizeX: TLabel
        Left = 400
        Top = 8
        Width = 49
        Height = 21
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'Size X'
        Layout = tlCenter
      end
      object LabelMapSizeY: TLabel
        Left = 400
        Top = 32
        Width = 49
        Height = 21
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'Y'
        Layout = tlCenter
      end
      object LabelMapperTiles: TLabel
        Left = 48
        Top = 56
        Width = 41
        Height = 21
        Hint = 'Horizontal Total'
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'Tiles'
        Layout = tlCenter
      end
      object LabelMapperIndexing: TLabel
        Left = 200
        Top = 64
        Width = 89
        Height = 21
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'Indexing'
        Layout = tlCenter
      end
      object PanelFormatFirstTile: TPanel
        Left = 96
        Top = 8
        Width = 17
        Height = 21
        Caption = '&&'
        Color = 16773087
        ParentBackground = False
        TabOrder = 0
        OnClick = PanelFormatFirstTileClick
      end
      object PanelFirstTile: TPanel
        Left = 113
        Top = 8
        Width = 72
        Height = 21
        Hint = 'Address of the first tile'
        Color = clWhite
        ParentBackground = False
        PopupMenu = PopupMenuMapper
        TabOrder = 1
        OnClick = PanelFirstTileClick
      end
      object PanelLastTile: TPanel
        Left = 113
        Top = 32
        Width = 72
        Height = 21
        Hint = 'Address of the last tile'
        Color = clWhite
        ParentBackground = False
        PopupMenu = PopupMenuMapper
        TabOrder = 2
        OnClick = PanelLastTileClick
      end
      object PanelFormatMapStart: TPanel
        Left = 296
        Top = 8
        Width = 17
        Height = 21
        Caption = '&&'
        Color = 16773087
        ParentBackground = False
        TabOrder = 3
        OnClick = PanelFormatMapStartClick
      end
      object PanelMapStart: TPanel
        Left = 313
        Top = 8
        Width = 72
        Height = 21
        Hint = 'Address of the map start'
        Color = clWhite
        ParentBackground = False
        PopupMenu = PopupMenuMapper
        TabOrder = 4
        OnClick = PanelMapStartClick
      end
      object EditMapX: TEdit
        Left = 472
        Top = 8
        Width = 57
        Height = 21
        Hint = 'Map size X'
        NumbersOnly = True
        TabOrder = 5
        Text = '32'
        OnChange = EditMapXChange
      end
      object UpDownMapX: TUpDown
        Left = 529
        Top = 8
        Width = 16
        Height = 21
        Associate = EditMapX
        Min = 1
        Max = 999
        Position = 32
        TabOrder = 6
      end
      object EditMapY: TEdit
        Left = 472
        Top = 32
        Width = 57
        Height = 21
        Hint = 'Map size Y'
        NumbersOnly = True
        TabOrder = 7
        Text = '32'
        OnChange = EditMapYChange
      end
      object UpDownMapY: TUpDown
        Left = 529
        Top = 32
        Width = 16
        Height = 21
        Associate = EditMapY
        Min = 1
        Max = 999
        Position = 32
        TabOrder = 8
      end
      object ComboBoxIndexing: TComboBox
        Left = 296
        Top = 64
        Width = 249
        Height = 22
        Hint = 'Select how to build map.'
        Style = csOwnerDrawFixed
        Anchors = [akLeft, akTop, akRight]
        TabOrder = 9
        OnChange = ComboBoxIndexingChange
        OnDrawItem = ComboBoxDrawItem
        Items.Strings = (
          'Linear bytes'
          'Compressed RL0 (Sorcery)'
          'Linear words Big Endian (Prohibition)')
      end
      object EditMapperTiles: TEdit
        Left = 112
        Top = 56
        Width = 73
        Height = 21
        Hint = 'Number of tiles'
        NumbersOnly = True
        TabOrder = 10
        Text = '1'
        OnChange = EditMapperTilesChange
      end
      object UpDownMapperTiles: TUpDown
        Left = 185
        Top = 56
        Width = 16
        Height = 21
        Associate = EditMapperTiles
        Min = 1
        Max = 1024
        Position = 1
        TabOrder = 11
      end
      object PanelMapXReverse: TPanel
        Left = 455
        Top = 9
        Width = 17
        Height = 21
        Hint = 'Reverse X'
        Color = 16773087
        ParentBackground = False
        TabOrder = 12
        OnClick = PanelMapXReverseClick
      end
      object PanelMapYReverse: TPanel
        Left = 455
        Top = 33
        Width = 17
        Height = 21
        Hint = 'Reverse Y'
        Color = 16773087
        ParentBackground = False
        TabOrder = 13
        OnClick = PanelMapYReverseClick
      end
    end
  end
  object ButtonMonitor: TButton
    Left = 8
    Top = 8
    Width = 97
    Height = 25
    Hint = 'Start / Stop real time refresh'
    Caption = 'Start'
    TabOrder = 6
    OnClick = ButtonMonitorClick
  end
  object ButtonRefresh: TButton
    Left = 8
    Top = 40
    Width = 97
    Height = 25
    Hint = 'Refresh display'
    Caption = 'Refresh'
    TabOrder = 7
    OnClick = ButtonRefreshClick
  end
  object PanelCursorAddress: TPanel
    Left = 560
    Top = 136
    Width = 73
    Height = 21
    Hint = 'Address under the mouse cursor'
    BevelInner = bvLowered
    ParentColor = True
    TabOrder = 8
  end
  object PanelTileAddress: TPanel
    Left = 336
    Top = 136
    Width = 73
    Height = 21
    Hint = 'Address of the selected tile'
    BevelInner = bvLowered
    ParentColor = True
    TabOrder = 9
  end
  object PanelFormatAddress: TPanel
    Left = 192
    Top = 136
    Width = 17
    Height = 21
    Caption = '&&'
    Color = 16773087
    ParentBackground = False
    TabOrder = 10
    OnClick = PanelFormatAddressClick
  end
  object PanelAddress: TPanel
    Left = 209
    Top = 136
    Width = 72
    Height = 21
    Hint = 'Address of the top left tile'
    Color = clWhite
    ParentBackground = False
    TabOrder = 11
    OnClick = PanelAddressClick
  end
  object PanelCursorData: TPanel
    Left = 632
    Top = 136
    Width = 41
    Height = 21
    Hint = 'Data under the mouse cursor'
    BevelInner = bvLowered
    ParentColor = True
    TabOrder = 12
  end
  object ButtonMapperCopy: TButton
    Left = 8
    Top = 72
    Width = 97
    Height = 25
    Hint = 'Copy shown tiles or map preview to clipboard.'
    Caption = 'Copy'
    TabOrder = 13
    OnClick = ButtonMapperCopyClick
  end
  object PanelTileIndex: TPanel
    Left = 408
    Top = 136
    Width = 57
    Height = 21
    Hint = 'Index of the selected tile from first tile'
    BevelInner = bvLowered
    ParentColor = True
    TabOrder = 14
  end
  object PopupMenuTiles: TPopupMenu
    Left = 80
    Top = 176
    object EditTileMenu: TMenuItem
      Caption = 'Edit Selected Tile'
      OnClick = EditTileMenuClick
    end
    object N1: TMenuItem
      Caption = '-'
    end
    object TileAsTopLeftMenu: TMenuItem
      Caption = 'Selected Tile as Top Left'
      OnClick = TileAsTopLeftMenuClick
    end
    object CursorAsTopLeftMenu: TMenuItem
      Caption = 'Cursor as Top Left'
      OnClick = CursorAsTopLeftMenuClick
    end
    object N2: TMenuItem
      Caption = '-'
    end
    object SelectedTileAsFirstTileMenu: TMenuItem
      Caption = 'Selected Tile as First Tile'
      OnClick = SelectedTileAsFirstTileMenuClick
    end
    object SelectedTileAsLastTileMenu: TMenuItem
      Caption = 'Selected Tile as Last Tile'
      OnClick = SelectedTileAsLastTileMenuClick
    end
  end
  object PopupMenuMapper: TPopupMenu
    Left = 176
    Top = 176
    object MapperSetTopLeftTileAddressMenu: TMenuItem
      Caption = 'Set Top Left Tile address'
      OnClick = MapperSetTopLeftTileAddressMenuClick
    end
    object MapperSetSelectedTileAddressMenu: TMenuItem
      Caption = 'Set Selected Tile address'
      OnClick = MapperSetSelectedTileAddressMenuClick
    end
  end
end
