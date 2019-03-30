object FormDebugPSG: TFormDebugPSG
  Left = 1147
  Top = 165
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'PSG'
  ClientHeight = 425
  ClientWidth = 475
  Color = clBtnFace
  Constraints.MinHeight = 453
  Constraints.MinWidth = 481
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
  DesignSize = (
    475
    425)
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBoxMonitor: TGroupBox
    Left = 144
    Top = 184
    Width = 323
    Height = 233
    Anchors = [akLeft, akTop, akRight]
    Caption = ' Monitor '
    TabOrder = 0
    DesignSize = (
      323
      233)
    object GroupBoxLeftSample: TGroupBox
      Left = 8
      Top = 64
      Width = 307
      Height = 73
      Anchors = [akLeft, akTop, akRight]
      Caption = ' Left '
      TabOrder = 0
      DesignSize = (
        307
        73)
      object LabelLeftSampleMax: TLabel
        Left = 8
        Top = 46
        Width = 57
        Height = 17
        AutoSize = False
        Caption = 'Max'
        Layout = tlCenter
      end
      object LabelLeftSample: TLabel
        Left = 8
        Top = 22
        Width = 57
        Height = 17
        AutoSize = False
        Caption = 'Sample'
        Layout = tlCenter
      end
      object PanelLeftSample: TPanel
        Left = 72
        Top = 20
        Width = 65
        Height = 21
        ParentBackground = False
        ParentColor = True
        TabOrder = 0
      end
      object PanelLeftSampleMax: TPanel
        Left = 72
        Top = 45
        Width = 65
        Height = 21
        Hint = 'Click to reset'
        Color = clWhite
        ParentBackground = False
        TabOrder = 1
        OnClick = PanelLeftSampleMaxClick
        OnDblClick = PanelLeftSampleMaxClick
      end
      object ChartLeft: TChart
        Left = 144
        Top = 16
        Width = 155
        Height = 49
        AllowPanning = pmNone
        BackWall.Brush.Style = bsClear
        BackWall.Pen.Visible = False
        Legend.Visible = False
        MarginBottom = 1
        MarginLeft = 1
        MarginRight = 1
        MarginTop = 1
        Title.Text.Strings = (
          'TChart')
        Title.Visible = False
        Title.AdjustFrame = False
        AxisVisible = False
        ClipPoints = False
        Frame.Visible = False
        LeftAxis.Automatic = False
        LeftAxis.AutomaticMaximum = False
        LeftAxis.AutomaticMinimum = False
        LeftAxis.Labels = False
        LeftAxis.LabelsFormat.Visible = False
        LeftAxis.Maximum = 32767.000000000000000000
        LeftAxis.Minimum = -2048.000000000000000000
        LeftAxis.Visible = False
        View3D = False
        View3DWalls = False
        Zoom.Allow = False
        BevelOuter = bvNone
        ParentColor = True
        TabOrder = 2
        Anchors = [akLeft, akTop, akRight]
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
        object Series1: TFastLineSeries
          Legend.Visible = False
          SeriesColor = 16744448
          ShowInLegend = False
          LinePen.Color = 16744448
          XValues.Name = 'X '
          XValues.Order = loAscending
          YValues.Name = 'Y '
          YValues.Order = loNone
        end
      end
    end
    object GroupBoxRightSample: TGroupBox
      Left = 8
      Top = 152
      Width = 307
      Height = 73
      Anchors = [akLeft, akTop, akRight]
      Caption = ' Right '
      TabOrder = 1
      DesignSize = (
        307
        73)
      object LabelRightSampleMax: TLabel
        Left = 8
        Top = 46
        Width = 57
        Height = 17
        AutoSize = False
        Caption = 'Max'
        Layout = tlCenter
      end
      object LabelRightSample: TLabel
        Left = 8
        Top = 22
        Width = 57
        Height = 17
        AutoSize = False
        Caption = 'Sample'
        Layout = tlCenter
      end
      object PanelRightSample: TPanel
        Left = 72
        Top = 20
        Width = 65
        Height = 21
        ParentBackground = False
        ParentColor = True
        TabOrder = 0
      end
      object PanelRightSampleMax: TPanel
        Left = 72
        Top = 44
        Width = 65
        Height = 21
        Hint = 'Click to reset'
        Color = clWhite
        ParentBackground = False
        TabOrder = 1
        OnClick = PanelRightSampleMaxClick
        OnDblClick = PanelRightSampleMaxClick
      end
      object ChartRight: TChart
        Left = 144
        Top = 16
        Width = 155
        Height = 49
        AllowPanning = pmNone
        BackWall.Brush.Style = bsClear
        BackWall.Pen.Visible = False
        Legend.Visible = False
        MarginBottom = 1
        MarginLeft = 1
        MarginRight = 1
        MarginTop = 1
        Title.Text.Strings = (
          'TChart')
        Title.Visible = False
        Title.AdjustFrame = False
        AxisVisible = False
        ClipPoints = False
        Frame.Visible = False
        LeftAxis.Automatic = False
        LeftAxis.AutomaticMaximum = False
        LeftAxis.AutomaticMinimum = False
        LeftAxis.Labels = False
        LeftAxis.LabelsFormat.Visible = False
        LeftAxis.Maximum = 32767.000000000000000000
        LeftAxis.Minimum = -2048.000000000000000000
        LeftAxis.Visible = False
        View3D = False
        View3DWalls = False
        Zoom.Allow = False
        BevelOuter = bvNone
        ParentColor = True
        TabOrder = 2
        Anchors = [akLeft, akTop, akRight]
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
        object FastLineSeries1: TFastLineSeries
          Legend.Visible = False
          SeriesColor = 16744448
          ShowInLegend = False
          LinePen.Color = 16744448
          XValues.Name = 'X '
          XValues.Order = loAscending
          YValues.Name = 'Y '
          YValues.Order = loNone
        end
      end
    end
    object ButtonMonitorStartStop: TButton
      Left = 8
      Top = 24
      Width = 113
      Height = 25
      Caption = 'Start'
      TabOrder = 2
      OnClick = ButtonMonitorStartStopClick
    end
  end
  object GroupBoxRegisters: TGroupBox
    Left = 8
    Top = 8
    Width = 129
    Height = 409
    Caption = ' Registers '
    TabOrder = 1
    object LabelR0: TLabel
      Left = 24
      Top = 18
      Width = 33
      Height = 21
      Hint = 'Channel A Fine Tune Register'
      AutoSize = False
      Caption = 'R0'
      Layout = tlCenter
    end
    object LabelR1: TLabel
      Left = 24
      Top = 42
      Width = 33
      Height = 21
      Hint = 'Channel A Coarse Tune Register'
      AutoSize = False
      Caption = 'R1'
      Layout = tlCenter
    end
    object LabelR15: TLabel
      Left = 24
      Top = 378
      Width = 33
      Height = 21
      Hint = 'I/O Port Data'
      AutoSize = False
      Caption = 'R17'
      Layout = tlCenter
    end
    object LabelR2: TLabel
      Left = 24
      Top = 66
      Width = 33
      Height = 21
      Hint = 'Channel B Fine Tune Register'
      AutoSize = False
      Caption = 'R2'
      Layout = tlCenter
    end
    object LabelR8: TLabel
      Left = 24
      Top = 210
      Width = 33
      Height = 21
      Hint = 'Channel A Amplitude Control'
      AutoSize = False
      Caption = 'R10'
      Layout = tlCenter
    end
    object LabelR4: TLabel
      Left = 24
      Top = 114
      Width = 33
      Height = 21
      Hint = 'Channel C Fine Tune Register'
      AutoSize = False
      Caption = 'R4'
      Layout = tlCenter
    end
    object LabelR5: TLabel
      Left = 24
      Top = 138
      Width = 33
      Height = 21
      Hint = 'Channel C Coarse Tune Register'
      AutoSize = False
      Caption = 'R5'
      Layout = tlCenter
    end
    object LabelR6: TLabel
      Left = 24
      Top = 162
      Width = 33
      Height = 21
      Hint = 'Noise Generator Control'
      AutoSize = False
      Caption = 'R6'
      Layout = tlCenter
    end
    object LabelR7: TLabel
      Left = 24
      Top = 186
      Width = 33
      Height = 21
      Hint = 'Mixer Control-I/O Enable'
      AutoSize = False
      Caption = 'R7'
      Layout = tlCenter
    end
    object LabelR9: TLabel
      Left = 24
      Top = 234
      Width = 33
      Height = 21
      Hint = 'Channel B Amplitude Control'
      AutoSize = False
      Caption = 'R11'
      Layout = tlCenter
    end
    object LabelR3: TLabel
      Left = 24
      Top = 90
      Width = 33
      Height = 21
      Hint = 'Channel B Coarse Tune Register'
      AutoSize = False
      Caption = 'R3'
      Layout = tlCenter
    end
    object LabelR10: TLabel
      Left = 24
      Top = 258
      Width = 33
      Height = 21
      Hint = 'Channel C Amplitude Control'
      AutoSize = False
      Caption = 'R12'
      Layout = tlCenter
    end
    object LabelR11: TLabel
      Left = 24
      Top = 282
      Width = 33
      Height = 21
      Hint = 'Enveloppe Period Control Fine Tune'
      AutoSize = False
      Caption = 'R13'
      Layout = tlCenter
    end
    object LabelR12: TLabel
      Left = 24
      Top = 306
      Width = 33
      Height = 21
      Hint = 'Enveloppe Period Control Coarse Tune'
      AutoSize = False
      Caption = 'R14'
      Layout = tlCenter
    end
    object LabelR13: TLabel
      Left = 24
      Top = 330
      Width = 33
      Height = 21
      Hint = 'Enveloppe Shape/Cycle Control'
      AutoSize = False
      Caption = 'R15'
      Layout = tlCenter
    end
    object LabelR14: TLabel
      Left = 24
      Top = 354
      Width = 33
      Height = 21
      Hint = 'I/O Port Data'
      AutoSize = False
      Caption = 'R16'
      Layout = tlCenter
    end
    object RadioButtonR15: TRadioButton
      Left = 8
      Top = 377
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 0
    end
    object RadioButtonR0: TRadioButton
      Left = 8
      Top = 17
      Width = 17
      Height = 21
      Checked = True
      Enabled = False
      TabOrder = 1
      TabStop = True
    end
    object RadioButtonR1: TRadioButton
      Left = 8
      Top = 41
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 2
    end
    object RadioButtonR2: TRadioButton
      Left = 8
      Top = 65
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 3
    end
    object RadioButtonR8: TRadioButton
      Left = 8
      Top = 209
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 4
    end
    object RadioButtonR4: TRadioButton
      Left = 8
      Top = 113
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 5
    end
    object RadioButtonR5: TRadioButton
      Left = 8
      Top = 137
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 6
    end
    object RadioButtonR6: TRadioButton
      Left = 8
      Top = 161
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 7
    end
    object RadioButtonR7: TRadioButton
      Left = 8
      Top = 185
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 8
    end
    object RadioButtonR9: TRadioButton
      Left = 8
      Top = 233
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 9
    end
    object RadioButtonR3: TRadioButton
      Left = 8
      Top = 89
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 10
    end
    object RadioButtonR10: TRadioButton
      Left = 8
      Top = 257
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 11
    end
    object RadioButtonR11: TRadioButton
      Left = 8
      Top = 281
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 12
    end
    object RadioButtonR12: TRadioButton
      Left = 8
      Top = 305
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 13
    end
    object RadioButtonR13: TRadioButton
      Left = 8
      Top = 329
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 14
    end
    object RadioButtonR14: TRadioButton
      Left = 8
      Top = 353
      Width = 17
      Height = 21
      Enabled = False
      TabOrder = 15
    end
    object PanelFormatR0: TPanel
      Left = 56
      Top = 18
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 16
      OnClick = PanelFormatClick
      OnDblClick = PanelFormatClick
    end
    object PanelFormatR1: TPanel
      Tag = 1
      Left = 56
      Top = 42
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 17
      OnClick = PanelFormatClick
      OnDblClick = PanelFormatClick
    end
    object PanelFormatR2: TPanel
      Tag = 2
      Left = 56
      Top = 66
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 18
      OnClick = PanelFormatClick
      OnDblClick = PanelFormatClick
    end
    object PanelFormatR3: TPanel
      Tag = 3
      Left = 56
      Top = 90
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 19
      OnClick = PanelFormatClick
      OnDblClick = PanelFormatClick
    end
    object PanelFormatR4: TPanel
      Tag = 4
      Left = 56
      Top = 114
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 20
      OnClick = PanelFormatClick
      OnDblClick = PanelFormatClick
    end
    object PanelFormatR5: TPanel
      Tag = 5
      Left = 56
      Top = 138
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 21
      OnClick = PanelFormatClick
      OnDblClick = PanelFormatClick
    end
    object PanelFormatR6: TPanel
      Tag = 6
      Left = 56
      Top = 162
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 22
      OnClick = PanelFormatClick
      OnDblClick = PanelFormatClick
    end
    object PanelFormatR7: TPanel
      Tag = 7
      Left = 56
      Top = 186
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 23
      OnClick = PanelFormatClick
      OnDblClick = PanelFormatClick
    end
    object PanelFormatR8: TPanel
      Tag = 8
      Left = 56
      Top = 210
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 24
      OnClick = PanelFormatClick
      OnDblClick = PanelFormatClick
    end
    object PanelFormatR9: TPanel
      Tag = 9
      Left = 56
      Top = 234
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 25
      OnClick = PanelFormatClick
      OnDblClick = PanelFormatClick
    end
    object PanelFormatR10: TPanel
      Tag = 10
      Left = 56
      Top = 258
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 26
      OnClick = PanelFormatClick
      OnDblClick = PanelFormatClick
    end
    object PanelFormatR11: TPanel
      Tag = 11
      Left = 56
      Top = 282
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 27
      OnClick = PanelFormatClick
      OnDblClick = PanelFormatClick
    end
    object PanelFormatR12: TPanel
      Tag = 12
      Left = 56
      Top = 306
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 28
      OnClick = PanelFormatClick
      OnDblClick = PanelFormatClick
    end
    object PanelFormatR13: TPanel
      Tag = 13
      Left = 56
      Top = 330
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 29
      OnClick = PanelFormatClick
      OnDblClick = PanelFormatClick
    end
    object PanelFormatR14: TPanel
      Tag = 14
      Left = 56
      Top = 354
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 30
      OnClick = PanelFormatClick
      OnDblClick = PanelFormatClick
    end
    object PanelFormatR15: TPanel
      Tag = 15
      Left = 56
      Top = 378
      Width = 17
      Height = 21
      Caption = '&&'
      Color = 16773087
      ParentBackground = False
      TabOrder = 31
      OnClick = PanelFormatClick
      OnDblClick = PanelFormatClick
    end
    object PanelR0: TPanel
      Left = 73
      Top = 18
      Width = 40
      Height = 21
      Hint = 'Channel A Fine Tone Period Register'
      ParentBackground = False
      ParentColor = True
      TabOrder = 32
    end
    object PanelR1: TPanel
      Tag = 1
      Left = 73
      Top = 42
      Width = 40
      Height = 21
      Hint = 'Channel A Coarse Tone Period Register'
      ParentBackground = False
      ParentColor = True
      TabOrder = 33
    end
    object PanelR2: TPanel
      Tag = 2
      Left = 73
      Top = 66
      Width = 40
      Height = 21
      Hint = 'Channel B Fine Tone Period Register'
      ParentBackground = False
      ParentColor = True
      TabOrder = 34
    end
    object PanelR3: TPanel
      Tag = 3
      Left = 73
      Top = 90
      Width = 40
      Height = 21
      Hint = 'Channel B Coarse Tone Period Register'
      ParentBackground = False
      ParentColor = True
      TabOrder = 35
    end
    object PanelR4: TPanel
      Tag = 4
      Left = 73
      Top = 114
      Width = 40
      Height = 21
      Hint = 'Channel C Fine Tone Period Register'
      ParentBackground = False
      ParentColor = True
      TabOrder = 36
    end
    object PanelR5: TPanel
      Tag = 5
      Left = 73
      Top = 138
      Width = 40
      Height = 21
      Hint = 'Channel C Coarse Tone Period Register'
      ParentBackground = False
      ParentColor = True
      TabOrder = 37
    end
    object PanelR6: TPanel
      Tag = 6
      Left = 73
      Top = 162
      Width = 40
      Height = 21
      Hint = 'Noise Period Control'
      ParentBackground = False
      ParentColor = True
      TabOrder = 38
    end
    object PanelR7: TPanel
      Tag = 7
      Left = 73
      Top = 186
      Width = 40
      Height = 21
      Hint = 'Mixer Control-I/O Enable'
      ParentBackground = False
      ParentColor = True
      TabOrder = 39
    end
    object PanelR8: TPanel
      Tag = 8
      Left = 73
      Top = 210
      Width = 40
      Height = 21
      Hint = 'Channel A Amplitude Control'
      ParentBackground = False
      ParentColor = True
      TabOrder = 40
    end
    object PanelR9: TPanel
      Tag = 9
      Left = 73
      Top = 234
      Width = 40
      Height = 21
      Hint = 'Channel B Amplitude Control'
      ParentBackground = False
      ParentColor = True
      TabOrder = 41
    end
    object PanelR10: TPanel
      Tag = 10
      Left = 73
      Top = 258
      Width = 40
      Height = 21
      Hint = 'Channel C Amplitude Control'
      ParentBackground = False
      ParentColor = True
      TabOrder = 42
    end
    object PanelR11: TPanel
      Tag = 11
      Left = 73
      Top = 282
      Width = 40
      Height = 21
      Hint = 'Envelope Fine Period Register'
      ParentBackground = False
      ParentColor = True
      TabOrder = 43
    end
    object PanelR12: TPanel
      Tag = 12
      Left = 73
      Top = 306
      Width = 40
      Height = 21
      Hint = 'Envelope Coarse Period Register'
      ParentBackground = False
      ParentColor = True
      TabOrder = 44
    end
    object PanelR13: TPanel
      Tag = 13
      Left = 73
      Top = 330
      Width = 40
      Height = 21
      Hint = 'Envelope Shape/Cycle Control'
      ParentBackground = False
      ParentColor = True
      TabOrder = 45
    end
    object PanelR14: TPanel
      Tag = 14
      Left = 73
      Top = 354
      Width = 40
      Height = 21
      Hint = 'I/O Port Data'
      ParentBackground = False
      ParentColor = True
      TabOrder = 46
    end
    object PanelR15: TPanel
      Tag = 15
      Left = 73
      Top = 378
      Width = 40
      Height = 21
      Hint = 'I/O Port Data'
      ParentBackground = False
      ParentColor = True
      TabOrder = 47
    end
  end
  object GroupBoxChannels: TGroupBox
    Left = 144
    Top = 8
    Width = 323
    Height = 113
    Anchors = [akLeft, akTop, akRight]
    Caption = ' Channels '
    TabOrder = 2
    DesignSize = (
      323
      113)
    object PanelChannelAFreq: TPanel
      Left = 49
      Top = 18
      Width = 80
      Height = 21
      Hint = 'Channel A Tone frequency'
      ParentBackground = False
      ParentColor = True
      TabOrder = 0
    end
    object PanelChannelBFreq: TPanel
      Left = 49
      Top = 50
      Width = 80
      Height = 21
      Hint = 'Channel B Tone frequency'
      ParentBackground = False
      ParentColor = True
      TabOrder = 1
    end
    object PanelChannelCFreq: TPanel
      Left = 49
      Top = 82
      Width = 80
      Height = 21
      Hint = 'Channel C Tone frequency'
      ParentBackground = False
      ParentColor = True
      TabOrder = 2
    end
    object PanelChannelANoise: TPanel
      Left = 129
      Top = 18
      Width = 48
      Height = 21
      Hint = 'Channel A Noise Enabled (green) / Disabled (red)'
      Caption = 'Noise'
      ParentBackground = False
      ParentColor = True
      TabOrder = 3
    end
    object PanelChannelBNoise: TPanel
      Left = 129
      Top = 50
      Width = 48
      Height = 21
      Hint = 'Channel B Noise Enabled (green) / Disabled (red)'
      Caption = 'Noise'
      ParentBackground = False
      ParentColor = True
      TabOrder = 4
    end
    object PanelChannelCNoise: TPanel
      Left = 129
      Top = 82
      Width = 48
      Height = 21
      Hint = 'Channel C Noise Enabled (green) / Disabled (red)'
      Caption = 'Noise'
      ParentBackground = False
      ParentColor = True
      TabOrder = 5
    end
    object ProgressBarChannelAAmplitude: TProgressBar
      Left = 216
      Top = 18
      Width = 99
      Height = 21
      Hint = 'Channel A Amplitude'
      Anchors = [akLeft, akTop, akRight]
      Smooth = True
      Step = 5
      TabOrder = 6
    end
    object ProgressBarChannelBAmplitude: TProgressBar
      Left = 216
      Top = 50
      Width = 99
      Height = 21
      Hint = 'Channel B Amplitude'
      Anchors = [akLeft, akTop, akRight]
      Smooth = True
      Step = 5
      TabOrder = 7
    end
    object ProgressBarChannelCAmplitude: TProgressBar
      Left = 216
      Top = 82
      Width = 99
      Height = 21
      Hint = 'Channel C Amplitude'
      Anchors = [akLeft, akTop, akRight]
      Smooth = True
      Step = 5
      TabOrder = 8
    end
    object PanelChannelAEnv: TPanel
      Left = 177
      Top = 18
      Width = 32
      Height = 21
      Hint = 'Channel A Envelope Enabled (green) / Disabled (red)'
      Caption = 'Env'
      ParentBackground = False
      ParentColor = True
      TabOrder = 9
    end
    object PanelChannelBEnv: TPanel
      Left = 177
      Top = 50
      Width = 32
      Height = 21
      Hint = 'Channel B Envelope Enabled (green) / Disabled (red)'
      Caption = 'Env'
      ParentBackground = False
      ParentColor = True
      TabOrder = 10
    end
    object PanelChannelCEnv: TPanel
      Left = 177
      Top = 82
      Width = 32
      Height = 21
      Hint = 'Channel C Envelope Enabled (green) / Disabled (red)'
      Caption = 'Env'
      ParentBackground = False
      ParentColor = True
      TabOrder = 11
    end
    object PanelChannelA: TPanel
      Left = 9
      Top = 18
      Width = 32
      Height = 21
      Hint = 
        'Channel A Enabled (green) / Disabled (red). Click to Enable/Disa' +
        'ble.'
      Caption = 'A'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentBackground = False
      ParentColor = True
      ParentFont = False
      TabOrder = 12
      OnClick = PanelChannelAClick
    end
    object PanelChannelB: TPanel
      Left = 9
      Top = 50
      Width = 32
      Height = 21
      Hint = 
        'Channel B Enabled (green) / Disabled (red).  Click to Enable/Dis' +
        'able.'
      Caption = 'B'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentBackground = False
      ParentColor = True
      ParentFont = False
      TabOrder = 13
      OnClick = PanelChannelBClick
    end
    object PanelChannelC: TPanel
      Left = 9
      Top = 82
      Width = 32
      Height = 21
      Hint = 
        'Channel C Enabled (green) / Disabled (red).  Click to Enable/Dis' +
        'able.'
      Caption = 'C'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentBackground = False
      ParentColor = True
      ParentFont = False
      TabOrder = 14
      OnClick = PanelChannelCClick
    end
  end
  object GroupBoxEnvelope: TGroupBox
    Left = 240
    Top = 128
    Width = 227
    Height = 49
    Anchors = [akLeft, akTop, akRight]
    Caption = ' Envelope '
    TabOrder = 3
    DesignSize = (
      227
      49)
    object PanelEnveloppeFreq: TPanel
      Left = 10
      Top = 20
      Width = 79
      Height = 21
      Hint = 'Envelope frequency'
      ParentBackground = False
      ParentColor = True
      TabOrder = 0
    end
    object ChartEnvelope: TChart
      Left = 96
      Top = 16
      Width = 123
      Height = 25
      Hint = 'Envelope shape'
      AllowPanning = pmNone
      BackWall.Brush.Style = bsClear
      BackWall.Pen.Visible = False
      Legend.Visible = False
      MarginBottom = 1
      MarginLeft = 1
      MarginRight = 1
      MarginTop = 1
      Title.Text.Strings = (
        'TChart')
      Title.Visible = False
      Title.AdjustFrame = False
      AxisVisible = False
      ClipPoints = False
      Frame.Visible = False
      LeftAxis.Automatic = False
      LeftAxis.AutomaticMaximum = False
      LeftAxis.AutomaticMinimum = False
      LeftAxis.Labels = False
      LeftAxis.LabelsFormat.Visible = False
      LeftAxis.Maximum = 12.000000000000000000
      LeftAxis.Minimum = -1.000000000000000000
      LeftAxis.Visible = False
      View3D = False
      View3DWalls = False
      Zoom.Allow = False
      BevelOuter = bvNone
      ParentColor = True
      TabOrder = 1
      Anchors = [akLeft, akTop, akRight]
      DefaultCanvas = 'TGDIPlusCanvas'
      ColorPaletteIndex = 13
      object FastLineSeries2: TFastLineSeries
        Legend.Visible = False
        SeriesColor = 16744448
        ShowInLegend = False
        LinePen.Color = 16744448
        XValues.Name = 'X '
        XValues.Order = loAscending
        YValues.Name = 'Y '
        YValues.Order = loNone
      end
    end
  end
  object GroupBoxNoise: TGroupBox
    Left = 144
    Top = 128
    Width = 97
    Height = 49
    Caption = ' Noise '
    TabOrder = 4
    object PanelNoiseFreq: TPanel
      Left = 9
      Top = 20
      Width = 80
      Height = 21
      Hint = 'Noise frequency'
      ParentBackground = False
      ParentColor = True
      TabOrder = 0
    end
  end
end
