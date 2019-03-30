object FormDebugMEA8000: TFormDebugMEA8000
  Left = 587
  Top = 206
  BorderIcons = [biSystemMenu]
  Caption = 'MEA8000'
  ClientHeight = 185
  ClientWidth = 553
  Color = clBtnFace
  Constraints.MinHeight = 223
  Constraints.MinWidth = 569
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
    553
    185)
  PixelsPerInch = 96
  TextHeight = 13
  object LabelState: TLabel
    Left = 8
    Top = 8
    Width = 89
    Height = 21
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'State'
    Layout = tlCenter
  end
  object LabelPitch: TLabel
    Left = 8
    Top = 40
    Width = 89
    Height = 21
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'Pitch'
    Layout = tlCenter
  end
  object LabelAmplitude: TLabel
    Left = 8
    Top = 64
    Width = 89
    Height = 21
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'Amplitude'
    Layout = tlCenter
  end
  object PanelState: TPanel
    Left = 104
    Top = 8
    Width = 121
    Height = 21
    Hint = 'Horizontal Total'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentBackground = False
    ParentColor = True
    ParentFont = False
    TabOrder = 0
  end
  object PanelPitch: TPanel
    Left = 104
    Top = 40
    Width = 49
    Height = 21
    Hint = 'Horizontal Total'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentBackground = False
    ParentColor = True
    ParentFont = False
    TabOrder = 1
  end
  object PanelAmplitude: TPanel
    Left = 104
    Top = 64
    Width = 49
    Height = 21
    Hint = 'Horizontal Total'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentBackground = False
    ParentColor = True
    ParentFont = False
    TabOrder = 2
  end
  object GroupBoxOutput: TGroupBox
    Left = 8
    Top = 96
    Width = 217
    Height = 81
    Caption = ' Output '
    TabOrder = 3
    DesignSize = (
      217
      81)
    object ChartSample: TChart
      Left = 8
      Top = 24
      Width = 201
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
      LeftAxis.Minimum = -32768.000000000000000000
      LeftAxis.Visible = False
      View3D = False
      View3DWalls = False
      Zoom.Allow = False
      BevelInner = bvRaised
      BevelOuter = bvNone
      ParentColor = True
      TabOrder = 0
      Anchors = [akLeft, akTop, akRight, akBottom]
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
  object GroupBoxMonitoring: TGroupBox
    Left = 232
    Top = 8
    Width = 315
    Height = 169
    Anchors = [akLeft, akTop, akRight, akBottom]
    Caption = ' Monitor '
    TabOrder = 4
    object PanelHistoryControls: TPanel
      Left = 2
      Top = 15
      Width = 311
      Height = 74
      Align = alTop
      BevelOuter = bvNone
      ParentColor = True
      TabOrder = 0
      DesignSize = (
        311
        74)
      object ButtonMonitorStartStop: TButton
        Left = 8
        Top = 8
        Width = 199
        Height = 25
        Anchors = [akLeft, akTop, akRight]
        Caption = 'Start'
        TabOrder = 0
        OnClick = ButtonMonitorStartStopClick
      end
      object ButtonHistoryClear: TButton
        Left = 214
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
        Width = 161
        Height = 17
        Caption = 'Show READ Status'
        TabOrder = 2
      end
      object CheckBoxShowSamples: TCheckBox
        Left = 176
        Top = 40
        Width = 121
        Height = 17
        Caption = 'Show Samples'
        TabOrder = 3
      end
    end
    object ListBoxHistory: TListBox
      Left = 2
      Top = 89
      Width = 311
      Height = 78
      Align = alClient
      BorderStyle = bsNone
      IntegralHeight = True
      ItemHeight = 13
      ParentColor = True
      TabOrder = 1
      TabWidth = 30
    end
  end
end
