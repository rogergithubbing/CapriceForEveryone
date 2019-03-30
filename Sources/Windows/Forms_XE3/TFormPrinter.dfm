object FormPrinter: TFormPrinter
  Left = 288
  Top = 199
  BorderIcons = [biSystemMenu]
  Caption = 'Printer'
  ClientHeight = 326
  ClientWidth = 740
  Color = clBtnFace
  Constraints.MinHeight = 310
  Constraints.MinWidth = 756
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnDestroy = FormDestroy
  OnHide = FormHide
  OnResize = FormResize
  OnShow = FormShow
  DesignSize = (
    740
    326)
  PixelsPerInch = 96
  TextHeight = 13
  object ImageBorderTile: TImage
    Left = 352
    Top = 8
    Width = 25
    Height = 25
    Picture.Data = {
      07544269746D61702E060000424D2E0600000000000036040000280000001500
      0000150000000100080000000000F8010000130B0000130B0000000100000000
      000062686D0063696E00686E7300696F74006A7075006D737800747A7F00757B
      8000787E8300797F84007B8186007E8489007F858A00848A8F00868C91008A90
      95008C9297008E949900959BA000979DA2009BA1A6009CA2A7009DA3A8009EA4
      A9009FA5AA00A1A7AC00A2A8AD00ACB2B700C4CACF00C8CED300CDD3D800D0D6
      DB00D3D9DE00D5DEE200DBE1E600DDE3E800E1E7EC00E4EAEF00E8EEF300EBF1
      F600ECF2F700F0F6FB00F2F8FD00F5FBFF00F6FCFF00F7FDFF00F8FEFF00F8FF
      FF00F9FFFF00FAFEFF00FAFFFF00FBFFFF00FCFFFF00FDFFFF00FEFFFF00FFFF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000213734373737373737373737363737373737373521000000213734373737
      3733313637373737373737373737210000002137343537373737373737373737
      3737373737372100000021373437373737373737373737373737373737352100
      0000213736373736373237373737373732373737373621000000213736353737
      373237343334333737352C3537332100000021373637362E3737373737373737
      373537373633210000002137363736343737322C372E35373737373737372100
      0000213736373737303637373737373730373633373721000000213736373737
      2C3737251D202D373737373736322100000021373632343737371C0E04011528
      37373437352F2100000021373637343735220B04121102193237323737322100
      0000213737373232371F0313141410082E373732373221000000213737343737
      341E050F171B0D092A37373737362100000021373737373737270C07160A061A
      3737373733372100000021373732373737371F0F000918263736363736322100
      00002137373737372937373224232A3736343737373021000000213737372B37
      37332D2E373737372B373737373421000000213737373737303037372E2E2E33
      3637373737342100000021373736373737363530373737373732333734322100
      0000213737363636353535343436363636363636363421000000}
    Visible = False
  end
  object ButtonNew: TButton
    Left = 8
    Top = 8
    Width = 75
    Height = 33
    Caption = 'New'
    TabOrder = 0
    OnClick = ButtonNewClick
  end
  object ButtonOnline: TButton
    Left = 88
    Top = 8
    Width = 121
    Height = 33
    Caption = 'Online'
    TabOrder = 1
    OnClick = ButtonOnlineClick
  end
  object PageControlPrinter: TPageControl
    Left = 2
    Top = 56
    Width = 736
    Height = 263
    ActivePage = TabSheetText
    Anchors = [akLeft, akTop, akRight, akBottom]
    OwnerDraw = True
    TabHeight = 20
    TabOrder = 2
    OnDrawTab = PageControlPrinterDrawTab
    object TabSheetText: TTabSheet
      Caption = 'Text'
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      DesignSize = (
        728
        233)
      object LabelTextCaution: TLabel
        Left = 8
        Top = 8
        Width = 299
        Height = 13
        Caption = 'CAUTION: Do not click into below view window while printing !!'
      end
      object PanelText: TPanel
        Left = 8
        Top = 32
        Width = 713
        Height = 177
        Anchors = [akLeft, akTop, akRight, akBottom]
        BevelOuter = bvNone
        Color = clWhite
        ParentBackground = False
        TabOrder = 0
        DesignSize = (
          713
          177)
        object ImageBorderLeft: TImage
          Left = 0
          Top = 0
          Width = 21
          Height = 177
          Anchors = [akLeft, akTop, akBottom]
        end
        object ImageBorderRight: TImage
          Left = 692
          Top = 0
          Width = 21
          Height = 177
          Anchors = [akTop, akRight, akBottom]
          ExplicitHeight = 185
        end
        object RichEditPrinter: TRichEdit
          Left = 32
          Top = 0
          Width = 649
          Height = 177
          Anchors = [akLeft, akTop, akRight, akBottom]
          BorderStyle = bsNone
          Ctl3D = True
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Courier New'
          Font.Style = []
          ParentColor = True
          ParentCtl3D = False
          ParentFont = False
          TabOrder = 0
          Zoom = 100
          OnSelectionChange = RichEditPrinterSelectionChange
        end
      end
      object RichEditLegend: TRichEdit
        Left = 0
        Top = 217
        Width = 729
        Height = 16
        Anchors = [akLeft, akRight, akBottom]
        BorderStyle = bsNone
        Color = clWhite
        Enabled = False
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        Lines.Strings = (
          'RichEditLegend')
        ParentFont = False
        ReadOnly = True
        TabOrder = 1
        Zoom = 100
      end
    end
    object TabSheetCommands: TTabSheet
      Caption = 'Commands'
      ImageIndex = 1
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      DesignSize = (
        728
        233)
      object ButtonCommandsClear: TButton
        Left = 628
        Top = 8
        Width = 92
        Height = 25
        Anchors = [akTop, akRight]
        Caption = 'Clear'
        TabOrder = 0
        OnClick = ButtonCommandsClearClick
      end
      object ButtonCommandsStartStop: TButton
        Left = 10
        Top = 8
        Width = 606
        Height = 25
        Anchors = [akLeft, akTop, akRight]
        Caption = 'Start'
        TabOrder = 1
        OnClick = ButtonCommandsStartStopClick
      end
      object ListBoxHistory: TListBox
        Left = 10
        Top = 40
        Width = 711
        Height = 156
        Anchors = [akLeft, akTop, akRight, akBottom]
        BorderStyle = bsNone
        IntegralHeight = True
        ItemHeight = 13
        ParentColor = True
        TabOrder = 2
        TabWidth = 30
      end
    end
  end
  object ComboBoxCountry: TComboBox
    Left = 611
    Top = 8
    Width = 121
    Height = 24
    Style = csDropDownList
    Anchors = [akTop, akRight]
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    OnChange = ComboBoxCountryChange
    Items.Strings = (
      'U.S.A.'
      'U.K.'
      'GERMANY'
      'SWEDEN'
      'FRANCE'
      'DENMARK'
      'ITALY'
      'SPAIN')
  end
  object ButtonPrint: TButton
    Left = 232
    Top = 8
    Width = 65
    Height = 33
    Caption = 'Print'
    TabOrder = 4
    OnClick = ButtonPrintClick
  end
  object ComboBoxCRLF: TComboBox
    Left = 536
    Top = 8
    Width = 72
    Height = 24
    Style = csDropDownList
    Anchors = [akTop, akRight]
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 5
    OnChange = ComboBoxCRLFChange
    Items.Strings = (
      'CR'
      'CR+LF')
  end
  object ButtonDebug: TButton
    Left = 392
    Top = 8
    Width = 65
    Height = 33
    Caption = 'Debug'
    TabOrder = 6
    OnClick = ButtonDebugClick
  end
  object PrintDialog1: TPrintDialog
    Left = 312
    Top = 8
  end
  object PopupMenuHistory: TPopupMenu
    Left = 145
    Top = 136
    object Copytoclipboard1: TMenuItem
      Caption = 'Copy to clipboard'
      OnClick = Copytoclipboard1Click
    end
  end
end
