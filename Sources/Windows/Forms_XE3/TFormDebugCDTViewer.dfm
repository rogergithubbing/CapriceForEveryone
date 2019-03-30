object FormDebugCDTViewer: TFormDebugCDTViewer
  Left = 447
  Top = 250
  BorderIcons = [biSystemMenu]
  Caption = 'CDT Tape viewer'
  ClientHeight = 365
  ClientWidth = 784
  Color = clBtnFace
  Constraints.MinHeight = 403
  Constraints.MinWidth = 800
  DoubleBuffered = True
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnHide = FormHide
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object PanelTape: TPanel
    Left = 0
    Top = 0
    Width = 481
    Height = 365
    Align = alLeft
    BevelOuter = bvNone
    ParentColor = True
    TabOrder = 1
    DesignSize = (
      481
      365)
    object GroupBoxTapeData: TGroupBox
      Left = 8
      Top = 0
      Width = 177
      Height = 57
      Caption = ' Tape '
      TabOrder = 0
      DesignSize = (
        177
        57)
      object MemoTapeInfo: TMemo
        Left = 8
        Top = 15
        Width = 161
        Height = 34
        Anchors = [akLeft, akTop, akRight, akBottom]
        BorderStyle = bsNone
        Lines.Strings = (
          'MemoCDTInfo')
        ParentColor = True
        ReadOnly = True
        TabOrder = 0
      end
    end
    object GroupBoxBlockInfo: TGroupBox
      Left = 8
      Top = 64
      Width = 465
      Height = 291
      Anchors = [akLeft, akTop, akRight, akBottom]
      Caption = ' Block '
      TabOrder = 1
      DesignSize = (
        465
        291)
      object PanelBlockType: TPanel
        Left = 8
        Top = 16
        Width = 353
        Height = 25
        Anchors = [akLeft, akTop, akRight]
        ParentBackground = False
        TabOrder = 0
      end
      object PageControlBlock: TPageControl
        Left = 8
        Top = 48
        Width = 449
        Height = 235
        ActivePage = TabSheetBlockInfo
        Anchors = [akLeft, akTop, akRight, akBottom]
        OwnerDraw = True
        TabHeight = 20
        TabOrder = 1
        OnChange = PageControlBlockChange
        OnDrawTab = PageControlBlockDrawTab
        object TabSheetBlockInfo: TTabSheet
          Caption = 'Block Info'
          DesignSize = (
            441
            205)
          object MemoBlockInfo: TMemo
            Left = 8
            Top = 8
            Width = 425
            Height = 191
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
        object TabSheetBlockData: TTabSheet
          Caption = 'Data'
          ImageIndex = 1
          ExplicitLeft = 0
          ExplicitTop = 0
          ExplicitWidth = 0
          ExplicitHeight = 0
          DesignSize = (
            441
            205)
          object MemoBlockData: TMemo
            Left = 8
            Top = 8
            Width = 425
            Height = 191
            Cursor = crArrow
            Anchors = [akLeft, akTop, akRight, akBottom]
            BorderStyle = bsNone
            ParentColor = True
            PopupMenu = PopupMenuClipboard
            ReadOnly = True
            ScrollBars = ssVertical
            TabOrder = 0
            WordWrap = False
          end
        end
        object TabSheetBlockHeader: TTabSheet
          Caption = 'Header'
          ImageIndex = 2
          ExplicitLeft = 0
          ExplicitTop = 0
          ExplicitWidth = 0
          ExplicitHeight = 0
          DesignSize = (
            441
            205)
          object MemoHeaderRecord: TMemo
            Left = 8
            Top = 8
            Width = 425
            Height = 191
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
      end
      object ButtonPlayBlock: TButton
        Left = 368
        Top = 16
        Width = 89
        Height = 25
        Anchors = [akTop, akRight]
        Caption = 'Play'
        TabOrder = 2
        OnClick = ButtonPlayBlockClick
      end
    end
    object PanelControls: TPanel
      Left = 216
      Top = 0
      Width = 257
      Height = 57
      Anchors = [akTop, akRight]
      BevelOuter = bvNone
      ParentColor = True
      TabOrder = 2
      DesignSize = (
        257
        57)
      object ButtonSelectPlayed: TButton
        Left = 128
        Top = 8
        Width = 121
        Height = 33
        Anchors = [akTop, akRight]
        Caption = 'Select played'
        TabOrder = 0
        OnClick = ButtonSelectPlayedClick
        ExplicitLeft = 112
      end
      object ButtonSelect000: TButton
        Left = 16
        Top = 8
        Width = 105
        Height = 33
        Anchors = [akLeft, akRight]
        Caption = 'Select 000'
        TabOrder = 1
        OnClick = ButtonSelect000Click
      end
    end
  end
  object PanelBlocks: TPanel
    Left = 481
    Top = 0
    Width = 303
    Height = 365
    Align = alClient
    BevelOuter = bvNone
    ParentColor = True
    TabOrder = 0
    DesignSize = (
      303
      365)
    object PanelBlocksHeader: TPanel
      Left = 0
      Top = 0
      Width = 303
      Height = 33
      Align = alTop
      BevelOuter = bvNone
      ParentColor = True
      TabOrder = 1
      DesignSize = (
        303
        33)
      object LabelBlockPlayed: TLabel
        Left = 0
        Top = 16
        Width = 49
        Height = 13
        Alignment = taCenter
        Anchors = [akLeft, akBottom]
        AutoSize = False
        Caption = 'Played'
        ExplicitTop = 8
      end
      object LabelBlocks: TLabel
        Left = 56
        Top = 16
        Width = 32
        Height = 13
        Anchors = [akLeft, akBottom]
        Caption = 'Blocks'
        ExplicitTop = 8
      end
      object EditGoToBlock: TEdit
        Left = 192
        Top = 8
        Width = 57
        Height = 21
        Anchors = [akTop, akRight]
        NumbersOnly = True
        TabOrder = 0
        Text = '1'
      end
    end
    object StringGridBlocks: TStringGrid
      Left = 0
      Top = 33
      Width = 303
      Height = 332
      Align = alClient
      BorderStyle = bsNone
      ColCount = 2
      DefaultColWidth = 50
      DefaultRowHeight = 20
      DrawingStyle = gdsGradient
      FixedColor = clWindow
      FixedRows = 0
      GridLineWidth = 0
      Options = [goVertLine, goHorzLine, goThumbTracking, goFixedHotTrack]
      ParentColor = True
      ParentShowHint = False
      ShowHint = False
      TabOrder = 0
      OnDrawCell = StringGridBlocksDrawCell
      OnSelectCell = StringGridBlocksSelectCell
      RowHeights = (
        20
        20
        20
        20
        20)
    end
    object ButtonGoToBlock: TButton
      Left = 248
      Top = 8
      Width = 51
      Height = 21
      Anchors = [akTop, akRight]
      Caption = 'Go'
      TabOrder = 2
      OnClick = ButtonGoToBlockClick
    end
  end
  object PopupMenuClipboard: TPopupMenu
    Left = 577
    Top = 208
    object CopyToClipboardMenu: TMenuItem
      Caption = 'Copy to clipboard'
      OnClick = CopyToClipboardMenuClick
    end
  end
end
