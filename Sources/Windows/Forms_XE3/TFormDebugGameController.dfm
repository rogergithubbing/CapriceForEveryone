object FormDebugGameController: TFormDebugGameController
  Left = 856
  Top = 206
  Caption = 'Game controller'
  ClientHeight = 229
  ClientWidth = 249
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 249
    Height = 229
    ActivePage = TabSheetAnalog
    Align = alClient
    TabOrder = 0
    object TabSheetButtons: TTabSheet
      Caption = 'Buttons'
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object SpeedButtonButton1: TSpeedButton
        Left = 8
        Top = 8
        Width = 81
        Height = 41
        AllowAllUp = True
        GroupIndex = 1
        Caption = '1'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'Verdana'
        Font.Style = []
        ParentFont = False
      end
      object SpeedButtonButton2: TSpeedButton
        Left = 8
        Top = 56
        Width = 81
        Height = 41
        AllowAllUp = True
        GroupIndex = 2
        Caption = '2'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'Verdana'
        Font.Style = []
        ParentFont = False
      end
      object SpeedButtonButton3: TSpeedButton
        Left = 8
        Top = 104
        Width = 81
        Height = 41
        AllowAllUp = True
        GroupIndex = 3
        Caption = '3'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'Verdana'
        Font.Style = []
        ParentFont = False
      end
      object SpeedButtonButton4: TSpeedButton
        Left = 8
        Top = 152
        Width = 81
        Height = 41
        AllowAllUp = True
        GroupIndex = 4
        Caption = '4'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'Verdana'
        Font.Style = []
        ParentFont = False
      end
    end
    object TabSheetPOV: TTabSheet
      Caption = 'POV'
      ImageIndex = 1
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object GroupBoxPOV1: TGroupBox
        Left = 8
        Top = 16
        Width = 161
        Height = 145
        Caption = ' POV1 '
        TabOrder = 0
        object SpeedButtonPOV1Up: TSpeedButton
          Left = 56
          Top = 16
          Width = 49
          Height = 33
          AllowAllUp = True
          GroupIndex = 1
          Caption = 'UP'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Verdana'
          Font.Style = []
          ParentFont = False
        end
        object SpeedButtonPOV1Right: TSpeedButton
          Left = 104
          Top = 56
          Width = 49
          Height = 33
          AllowAllUp = True
          GroupIndex = 2
          Caption = 'R'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Verdana'
          Font.Style = []
          ParentFont = False
        end
        object SpeedButtonPOV1Down: TSpeedButton
          Left = 56
          Top = 96
          Width = 49
          Height = 33
          AllowAllUp = True
          GroupIndex = 3
          Caption = 'DN'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Verdana'
          Font.Style = []
          ParentFont = False
        end
        object SpeedButtonPOV1Left: TSpeedButton
          Left = 8
          Top = 56
          Width = 49
          Height = 33
          AllowAllUp = True
          GroupIndex = 4
          Caption = 'L'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Verdana'
          Font.Style = []
          ParentFont = False
        end
      end
    end
    object TabSheetAnalog: TTabSheet
      Caption = 'Analog'
      ImageIndex = 2
      object LabelXAxis: TLabel
        Left = 16
        Top = 16
        Width = 73
        Height = 33
        AutoSize = False
        Caption = 'X axis'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        Layout = tlCenter
      end
      object LabelYAxis: TLabel
        Left = 16
        Top = 56
        Width = 73
        Height = 33
        AutoSize = False
        Caption = 'Y axis'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        Layout = tlCenter
      end
      object LabelZAxis: TLabel
        Left = 16
        Top = 112
        Width = 73
        Height = 33
        AutoSize = False
        Caption = 'Z axis'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        Layout = tlCenter
      end
      object LabelRxAxis: TLabel
        Left = 16
        Top = 152
        Width = 73
        Height = 33
        AutoSize = False
        Caption = 'Rx axis'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        Layout = tlCenter
      end
      object TrackBarXAxis: TTrackBar
        Left = 88
        Top = 16
        Width = 150
        Height = 33
        Max = 1000
        Min = -1000
        Frequency = 200
        TabOrder = 0
      end
      object TrackBarYAxis: TTrackBar
        Left = 88
        Top = 56
        Width = 150
        Height = 33
        Max = 1000
        Min = -1000
        Frequency = 200
        TabOrder = 1
      end
      object TrackBarZAxis: TTrackBar
        Left = 88
        Top = 112
        Width = 150
        Height = 33
        Max = 1000
        Min = -1000
        Frequency = 200
        TabOrder = 2
      end
      object TrackBarRxAxis: TTrackBar
        Left = 88
        Top = 152
        Width = 150
        Height = 33
        Max = 1000
        Min = -1000
        Frequency = 200
        TabOrder = 3
      end
    end
  end
end
