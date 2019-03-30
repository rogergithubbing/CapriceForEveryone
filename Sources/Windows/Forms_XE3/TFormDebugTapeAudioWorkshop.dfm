object FormDebugTapeAudioWorkshop: TFormDebugTapeAudioWorkshop
  Left = 544
  Top = 133
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Tape audio workshop'
  ClientHeight = 483
  ClientWidth = 631
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnShow = FormShow
  DesignSize = (
    631
    483)
  PixelsPerInch = 96
  TextHeight = 13
  object ShapeLine8bits: TShape
    Left = 80
    Top = 24
    Width = 369
    Height = 2
    Anchors = [akLeft, akTop, akRight]
    Brush.Color = clBlack
  end
  object Label1: TLabel
    Left = 8
    Top = 16
    Width = 65
    Height = 17
    AutoSize = False
    Caption = '8 bits input'
    Layout = tlCenter
  end
  object ShapeLine8bitsEnd: TShape
    Left = 536
    Top = 24
    Width = 25
    Height = 2
    Anchors = [akTop, akRight]
    Brush.Color = clBlack
  end
  object Label2: TLabel
    Left = 568
    Top = 16
    Width = 57
    Height = 17
    Anchors = [akTop, akRight]
    AutoSize = False
    Caption = 'PPI output'
    Layout = tlCenter
  end
  object ShapeLine16bitsEnd: TShape
    Left = 536
    Top = 88
    Width = 25
    Height = 2
    Anchors = [akTop, akRight]
    Brush.Color = clBlack
  end
  object Label5: TLabel
    Left = 566
    Top = 80
    Width = 57
    Height = 17
    Anchors = [akTop, akRight]
    AutoSize = False
    Caption = 'PPI output'
    Layout = tlCenter
  end
  object ShapeLine16bits: TShape
    Left = 80
    Top = 88
    Width = 369
    Height = 2
    Anchors = [akLeft, akTop, akRight]
    Brush.Color = clBlack
  end
  object Label6: TLabel
    Left = 8
    Top = 80
    Width = 65
    Height = 17
    AutoSize = False
    Caption = '16 bits input'
    Layout = tlCenter
  end
  object Label7: TLabel
    Left = 96
    Top = 112
    Width = 47
    Height = 13
    Caption = 'Auto Gain'
  end
  object Label8: TLabel
    Left = 160
    Top = 112
    Width = 89
    Height = 13
    Alignment = taCenter
    AutoSize = False
    Caption = 'Low Pass Filter 1'
  end
  object Label9: TLabel
    Left = 248
    Top = 112
    Width = 89
    Height = 13
    Alignment = taCenter
    AutoSize = False
    Caption = 'Low Pass Filter 2'
  end
  object Label18: TLabel
    Left = 344
    Top = 112
    Width = 89
    Height = 13
    Alignment = taCenter
    AutoSize = False
    Caption = 'High Pass Filter 2'
  end
  object LabelApplyNote: TLabel
    Left = 24
    Top = 256
    Width = 442
    Height = 20
    Caption = 
      'To apply parameters update, do not forget to reload audio file !' +
      '!'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object CheckBoxCreateDebugOutputAudio: TCheckBox
    Left = 480
    Top = 168
    Width = 129
    Height = 17
    Hint = 
      'Debug audio file contains filtered audio signal in left channel ' +
      'and PPI  binary signal in right channel if exists.'
    Anchors = [akTop, akRight]
    Caption = 'Debug audio output file'
    TabOrder = 1
    OnClick = CheckBoxCreateDebugOutputAudioClick
  end
  object ButtonReloadAudio: TButton
    Left = 472
    Top = 248
    Width = 145
    Height = 33
    Anchors = [akTop, akRight]
    Caption = 'Reload audio'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 0
    OnClick = ButtonReloadAudioClick
  end
  object PanelPPIThreshold8: TPanel
    Left = 448
    Top = 8
    Width = 89
    Height = 57
    Anchors = [akTop, akRight]
    ParentColor = True
    TabOrder = 2
    object Label3: TLabel
      Left = 8
      Top = 8
      Width = 67
      Height = 13
      Caption = 'PPI Threshold'
    end
    object EditPPIThreshold8: TEdit
      Left = 8
      Top = 24
      Width = 73
      Height = 21
      Hint = 'Value from 0 to 255 (Offset=128)'
      TabOrder = 0
      Text = '0'
      OnExit = EditPPIThreshold8Exit
      OnKeyPress = EditPPIThreshold8KeyPress
    end
  end
  object Panel1: TPanel
    Left = 448
    Top = 72
    Width = 89
    Height = 57
    Anchors = [akTop, akRight]
    ParentColor = True
    TabOrder = 3
    object Label4: TLabel
      Left = 8
      Top = 8
      Width = 67
      Height = 13
      Caption = 'PPI Threshold'
    end
    object EditPPIThreshold16: TEdit
      Left = 8
      Top = 24
      Width = 73
      Height = 21
      Hint = 'Value from -32768 to 32767 (Offset=0)'
      TabOrder = 0
      Text = '0'
      OnExit = EditPPIThreshold16Exit
      OnKeyPress = EditPPIThreshold16KeyPress
    end
  end
  object GroupBoxLPF1: TGroupBox
    Left = 16
    Top = 144
    Width = 137
    Height = 73
    Caption = ' Low Pass filter 1 '
    TabOrder = 4
    object LabelLPF1Param: TLabel
      Left = 16
      Top = 40
      Width = 17
      Height = 17
      AutoSize = False
      Caption = 'A'
      Layout = tlCenter
    end
    object Label1LPF1Info: TLabel
      Left = 8
      Top = 16
      Width = 115
      Height = 13
      Caption = 'Z = ((1-A) * X) + (A * Z-1)'
    end
    object EditLPF1_A: TEdit
      Left = 40
      Top = 40
      Width = 65
      Height = 21
      Hint = 'Value from 0.000 to 1.000'
      TabOrder = 0
      Text = '0.4'
      OnExit = EditLPF1_AExit
      OnKeyPress = EditLPF1_AKeyPress
    end
  end
  object PanelAutoGain: TPanel
    Left = 104
    Top = 72
    Width = 33
    Height = 33
    Hint = 'Activate module (Green=Active, Red=Inactive)'
    Color = clLime
    ParentBackground = False
    TabOrder = 5
    OnClick = PanelAutoGainClick
    OnDblClick = PanelAutoGainClick
  end
  object PanelLPF1: TPanel
    Left = 184
    Top = 72
    Width = 33
    Height = 33
    Hint = 'Activate module (Green=Active, Red=Inactive)'
    Color = clLime
    ParentBackground = False
    TabOrder = 6
    OnClick = PanelLPF1Click
    OnDblClick = PanelLPF1Click
  end
  object GroupBoxLPF2: TGroupBox
    Left = 168
    Top = 144
    Width = 121
    Height = 97
    Caption = ' Low Pass filter 2 '
    TabOrder = 7
    object LabelLPF2Info: TLabel
      Left = 8
      Top = 16
      Width = 76
      Height = 13
      Hint = 'Butterworth Low Pass Filter'
      Caption = 'Butterworth LPF'
      ParentShowHint = False
      ShowHint = True
    end
    object LabelLPF2Order: TLabel
      Left = 8
      Top = 40
      Width = 33
      Height = 17
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Order'
      Layout = tlCenter
    end
    object LabelLPF2Freq: TLabel
      Left = 8
      Top = 64
      Width = 33
      Height = 17
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Freq'
      Layout = tlCenter
    end
    object EditLPF2_Order: TEdit
      Left = 48
      Top = 40
      Width = 57
      Height = 21
      Hint = 'Value from 1 to 10'
      TabOrder = 0
      Text = '2'
      OnExit = EditLPF2_OrderExit
      OnKeyPress = EditLPF2_OrderKeyPress
    end
    object EditLPF2_Freq: TEdit
      Left = 48
      Top = 64
      Width = 57
      Height = 21
      Hint = 'Value from 10 to 20000'
      TabOrder = 1
      Text = '6000'
      OnExit = EditLPF2_FreqExit
      OnKeyPress = EditLPF2_FreqKeyPress
    end
  end
  object GroupBoxHPF2: TGroupBox
    Left = 304
    Top = 144
    Width = 121
    Height = 97
    Caption = ' High Pass filter 2'
    TabOrder = 8
    object LabelHPF2Info: TLabel
      Left = 8
      Top = 16
      Width = 78
      Height = 13
      Hint = 'Butterworth High Pass Filter'
      Caption = 'Butterworth HPF'
      ParentShowHint = False
      ShowHint = True
    end
    object LabelHPF2Order: TLabel
      Left = 8
      Top = 40
      Width = 33
      Height = 17
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Order'
      Layout = tlCenter
    end
    object LabelHPF2Freq: TLabel
      Left = 8
      Top = 64
      Width = 33
      Height = 17
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Freq'
      Layout = tlCenter
    end
    object EditHPF2_Order: TEdit
      Left = 48
      Top = 40
      Width = 57
      Height = 21
      Hint = 'Value from 1 to 10'
      TabOrder = 0
      Text = '2'
      OnExit = EditHPF2_OrderExit
      OnKeyPress = EditHPF2_OrderKeyPress
    end
    object EditHPF2_Freq: TEdit
      Left = 48
      Top = 64
      Width = 57
      Height = 21
      Hint = 'Value from 10 to 20000'
      TabOrder = 1
      Text = '500'
      OnExit = EditHPF2_FreqExit
      OnKeyPress = EditHPF2_FreqKeyPress
    end
  end
  object PanelLPF2: TPanel
    Left = 272
    Top = 72
    Width = 33
    Height = 33
    Hint = 'Activate module (Green=Active, Red=Inactive)'
    Color = clLime
    ParentBackground = False
    TabOrder = 9
    OnClick = PanelLPF2Click
    OnDblClick = PanelLPF2Click
  end
  object PanelHPF2: TPanel
    Left = 368
    Top = 72
    Width = 33
    Height = 33
    Hint = 'Activate module (Green=Active, Red=Inactive)'
    Color = clLime
    ParentBackground = False
    TabOrder = 10
    OnClick = PanelHPF2Click
  end
  object ButtonRestoreDefault: TButton
    Left = 472
    Top = 192
    Width = 145
    Height = 25
    Anchors = [akTop, akRight]
    Caption = 'Restore default'
    TabOrder = 11
    OnClick = ButtonRestoreDefaultClick
  end
  object GroupBoxFilterTest: TGroupBox
    Left = 16
    Top = 304
    Width = 601
    Height = 177
    Caption = ' Filter gain '
    TabOrder = 12
    DesignSize = (
      601
      177)
    object ChartFilterGain: TChart
      Left = 8
      Top = 16
      Width = 585
      Height = 153
      AllowPanning = pmNone
      BackWall.Brush.Style = bsClear
      BackWall.Pen.Color = clGray
      Legend.Visible = False
      MarginBottom = 3
      MarginRight = 0
      MarginTop = 0
      Title.Text.Strings = (
        'TChart')
      Title.Visible = False
      BottomAxis.ExactDateTime = False
      BottomAxis.Increment = 500.000000000000000000
      BottomAxis.LabelsOnAxis = False
      BottomAxis.Logarithmic = True
      BottomAxis.RoundFirstLabel = False
      BottomAxis.TickLength = 0
      BottomAxis.Title.Caption = 'Frequency (Hz)'
      Frame.Color = clGray
      LeftAxis.Automatic = False
      LeftAxis.AutomaticMaximum = False
      LeftAxis.AutomaticMinimum = False
      LeftAxis.Increment = 10.000000000000000000
      LeftAxis.Maximum = 10.000000000000000000
      LeftAxis.Minimum = -100.000000000000000000
      LeftAxis.Title.Caption = 'Gain (dB)'
      View3D = False
      View3DWalls = False
      Zoom.Allow = False
      BevelOuter = bvNone
      ParentColor = True
      TabOrder = 0
      Anchors = [akLeft, akTop, akRight, akBottom]
      DefaultCanvas = 'TGDIPlusCanvas'
      PrintMargins = (
        15
        34
        15
        34)
      ColorPaletteIndex = 13
      object Series1: TLineSeries
        SeriesColor = clRed
        Title = 'Gain'
        Brush.BackColor = clDefault
        Pointer.InflateMargins = True
        Pointer.Style = psRectangle
        XValues.Name = 'X '
        XValues.Order = loAscending
        YValues.Name = 'Y '
        YValues.Order = loNone
      end
    end
  end
  object CheckBoxCreateCSW: TCheckBox
    Left = 480
    Top = 152
    Width = 97
    Height = 17
    Caption = 'Create CSW'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 13
    OnClick = CheckBoxCreateCSWClick
  end
end
