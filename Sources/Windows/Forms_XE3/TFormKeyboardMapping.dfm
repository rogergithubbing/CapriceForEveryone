object FormKeyboardMapping: TFormKeyboardMapping
  Left = 151
  Top = 119
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Keyboard mapping'
  ClientHeight = 488
  ClientWidth = 624
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  Position = poScreenCenter
  OnDestroy = FormDestroy
  OnHide = FormHide
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object LabelTips1: TLabel
    Left = 112
    Top = 376
    Width = 181
    Height = 13
    Caption = '1. Select CPC key on above keyboard'
  end
  object LabelTips2: TLabel
    Left = 112
    Top = 392
    Width = 231
    Height = 13
    Caption = '2. Press the key on Windows keyboard you want'
  end
  object LabelTips3: TLabel
    Left = 112
    Top = 424
    Width = 284
    Height = 13
    Caption = '3. Affect Windows key to CPC key by pressing "Set" button.'
  end
  object LabelHowToUse: TLabel
    Left = 8
    Top = 376
    Width = 57
    Height = 13
    Caption = 'How to use:'
  end
  object LabelTips2bis: TLabel
    Left = 112
    Top = 408
    Width = 138
    Height = 13
    Caption = '   affect to selected CPC key.'
  end
  object PanelButtons: TPanel
    Left = 0
    Top = 447
    Width = 624
    Height = 41
    Align = alBottom
    BevelOuter = bvNone
    ParentBackground = False
    ParentColor = True
    TabOrder = 0
    DesignSize = (
      624
      41)
    object ButtonCancel: TButton
      Left = 533
      Top = 8
      Width = 83
      Height = 25
      Anchors = [akTop, akRight]
      Caption = 'Cancel'
      ModalResult = 2
      TabOrder = 0
    end
    object ButtonApply: TButton
      Left = 429
      Top = 8
      Width = 91
      Height = 25
      Anchors = [akTop, akRight]
      Caption = 'Apply'
      ModalResult = 1
      TabOrder = 1
      OnClick = ButtonApplyClick
    end
    object CheckBoxUseHardwareKeyboardMapping: TCheckBox
      Left = 128
      Top = 8
      Width = 265
      Height = 25
      Hint = 'Checked = Hardware mapping. Unchecked = ASCII mapping'
      Caption = 'Use hardware keyboard mapping'
      TabOrder = 2
    end
    object ButtonDefault: TButton
      Left = 8
      Top = 8
      Width = 89
      Height = 25
      Hint = 'Restore factory default mapping. '
      Caption = 'Default'
      TabOrder = 3
      OnClick = ButtonDefaultClick
    end
  end
  object TabControlGraphicLayout: TTabControl
    Left = 8
    Top = 8
    Width = 609
    Height = 209
    HotTrack = True
    OwnerDraw = True
    TabHeight = 20
    TabOrder = 1
    Tabs.Strings = (
      'UK Layout'
      'FR Layout'
      'SP Layout'
      'CPC Keycode')
    TabIndex = 0
    OnChange = TabControlGraphicLayoutChange
    OnDrawTab = TabControlGraphicLayoutDrawTab
    object PanelGraphicKey66: TPanel
      Tag = 66
      Left = 8
      Top = 32
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = 'ESC'
      Color = clWhite
      ParentBackground = False
      TabOrder = 0
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey64: TPanel
      Tag = 64
      Left = 41
      Top = 32
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = '1'
      Color = clWhite
      ParentBackground = False
      TabOrder = 1
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey65: TPanel
      Tag = 65
      Left = 74
      Top = 32
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = '2'
      Color = clWhite
      ParentBackground = False
      TabOrder = 2
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey68: TPanel
      Tag = 68
      Left = 8
      Top = 65
      Width = 41
      Height = 33
      BevelInner = bvRaised
      Caption = 'TAB'
      Color = clWhite
      ParentBackground = False
      TabOrder = 3
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey70: TPanel
      Tag = 70
      Left = 8
      Top = 98
      Width = 50
      Height = 33
      BevelInner = bvRaised
      Caption = 'CAPS'
      Color = clWhite
      ParentBackground = False
      TabOrder = 4
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey21L: TPanel
      Tag = 21
      Left = 8
      Top = 131
      Width = 66
      Height = 33
      BevelInner = bvRaised
      Caption = 'SHIFT'
      Color = clWhite
      ParentBackground = False
      TabOrder = 5
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey23: TPanel
      Tag = 23
      Left = 8
      Top = 164
      Width = 66
      Height = 33
      BevelInner = bvRaised
      Caption = 'CONTROL'
      Color = clWhite
      ParentBackground = False
      TabOrder = 6
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey57: TPanel
      Tag = 57
      Left = 107
      Top = 32
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = '3'
      Color = clWhite
      ParentBackground = False
      TabOrder = 7
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey56: TPanel
      Tag = 56
      Left = 140
      Top = 32
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = '4'
      Color = clWhite
      ParentBackground = False
      TabOrder = 8
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey49: TPanel
      Tag = 49
      Left = 173
      Top = 32
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = '5'
      Color = clWhite
      ParentBackground = False
      TabOrder = 9
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey48: TPanel
      Tag = 48
      Left = 206
      Top = 32
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = '6'
      Color = clWhite
      ParentBackground = False
      TabOrder = 10
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey41: TPanel
      Tag = 41
      Left = 239
      Top = 32
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = '7'
      Color = clWhite
      ParentBackground = False
      TabOrder = 11
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey40: TPanel
      Tag = 40
      Left = 272
      Top = 32
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = '8'
      Color = clWhite
      ParentBackground = False
      TabOrder = 12
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey33: TPanel
      Tag = 33
      Left = 305
      Top = 32
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = '9'
      Color = clWhite
      ParentBackground = False
      TabOrder = 13
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey32: TPanel
      Tag = 32
      Left = 338
      Top = 32
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = '0'
      Color = clWhite
      ParentBackground = False
      TabOrder = 14
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey25: TPanel
      Tag = 25
      Left = 371
      Top = 32
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = '-'
      Color = clWhite
      ParentBackground = False
      TabOrder = 15
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey24: TPanel
      Tag = 24
      Left = 404
      Top = 32
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = '^'
      Color = clWhite
      ParentBackground = False
      TabOrder = 16
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey16: TPanel
      Tag = 16
      Left = 437
      Top = 32
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = 'CLR'
      Color = clWhite
      ParentBackground = False
      TabOrder = 17
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey79: TPanel
      Tag = 79
      Left = 470
      Top = 32
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = 'DEL'
      Color = clWhite
      ParentBackground = False
      TabOrder = 18
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey10: TPanel
      Tag = 10
      Left = 503
      Top = 32
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = 'f7'
      Color = clWhite
      ParentBackground = False
      TabOrder = 19
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey11: TPanel
      Tag = 11
      Left = 536
      Top = 32
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = 'f8'
      Color = clWhite
      ParentBackground = False
      TabOrder = 20
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey3: TPanel
      Tag = 3
      Left = 569
      Top = 32
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = 'f9'
      Color = clWhite
      ParentBackground = False
      TabOrder = 21
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey67: TPanel
      Tag = 67
      Left = 49
      Top = 65
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = 'A'
      Color = clWhite
      ParentBackground = False
      TabOrder = 22
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey59: TPanel
      Tag = 59
      Left = 82
      Top = 65
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = 'Z'
      Color = clWhite
      ParentBackground = False
      TabOrder = 23
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey58: TPanel
      Tag = 58
      Left = 115
      Top = 65
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = 'E'
      Color = clWhite
      ParentBackground = False
      TabOrder = 24
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey50: TPanel
      Tag = 50
      Left = 148
      Top = 65
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = 'R'
      Color = clWhite
      ParentBackground = False
      TabOrder = 25
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey51: TPanel
      Tag = 51
      Left = 181
      Top = 65
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = 'T'
      Color = clWhite
      ParentBackground = False
      TabOrder = 26
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey43: TPanel
      Tag = 43
      Left = 214
      Top = 65
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = 'Y'
      Color = clWhite
      ParentBackground = False
      TabOrder = 43
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey42: TPanel
      Tag = 42
      Left = 247
      Top = 65
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = 'U'
      Color = clWhite
      ParentBackground = False
      TabOrder = 27
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey35: TPanel
      Tag = 35
      Left = 280
      Top = 65
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = 'I'
      Color = clWhite
      ParentBackground = False
      TabOrder = 28
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey34: TPanel
      Tag = 34
      Left = 313
      Top = 65
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = 'O'
      Color = clWhite
      ParentBackground = False
      TabOrder = 29
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey27: TPanel
      Tag = 27
      Left = 346
      Top = 65
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = 'P'
      Color = clWhite
      ParentBackground = False
      TabOrder = 30
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey26: TPanel
      Tag = 26
      Left = 379
      Top = 65
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Color = clWhite
      ParentBackground = False
      TabOrder = 31
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey17: TPanel
      Tag = 17
      Left = 412
      Top = 65
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Color = clWhite
      ParentBackground = False
      TabOrder = 32
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey20: TPanel
      Tag = 20
      Left = 503
      Top = 65
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = 'f4'
      Color = clWhite
      ParentBackground = False
      TabOrder = 33
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey12: TPanel
      Tag = 12
      Left = 536
      Top = 65
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = 'f5'
      Color = clWhite
      ParentBackground = False
      TabOrder = 34
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey4: TPanel
      Tag = 4
      Left = 569
      Top = 65
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = 'f6'
      Color = clWhite
      ParentBackground = False
      TabOrder = 35
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey13: TPanel
      Tag = 13
      Left = 503
      Top = 98
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = 'f1'
      Color = clWhite
      ParentBackground = False
      TabOrder = 36
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey14: TPanel
      Tag = 14
      Left = 536
      Top = 98
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = 'f2'
      Color = clWhite
      ParentBackground = False
      TabOrder = 37
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey5: TPanel
      Tag = 5
      Left = 569
      Top = 98
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = 'f3'
      Color = clWhite
      ParentBackground = False
      TabOrder = 38
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey15: TPanel
      Tag = 15
      Left = 503
      Top = 131
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = 'f0'
      Color = clWhite
      ParentBackground = False
      TabOrder = 39
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey0: TPanel
      Left = 536
      Top = 131
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Color = clWhite
      ParentBackground = False
      TabOrder = 40
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
      object ImageGraphicKey0: TImage
        Left = 0
        Top = 0
        Width = 32
        Height = 32
        Center = True
        Picture.Data = {
          07544269746D6170D6020000424DD60200000000000036000000280000000A00
          0000150000000100180000000000A0020000130B0000130B0000000000000000
          0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFE8E8E8C6C6C6C6C6C6EAEAEAFFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFB9B9B91A15151E1919C0C0C0FFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFBCBCBC3A37373C3939C2C2C2FFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFBEBEBE4D4B4B4E4B4BC4C4C4FFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFBFBFBF525050525050C5C5C5FFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFC1C1C1575554575554C7C7C7FFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFC2C2C2575554575554C8C8C8FFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFC5C5C55B59585A5859CBCACAFFFFFFFFFFFFFFFFFF
          0000F9F9F9F0F0F0F2F2F2BABABA5A5858595757C0BFBFF2F2F2F0F0F0F9F9F9
          0000EBEAEB8887875655545957565755545755545A58575654538D8D8DEFEFEF
          0000FFFFFFBFBEBF3B3737504D4D5B59585A58594F4D4D3C3838C5C5C5FFFFFF
          0000FFFFFFEFEFEF6C6A6A52504F5A5858595757504E4E737171F2F2F2FFFFFF
          0000FFFFFFFFFFFFB3B2B24845465B59585A5859474444B9B9B9FFFFFFFFFFFF
          0000FFFFFFFFFFFFE8E8E86867665A58585957576E6C6CEBEBEBFFFFFFFFFFFF
          0000FFFFFFFFFFFFFEFEFEA7A6A5555351545251ADACACFEFEFEFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFE1E1E1646161686565E6E5E5FFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFFDFDFD9493929B9999FEFEFEFFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFFFFFFFD8D7D7DADADAFFFFFFFFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFFFFFFFFDFDFDFEFEFEFFFFFFFFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          0000}
        Transparent = True
        OnClick = ImageGraphicKeyClick
        OnDblClick = ImageGraphicKeyClick
      end
    end
    object PanelGraphicKey7: TPanel
      Tag = 7
      Left = 569
      Top = 131
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = '.'
      Color = clWhite
      ParentBackground = False
      TabOrder = 41
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey8: TPanel
      Tag = 8
      Left = 503
      Top = 164
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Color = clWhite
      ParentBackground = False
      TabOrder = 42
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
      object ImageGraphicKey8: TImage
        Left = 0
        Top = 0
        Width = 32
        Height = 32
        Center = True
        Picture.Data = {
          07544269746D6170B6020000424DB60200000000000036000000280000001500
          00000A000000010018000000000080020000130B0000130B0000000000000000
          0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          EBEAEBF9F9F9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FF00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEFBFBEBF
          888787F0F0F0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FF00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEE8E8E8B3B2B26C6A6A3B3737
          565554F2F2F2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FF00FFFFFFFFFFFFFFFFFFFDFDFDE1E1E1A7A6A568676648454652504F504D4D
          595756BABABAC5C5C5C2C2C2C1C1C1BFBFBFBEBEBEBCBCBCB9B9B9E8E8E8FFFF
          FF00FFFFFFFDFDFDD8D7D79493926461615553515A58585B59585A58585B5958
          5755545A58585B59585755545755545250504D4B4B3A37371A1515C6C6C6FFFF
          FF00FFFFFFFEFEFEDADADA9B99996865655452515957575A58595957575A5859
          5755545957575A58595755545755545250504E4B4B3C39391E1919C6C6C6FFFF
          FF00FFFFFFFFFFFFFFFFFFFEFEFEE6E5E5ADACAC6E6C6C474444504E4E4F4D4D
          5A5857C0BFBFCBCACAC8C8C8C7C7C7C5C5C5C4C4C4C2C2C2C0C0C0EAEAEAFFFF
          FF00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEEBEBEBB9B9B97371713C3838
          565453F2F2F2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FF00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF2F2F2C5C5C5
          8D8D8DF0F0F0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FF00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          EFEFEFF9F9F9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FF00}
        Transparent = True
        OnClick = ImageGraphicKeyClick
        OnDblClick = ImageGraphicKeyClick
      end
    end
    object PanelGraphicKey2: TPanel
      Tag = 2
      Left = 536
      Top = 164
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Color = clWhite
      ParentBackground = False
      TabOrder = 44
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
      object ImageGraphicKey2: TImage
        Left = 0
        Top = 0
        Width = 32
        Height = 32
        Center = True
        Picture.Data = {
          07544269746D6170D6020000424DD60200000000000036000000280000000A00
          0000150000000100180000000000A0020000130B0000130B0000000000000000
          0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFFFFFFFFEFEFEFDFDFDFFFFFFFFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFFFFFFFDADADAD8D7D7FFFFFFFFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFFEFEFE9B9999949392FDFDFDFFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFE6E5E5686565646161E1E1E1FFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFEFEFEADACAC545251555351A7A6A5FEFEFEFFFFFFFFFFFF
          0000FFFFFFFFFFFFEBEBEB6E6C6C5957575A5858686766E8E8E8FFFFFFFFFFFF
          0000FFFFFFFFFFFFB9B9B94744445A58595B5958484546B3B2B2FFFFFFFFFFFF
          0000FFFFFFF2F2F2737171504E4E5957575A585852504F6C6A6AEFEFEFFFFFFF
          0000FFFFFFC5C5C53C38384F4D4D5A58595B5958504D4D3B3737BFBEBFFFFFFF
          0000EFEFEF8D8D8D5654535A5857575554575554595756565554888787EBEAEB
          0000F9F9F9F0F0F0F2F2F2C0BFBF5957575A5858BABABAF2F2F2F0F0F0F9F9F9
          0000FFFFFFFFFFFFFFFFFFCBCACA5A58595B5958C5C5C5FFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFC8C8C8575554575554C2C2C2FFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFC7C7C7575554575554C1C1C1FFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFC5C5C5525050525050BFBFBFFFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFC4C4C44E4B4B4D4B4BBEBEBEFFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFC2C2C23C39393A3737BCBCBCFFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFC0C0C01E19191A1515B9B9B9FFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFEAEAEAC6C6C6C6C6C6E8E8E8FFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          0000}
        Transparent = True
        OnClick = ImageGraphicKeyClick
        OnDblClick = ImageGraphicKeyClick
      end
    end
    object PanelGraphicKey1: TPanel
      Tag = 1
      Left = 569
      Top = 164
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Color = clWhite
      ParentBackground = False
      TabOrder = 45
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
      object ImageGraphicKey1: TImage
        Left = 0
        Top = 0
        Width = 32
        Height = 32
        Center = True
        Picture.Data = {
          07544269746D6170B6020000424DB60200000000000036000000280000001500
          00000A000000010018000000000080020000130B0000130B0000000000000000
          0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF9F9F9
          EFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FF00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0F0F0
          8D8D8DC5C5C5F2F2F2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FF00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF2F2F2
          5654533C3838737171B9B9B9EBEBEBFEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FF00FFFFFFEAEAEAC0C0C0C2C2C2C4C4C4C5C5C5C7C7C7C8C8C8CBCACAC0BFBF
          5A58574F4D4D504E4E4744446E6C6CADACACE6E5E5FEFEFEFFFFFFFFFFFFFFFF
          FF00FFFFFFC6C6C61E19193C39394E4B4B5250505755545755545A5859595757
          5755545A58595957575A58595957575452516865659B9999DADADAFEFEFEFFFF
          FF00FFFFFFC6C6C61A15153A37374D4B4B5250505755545755545B59585A5858
          5755545B59585A58585B59585A5858555351646161949392D8D7D7FDFDFDFFFF
          FF00FFFFFFE8E8E8B9B9B9BCBCBCBEBEBEBFBFBFC1C1C1C2C2C2C5C5C5BABABA
          595756504D4D52504F484546686766A7A6A5E1E1E1FDFDFDFFFFFFFFFFFFFFFF
          FF00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF2F2F2
          5655543B37376C6A6AB3B2B2E8E8E8FEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FF00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0F0F0
          888787BFBEBFEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FF00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF9F9F9
          EBEAEBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FF00}
        Transparent = True
        OnClick = ImageGraphicKeyClick
        OnDblClick = ImageGraphicKeyClick
      end
    end
    object PanelGraphicKey18: TPanel
      Tag = 18
      Left = 454
      Top = 65
      Width = 49
      Height = 66
      BevelInner = bvRaised
      Caption = 'RET'
      Color = clWhite
      ParentBackground = False
      TabOrder = 46
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey9: TPanel
      Tag = 9
      Left = 74
      Top = 164
      Width = 56
      Height = 33
      BevelInner = bvRaised
      Caption = 'COPY'
      Color = clWhite
      ParentBackground = False
      TabOrder = 47
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey6: TPanel
      Tag = 6
      Left = 408
      Top = 164
      Width = 95
      Height = 33
      BevelInner = bvRaised
      Caption = 'ENTER'
      Color = clWhite
      ParentBackground = False
      TabOrder = 48
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey47: TPanel
      Tag = 47
      Left = 130
      Top = 164
      Width = 278
      Height = 33
      BevelInner = bvRaised
      Color = clWhite
      ParentBackground = False
      TabOrder = 49
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey69: TPanel
      Tag = 69
      Left = 58
      Top = 98
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = 'Q'
      Color = clWhite
      ParentBackground = False
      TabOrder = 50
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey60: TPanel
      Tag = 60
      Left = 91
      Top = 98
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = 'S'
      Color = clWhite
      ParentBackground = False
      TabOrder = 51
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey61: TPanel
      Tag = 61
      Left = 124
      Top = 98
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = 'D'
      Color = clWhite
      ParentBackground = False
      TabOrder = 52
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey53: TPanel
      Tag = 53
      Left = 157
      Top = 98
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = 'F'
      Color = clWhite
      ParentBackground = False
      TabOrder = 53
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey52: TPanel
      Tag = 52
      Left = 190
      Top = 98
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = 'G'
      Color = clWhite
      ParentBackground = False
      TabOrder = 54
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey44: TPanel
      Tag = 44
      Left = 223
      Top = 98
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = 'H'
      Color = clWhite
      ParentBackground = False
      TabOrder = 55
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey45: TPanel
      Tag = 45
      Left = 256
      Top = 98
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = 'J'
      Color = clWhite
      ParentBackground = False
      TabOrder = 56
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey37: TPanel
      Tag = 37
      Left = 289
      Top = 98
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = 'K'
      Color = clWhite
      ParentBackground = False
      TabOrder = 57
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey36: TPanel
      Tag = 36
      Left = 322
      Top = 98
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = 'L'
      Color = clWhite
      ParentBackground = False
      TabOrder = 58
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey29: TPanel
      Tag = 29
      Left = 355
      Top = 98
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = 'M'
      Color = clWhite
      ParentBackground = False
      TabOrder = 59
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey28: TPanel
      Tag = 28
      Left = 388
      Top = 98
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Color = clWhite
      ParentBackground = False
      TabOrder = 60
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey19: TPanel
      Tag = 19
      Left = 421
      Top = 98
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Color = clWhite
      ParentBackground = False
      TabOrder = 61
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey71: TPanel
      Tag = 71
      Left = 74
      Top = 131
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = 'W'
      Color = clWhite
      ParentBackground = False
      TabOrder = 62
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey63: TPanel
      Tag = 63
      Left = 107
      Top = 131
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = 'X'
      Color = clWhite
      ParentBackground = False
      TabOrder = 63
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey62: TPanel
      Tag = 62
      Left = 140
      Top = 131
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = 'C'
      Color = clWhite
      ParentBackground = False
      TabOrder = 64
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey55: TPanel
      Tag = 55
      Left = 173
      Top = 131
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = 'V'
      Color = clWhite
      ParentBackground = False
      TabOrder = 65
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey54: TPanel
      Tag = 54
      Left = 206
      Top = 131
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = 'B'
      Color = clWhite
      ParentBackground = False
      TabOrder = 66
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey46: TPanel
      Tag = 46
      Left = 239
      Top = 131
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Caption = 'N'
      Color = clWhite
      ParentBackground = False
      TabOrder = 67
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey38: TPanel
      Tag = 38
      Left = 272
      Top = 131
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Color = clWhite
      ParentBackground = False
      TabOrder = 68
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey39: TPanel
      Tag = 39
      Left = 305
      Top = 131
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Color = clWhite
      ParentBackground = False
      TabOrder = 69
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey31: TPanel
      Tag = 31
      Left = 338
      Top = 131
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Color = clWhite
      ParentBackground = False
      TabOrder = 70
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey30: TPanel
      Tag = 30
      Left = 371
      Top = 131
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Color = clWhite
      ParentBackground = False
      TabOrder = 71
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey22: TPanel
      Tag = 22
      Left = 404
      Top = 131
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Color = clWhite
      ParentBackground = False
      TabOrder = 72
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey21R: TPanel
      Tag = 21
      Left = 437
      Top = 131
      Width = 66
      Height = 33
      BevelInner = bvRaised
      Caption = 'SHIFT'
      Color = clWhite
      ParentBackground = False
      TabOrder = 73
      OnClick = PanelGraphicKeyClick
      OnDblClick = PanelGraphicKeyClick
    end
    object PanelGraphicKey18End: TPanel
      Tag = -1
      Left = 445
      Top = 65
      Width = 9
      Height = 33
      BevelOuter = bvNone
      Color = clWhite
      ParentBackground = False
      TabOrder = 74
    end
  end
  object GroupBoxJoystick0: TGroupBox
    Left = 8
    Top = 224
    Width = 153
    Height = 137
    Caption = ' Joystick 0 '
    ParentBackground = False
    TabOrder = 2
    object PanelGraphicKey72: TPanel
      Tag = 72
      Left = 57
      Top = 22
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Color = clWhite
      ParentBackground = False
      ParentShowHint = False
      ShowHint = False
      TabOrder = 0
      OnClick = PanelGraphicJoystick0KeyClick
      OnDblClick = PanelGraphicJoystick0KeyClick
      object ImageGraphicKey72: TImage
        Left = 0
        Top = 0
        Width = 32
        Height = 32
        Center = True
        Picture.Data = {
          07544269746D6170D6020000424DD60200000000000036000000280000000A00
          0000150000000100180000000000A0020000130B0000130B0000000000000000
          0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFE8E8E8C6C6C6C6C6C6EAEAEAFFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFB9B9B91A15151E1919C0C0C0FFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFBCBCBC3A37373C3939C2C2C2FFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFBEBEBE4D4B4B4E4B4BC4C4C4FFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFBFBFBF525050525050C5C5C5FFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFC1C1C1575554575554C7C7C7FFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFC2C2C2575554575554C8C8C8FFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFC5C5C55B59585A5859CBCACAFFFFFFFFFFFFFFFFFF
          0000F9F9F9F0F0F0F2F2F2BABABA5A5858595757C0BFBFF2F2F2F0F0F0F9F9F9
          0000EBEAEB8887875655545957565755545755545A58575654538D8D8DEFEFEF
          0000FFFFFFBFBEBF3B3737504D4D5B59585A58594F4D4D3C3838C5C5C5FFFFFF
          0000FFFFFFEFEFEF6C6A6A52504F5A5858595757504E4E737171F2F2F2FFFFFF
          0000FFFFFFFFFFFFB3B2B24845465B59585A5859474444B9B9B9FFFFFFFFFFFF
          0000FFFFFFFFFFFFE8E8E86867665A58585957576E6C6CEBEBEBFFFFFFFFFFFF
          0000FFFFFFFFFFFFFEFEFEA7A6A5555351545251ADACACFEFEFEFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFE1E1E1646161686565E6E5E5FFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFFDFDFD9493929B9999FEFEFEFFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFFFFFFFD8D7D7DADADAFFFFFFFFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFFFFFFFFDFDFDFEFEFEFFFFFFFFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          0000}
        Transparent = True
        OnClick = ImageGraphicKeyJoystick0Click
        OnDblClick = ImageGraphicKeyJoystick0Click
      end
    end
    object PanelGraphicKey74: TPanel
      Tag = 74
      Left = 7
      Top = 56
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Color = clWhite
      ParentBackground = False
      ParentShowHint = False
      ShowHint = False
      TabOrder = 1
      OnClick = PanelGraphicJoystick0KeyClick
      OnDblClick = PanelGraphicJoystick0KeyClick
      object ImageGraphicKey74: TImage
        Left = 0
        Top = 0
        Width = 32
        Height = 32
        Center = True
        Picture.Data = {
          07544269746D6170B6020000424DB60200000000000036000000280000001500
          00000A000000010018000000000080020000130B0000130B0000000000000000
          0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          EBEAEBF9F9F9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FF00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEFBFBEBF
          888787F0F0F0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FF00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEE8E8E8B3B2B26C6A6A3B3737
          565554F2F2F2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FF00FFFFFFFFFFFFFFFFFFFDFDFDE1E1E1A7A6A568676648454652504F504D4D
          595756BABABAC5C5C5C2C2C2C1C1C1BFBFBFBEBEBEBCBCBCB9B9B9E8E8E8FFFF
          FF00FFFFFFFDFDFDD8D7D79493926461615553515A58585B59585A58585B5958
          5755545A58585B59585755545755545250504D4B4B3A37371A1515C6C6C6FFFF
          FF00FFFFFFFEFEFEDADADA9B99996865655452515957575A58595957575A5859
          5755545957575A58595755545755545250504E4B4B3C39391E1919C6C6C6FFFF
          FF00FFFFFFFFFFFFFFFFFFFEFEFEE6E5E5ADACAC6E6C6C474444504E4E4F4D4D
          5A5857C0BFBFCBCACAC8C8C8C7C7C7C5C5C5C4C4C4C2C2C2C0C0C0EAEAEAFFFF
          FF00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEEBEBEBB9B9B97371713C3838
          565453F2F2F2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FF00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF2F2F2C5C5C5
          8D8D8DF0F0F0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FF00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          EFEFEFF9F9F9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FF00}
        Transparent = True
        OnClick = ImageGraphicKeyJoystick0Click
        OnDblClick = ImageGraphicKeyJoystick0Click
      end
    end
    object PanelGraphicKey76: TPanel
      Tag = 76
      Left = 40
      Top = 56
      Width = 33
      Height = 33
      Hint = 'Fire 2'
      BevelInner = bvRaised
      Caption = 'F2'
      Color = clWhite
      ParentBackground = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 2
      OnClick = PanelGraphicJoystick0KeyClick
      OnDblClick = PanelGraphicJoystick0KeyClick
    end
    object PanelGraphicKey77: TPanel
      Tag = 77
      Left = 73
      Top = 56
      Width = 33
      Height = 33
      Hint = 'Fire 1'
      BevelInner = bvRaised
      Caption = 'F1'
      Color = clWhite
      ParentBackground = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 3
      OnClick = PanelGraphicJoystick0KeyClick
      OnDblClick = PanelGraphicJoystick0KeyClick
    end
    object PanelGraphicKey75: TPanel
      Tag = 75
      Left = 106
      Top = 56
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Color = clWhite
      ParentBackground = False
      ParentShowHint = False
      ShowHint = False
      TabOrder = 4
      OnClick = PanelGraphicJoystick0KeyClick
      OnDblClick = PanelGraphicJoystick0KeyClick
      object ImageGraphicKey75: TImage
        Left = 0
        Top = 0
        Width = 32
        Height = 32
        Center = True
        Picture.Data = {
          07544269746D6170B6020000424DB60200000000000036000000280000001500
          00000A000000010018000000000080020000130B0000130B0000000000000000
          0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF9F9F9
          EFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FF00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0F0F0
          8D8D8DC5C5C5F2F2F2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FF00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF2F2F2
          5654533C3838737171B9B9B9EBEBEBFEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FF00FFFFFFEAEAEAC0C0C0C2C2C2C4C4C4C5C5C5C7C7C7C8C8C8CBCACAC0BFBF
          5A58574F4D4D504E4E4744446E6C6CADACACE6E5E5FEFEFEFFFFFFFFFFFFFFFF
          FF00FFFFFFC6C6C61E19193C39394E4B4B5250505755545755545A5859595757
          5755545A58595957575A58595957575452516865659B9999DADADAFEFEFEFFFF
          FF00FFFFFFC6C6C61A15153A37374D4B4B5250505755545755545B59585A5858
          5755545B59585A58585B59585A5858555351646161949392D8D7D7FDFDFDFFFF
          FF00FFFFFFE8E8E8B9B9B9BCBCBCBEBEBEBFBFBFC1C1C1C2C2C2C5C5C5BABABA
          595756504D4D52504F484546686766A7A6A5E1E1E1FDFDFDFFFFFFFFFFFFFFFF
          FF00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF2F2F2
          5655543B37376C6A6AB3B2B2E8E8E8FEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FF00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0F0F0
          888787BFBEBFEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FF00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF9F9F9
          EBEAEBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FF00}
        Transparent = True
        OnClick = ImageGraphicKeyJoystick0Click
        OnDblClick = ImageGraphicKeyJoystick0Click
      end
    end
    object PanelGraphicKey73: TPanel
      Tag = 73
      Left = 57
      Top = 90
      Width = 33
      Height = 33
      BevelInner = bvRaised
      Color = clWhite
      ParentBackground = False
      ParentShowHint = False
      ShowHint = False
      TabOrder = 5
      OnClick = PanelGraphicJoystick0KeyClick
      OnDblClick = PanelGraphicJoystick0KeyClick
      object ImageGraphicKey73: TImage
        Left = 0
        Top = 0
        Width = 32
        Height = 32
        Center = True
        Picture.Data = {
          07544269746D6170D6020000424DD60200000000000036000000280000000A00
          0000150000000100180000000000A0020000130B0000130B0000000000000000
          0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFFFFFFFFEFEFEFDFDFDFFFFFFFFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFFFFFFFDADADAD8D7D7FFFFFFFFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFFEFEFE9B9999949392FDFDFDFFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFE6E5E5686565646161E1E1E1FFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFEFEFEADACAC545251555351A7A6A5FEFEFEFFFFFFFFFFFF
          0000FFFFFFFFFFFFEBEBEB6E6C6C5957575A5858686766E8E8E8FFFFFFFFFFFF
          0000FFFFFFFFFFFFB9B9B94744445A58595B5958484546B3B2B2FFFFFFFFFFFF
          0000FFFFFFF2F2F2737171504E4E5957575A585852504F6C6A6AEFEFEFFFFFFF
          0000FFFFFFC5C5C53C38384F4D4D5A58595B5958504D4D3B3737BFBEBFFFFFFF
          0000EFEFEF8D8D8D5654535A5857575554575554595756565554888787EBEAEB
          0000F9F9F9F0F0F0F2F2F2C0BFBF5957575A5858BABABAF2F2F2F0F0F0F9F9F9
          0000FFFFFFFFFFFFFFFFFFCBCACA5A58595B5958C5C5C5FFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFC8C8C8575554575554C2C2C2FFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFC7C7C7575554575554C1C1C1FFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFC5C5C5525050525050BFBFBFFFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFC4C4C44E4B4B4D4B4BBEBEBEFFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFC2C2C23C39393A3737BCBCBCFFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFC0C0C01E19191A1515B9B9B9FFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFEAEAEAC6C6C6C6C6C6E8E8E8FFFFFFFFFFFFFFFFFF
          0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          0000}
        Transparent = True
        OnClick = ImageGraphicKeyJoystick0Click
        OnDblClick = ImageGraphicKeyJoystick0Click
      end
    end
  end
  object GroupBoxGraphicHardwareMapping: TGroupBox
    Left = 424
    Top = 224
    Width = 193
    Height = 65
    Hint = 
      'Hardware mapping makes direct relation between Windows keyboard ' +
      'and CPC keyboard. But CPC keyboard depends of installed ROM.'
    Caption = ' Hardware mapping '
    ParentBackground = False
    TabOrder = 3
    DesignSize = (
      193
      65)
    object ButtonGraphicHardwareSet: TButton
      Left = 8
      Top = 24
      Width = 49
      Height = 28
      Hint = 
        'Affect last Windows key pressed to the highlighted CPC key in Ha' +
        'rdware mode.'
      Caption = 'Set'
      TabOrder = 0
      OnClick = ButtonGraphicHardwareSetClick
    end
    object PanelGraphicHardwareKey: TPanel
      Left = 80
      Top = 24
      Width = 105
      Height = 28
      Hint = 
        'Show windows key affected to the highlighted CPC key in hardware' +
        ' mode.'
      Anchors = [akLeft, akTop, akRight]
      Color = clWhite
      ParentBackground = False
      TabOrder = 1
    end
  end
  object GroupBoxGraphicASCIIMapping: TGroupBox
    Left = 424
    Top = 296
    Width = 193
    Height = 65
    Hint = 
      'All ASCII keys are automatically mapped, regardless of installed' +
      ' ROM. Just need to map special CPC keys such as SHIFT, CONTROL..' +
      '.'
    Caption = ' ASCII mapping '
    ParentBackground = False
    TabOrder = 4
    DesignSize = (
      193
      65)
    object PanelGraphicASCIIKey: TPanel
      Left = 80
      Top = 24
      Width = 105
      Height = 28
      Hint = 
        'Show windows key affected to the highlighted CPC key in ASCII mo' +
        'de.'
      Anchors = [akLeft, akTop, akRight]
      Color = clWhite
      ParentBackground = False
      TabOrder = 0
    end
    object ButtonGraphicASCIISet: TButton
      Left = 8
      Top = 24
      Width = 49
      Height = 28
      Hint = 
        'Affect last Windows key pressed to the highlighted CPC key in AS' +
        'CII mode.'
      Caption = 'Set'
      TabOrder = 1
      OnClick = ButtonGraphicASCIISetClick
    end
  end
  object GroupBoxLastWindowsKey: TGroupBox
    Left = 216
    Top = 224
    Width = 194
    Height = 65
    Caption = ' Windows key '
    TabOrder = 5
    object PanelFormatKeycode: TPanel
      Left = 65
      Top = 24
      Width = 17
      Height = 28
      Hint = 'Show Windows Virtual Key as number or string'
      Caption = '$'
      Color = clWhite
      ParentBackground = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 0
      OnClick = PanelFormatKeycodeClick
      OnDblClick = PanelFormatKeycodeClick
    end
    object PanelGraphicWindowsVK: TPanel
      Left = 82
      Top = 24
      Width = 104
      Height = 28
      Hint = 'Last Windows key pressed'
      Color = clWhite
      ParentBackground = False
      TabOrder = 1
    end
    object ButtonGraphicWindowsVKFind: TButton
      Left = 9
      Top = 24
      Width = 49
      Height = 28
      Hint = 'Find existing mapping'
      Caption = 'Find'
      TabOrder = 2
      OnClick = ButtonGraphicWindowsVKFindClick
    end
  end
  object GroupBoxJoystickMapping: TGroupBox
    Left = 216
    Top = 296
    Width = 193
    Height = 65
    Hint = 'Affect Windows key when keyboard is used as Joystick.'
    Caption = ' Joystick mapping '
    ParentBackground = False
    TabOrder = 6
    DesignSize = (
      193
      65)
    object ButtonJoystickSet: TButton
      Left = 8
      Top = 24
      Width = 49
      Height = 28
      Hint = 'Affect last Windows key pressed to Joystick mapping'
      Caption = 'Set'
      TabOrder = 0
      OnClick = ButtonJoystickSetClick
    end
    object PanelJoystickKey: TPanel
      Left = 80
      Top = 24
      Width = 105
      Height = 28
      Hint = 
        'Show Windows key affected to joystick control when keyboard is u' +
        'sed as Joytick.'
      Anchors = [akLeft, akTop, akRight]
      Color = clWhite
      ParentBackground = False
      TabOrder = 1
    end
  end
end
