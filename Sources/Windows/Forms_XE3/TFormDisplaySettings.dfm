object FormDisplaySettings: TFormDisplaySettings
  Left = 192
  Top = 124
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Display settings'
  ClientHeight = 322
  ClientWidth = 465
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnShow = FormShow
  DesignSize = (
    465
    322)
  PixelsPerInch = 96
  TextHeight = 13
  object ButtonCancel: TButton
    Left = 332
    Top = 272
    Width = 116
    Height = 41
    Anchors = [akRight, akBottom]
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 0
    OnClick = ButtonCancelClick
  end
  object ButtonOK: TButton
    Left = 332
    Top = 224
    Width = 116
    Height = 41
    Anchors = [akRight, akBottom]
    Caption = 'Apply'
    Default = True
    ModalResult = 1
    TabOrder = 7
    OnClick = ButtonOKClick
  end
  object CheckBoxFullscreenWithMenu: TCheckBox
    Left = 16
    Top = 40
    Width = 297
    Height = 17
    Caption = 'Fullscreen with menu'
    TabOrder = 2
  end
  object CheckBoxDrawScanlines: TCheckBox
    Left = 16
    Top = 88
    Width = 297
    Height = 17
    Caption = 'Draw scanlines'
    TabOrder = 4
    OnClick = CheckBoxDrawScanlinesClick
  end
  object CheckBoxKeepAspectRatio: TCheckBox
    Left = 16
    Top = 64
    Width = 297
    Height = 17
    Caption = 'Keep original aspect ratio'
    TabOrder = 3
  end
  object GroupBoxOSD: TGroupBox
    Left = 328
    Top = 8
    Width = 121
    Height = 129
    Caption = ' Onscreen '
    TabOrder = 6
    object CheckBoxOSDIcons: TCheckBox
      Left = 16
      Top = 24
      Width = 89
      Height = 17
      Caption = 'Icons'
      TabOrder = 0
    end
    object CheckBoxOSDDisk: TCheckBox
      Left = 16
      Top = 72
      Width = 81
      Height = 17
      Caption = 'Disk'
      TabOrder = 2
    end
    object CheckBoxOSDTape: TCheckBox
      Left = 16
      Top = 96
      Width = 81
      Height = 17
      Caption = 'Tape'
      TabOrder = 3
    end
    object CheckBoxOSDMessages: TCheckBox
      Left = 16
      Top = 48
      Width = 81
      Height = 17
      Caption = 'Messages'
      TabOrder = 1
    end
  end
  object CheckBoxKeepFullscreen: TCheckBox
    Left = 16
    Top = 16
    Width = 297
    Height = 17
    Caption = 'Keep fullscreen for next session'
    TabOrder = 1
  end
  object CheckBoxShowInfoDuringPause: TCheckBox
    Left = 16
    Top = 112
    Width = 297
    Height = 17
    Caption = 'Show Info during Pause'
    TabOrder = 5
    OnClick = CheckBoxDrawScanlinesClick
  end
  object GroupBoxTheme: TGroupBox
    Left = 16
    Top = 152
    Width = 233
    Height = 161
    Caption = ' Theme '
    TabOrder = 8
    object LabelThemeBackground: TLabel
      Left = 24
      Top = 56
      Width = 121
      Height = 25
      AutoSize = False
      Caption = 'Background'
      Layout = tlCenter
    end
    object LabelThemeText1: TLabel
      Left = 24
      Top = 88
      Width = 121
      Height = 25
      AutoSize = False
      Caption = 'Text 1'
      Layout = tlCenter
    end
    object LabelThemeText2: TLabel
      Left = 24
      Top = 120
      Width = 121
      Height = 25
      AutoSize = False
      Caption = 'Text 2'
      Layout = tlCenter
    end
    object ComboBoxTheme: TComboBox
      Left = 16
      Top = 24
      Width = 201
      Height = 22
      Style = csOwnerDrawFixed
      TabOrder = 0
      OnChange = ComboBoxThemeChange
      OnDrawItem = ComboBoxThemeDrawItem
      Items.Strings = (
        'Custom'
        'Reset'
        'Harrier Attack'
        'Zombi'
        'Bob Winner'
        'ATF'
        'Discology')
    end
    object PanelThemeBackground: TPanel
      Left = 152
      Top = 56
      Width = 65
      Height = 25
      Hint = 'Click to select new background color.'
      BevelInner = bvLowered
      ParentShowHint = False
      ShowHint = True
      TabOrder = 1
      OnClick = PanelThemeBackgroundClick
    end
    object PanelThemeText1: TPanel
      Left = 152
      Top = 88
      Width = 65
      Height = 25
      Hint = 'Click to select new text color.'
      BevelInner = bvLowered
      ParentShowHint = False
      ShowHint = True
      TabOrder = 2
      OnClick = PanelThemeText1Click
    end
    object PanelThemeText2: TPanel
      Left = 152
      Top = 120
      Width = 65
      Height = 25
      Hint = 'Click to select new text color.'
      BevelInner = bvLowered
      ParentShowHint = False
      ShowHint = True
      TabOrder = 3
      OnClick = PanelThemeText2Click
    end
  end
  object GroupBoxVHoldAdjust: TGroupBox
    Left = 264
    Top = 152
    Width = 185
    Height = 49
    Caption = ' V-HOLD Adjust '
    TabOrder = 9
    DesignSize = (
      185
      49)
    object TrackBarVHOLDAdjust: TTrackBar
      Left = 8
      Top = 16
      Width = 169
      Height = 25
      Hint = 'Adjust top of display'
      Anchors = [akLeft, akTop, akRight]
      Max = 270
      Min = 230
      Frequency = 5
      Position = 250
      ShowSelRange = False
      TabOrder = 0
      TickMarks = tmBoth
      TickStyle = tsNone
      OnChange = TrackBarVHOLDAdjustChange
    end
  end
end
