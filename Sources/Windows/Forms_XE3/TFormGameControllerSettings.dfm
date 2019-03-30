object FormGameControllerSettings: TFormGameControllerSettings
  Left = 648
  Top = 287
  BorderIcons = []
  Caption = 'Game controllers settings'
  ClientHeight = 474
  ClientWidth = 384
  Color = clBtnFace
  Constraints.MinHeight = 512
  Constraints.MinWidth = 400
  Font.Charset = ANSI_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Verdana'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnResize = FormResize
  OnShow = FormShow
  DesignSize = (
    384
    474)
  PixelsPerInch = 96
  TextHeight = 13
  object ComboBoxControllersList: TComboBox
    Left = 8
    Top = 8
    Width = 273
    Height = 22
    Style = csOwnerDrawFixed
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 1
    OnChange = ComboBoxControllersListChange
    OnDrawItem = ComboBoxControllersListDrawItem
  end
  object ButtonProperties: TButton
    Left = 288
    Top = 8
    Width = 89
    Height = 23
    Anchors = [akTop, akRight]
    Caption = 'Properties'
    TabOrder = 2
    OnClick = ButtonPropertiesClick
  end
  object PageControlActions: TPageControl
    Left = 8
    Top = 40
    Width = 369
    Height = 293
    ActivePage = TabSheetAnalog
    Anchors = [akLeft, akTop, akRight, akBottom]
    HotTrack = True
    OwnerDraw = True
    TabHeight = 20
    TabOrder = 0
    OnDrawTab = PageControlActionsDrawTab
    object TabSheetButtons: TTabSheet
      Caption = 'Buttons'
      OnShow = TabSheetButtonsShow
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      DesignSize = (
        361
        263)
      object StringGridButtons: TStringGrid
        Left = 16
        Top = 8
        Width = 329
        Height = 240
        Anchors = [akLeft, akTop, akRight, akBottom]
        BorderStyle = bsNone
        ColCount = 2
        DoubleBuffered = True
        RowCount = 1
        FixedRows = 0
        Options = [goRangeSelect]
        ParentColor = True
        ParentDoubleBuffered = False
        ScrollBars = ssVertical
        TabOrder = 0
        OnDrawCell = StringGridButtonsDrawCell
        OnMouseDown = StringGridButtonsMouseDown
      end
    end
    object TabSheetPOV: TTabSheet
      Caption = 'POV'
      ImageIndex = 1
      OnShow = TabSheetPOVShow
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      DesignSize = (
        361
        263)
      object POVId: TLabel
        Left = 16
        Top = 8
        Width = 62
        Height = 25
        AutoSize = False
        Caption = 'POV'
        Layout = tlCenter
      end
      object ComboBoxPOVId: TComboBox
        Left = 88
        Top = 8
        Width = 57
        Height = 22
        Style = csOwnerDrawFixed
        TabOrder = 0
        OnChange = ComboBoxPOVIdChange
        OnDrawItem = ComboBoxPOVIdDrawItem
      end
      object StringGridPOV: TStringGrid
        Left = 16
        Top = 48
        Width = 329
        Height = 105
        Anchors = [akLeft, akTop, akRight]
        BorderStyle = bsNone
        ColCount = 2
        DoubleBuffered = True
        RowCount = 4
        FixedRows = 0
        Options = [goRangeSelect]
        ParentColor = True
        ParentDoubleBuffered = False
        ScrollBars = ssVertical
        TabOrder = 1
        OnDrawCell = StringGridPOVDrawCell
        OnMouseDown = StringGridPOVMouseDown
      end
    end
    object TabSheetAnalog: TTabSheet
      Caption = 'Analog'
      ImageIndex = 2
      OnShow = TabSheetAnalogShow
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      DesignSize = (
        361
        263)
      object LabelAxis: TLabel
        Left = 16
        Top = 8
        Width = 105
        Height = 25
        AutoSize = False
        Caption = 'Axis'
        Layout = tlCenter
      end
      object LabelAnalogPressThreshold: TLabel
        Left = 16
        Top = 176
        Width = 153
        Height = 33
        AutoSize = False
        Caption = 'Press threshold'
        Layout = tlCenter
      end
      object LabelAnalogDeadZone: TLabel
        Left = 16
        Top = 216
        Width = 153
        Height = 33
        AutoSize = False
        Caption = 'Dead zone'
        Layout = tlCenter
      end
      object StringGridAnalog: TStringGrid
        Left = 16
        Top = 48
        Width = 329
        Height = 57
        Anchors = [akLeft, akTop, akRight]
        BorderStyle = bsNone
        ColCount = 2
        DoubleBuffered = True
        RowCount = 2
        FixedRows = 0
        Options = [goRangeSelect]
        ParentColor = True
        ParentDoubleBuffered = False
        ScrollBars = ssVertical
        TabOrder = 0
        OnDrawCell = StringGridAnalogDrawCell
        OnMouseDown = StringGridAnalogMouseDown
      end
      object ComboBoxAnalogAxis: TComboBox
        Left = 120
        Top = 8
        Width = 145
        Height = 22
        Style = csOwnerDrawFixed
        TabOrder = 1
        OnChange = ComboBoxAnalogAxisChange
        OnDrawItem = ComboBoxAnalogAxisDrawItem
      end
      object TrackBarAnalogPressThreshold: TTrackBar
        Left = 168
        Top = 176
        Width = 185
        Height = 33
        Hint = 'Threshold to detect button press '
        Max = 100
        PageSize = 10
        Frequency = 10
        Position = 50
        TabOrder = 2
        OnChange = TrackBarAnalogPressThresholdChange
      end
      object CheckBoxAnalogMouseX: TCheckBox
        Left = 16
        Top = 120
        Width = 257
        Height = 17
        Caption = 'Fullscreen Mouse X'
        TabOrder = 3
        OnClick = CheckBoxAnalogMouseXClick
      end
      object CheckBoxAnalogMouseY: TCheckBox
        Left = 16
        Top = 144
        Width = 257
        Height = 17
        Caption = 'Fullscreen Mouse Y'
        TabOrder = 4
        OnClick = CheckBoxAnalogMouseYClick
      end
      object TrackBarAnalogDeadZone: TTrackBar
        Left = 168
        Top = 216
        Width = 185
        Height = 33
        Hint = 'Threshold to detect analog movement'
        Max = 20
        Frequency = 2
        Position = 10
        TabOrder = 5
        OnChange = TrackBarAnalogDeadZoneChange
      end
    end
  end
  object ButtonApply: TButton
    Left = 176
    Top = 431
    Width = 108
    Height = 35
    Anchors = [akRight, akBottom]
    Caption = 'Save'
    Default = True
    ModalResult = 1
    TabOrder = 3
  end
  object ButtonCancel: TButton
    Left = 291
    Top = 431
    Width = 89
    Height = 35
    Anchors = [akRight, akBottom]
    Caption = 'Cancel'
    Default = True
    ModalResult = 2
    TabOrder = 4
  end
  object GroupBoxProfile: TGroupBox
    Left = 8
    Top = 340
    Width = 369
    Height = 80
    Anchors = [akLeft, akRight, akBottom]
    Caption = ' Profile '
    TabOrder = 5
    DesignSize = (
      369
      80)
    object LabelFromProfile: TLabel
      Left = 8
      Top = 16
      Width = 81
      Height = 21
      AutoSize = False
      Caption = 'From'
      Layout = tlCenter
    end
    object LabelSaveProfile: TLabel
      Left = 8
      Top = 48
      Width = 81
      Height = 21
      AutoSize = False
      Caption = 'Save into'
      Layout = tlCenter
    end
    object ComboBoxProfiles: TComboBox
      Left = 96
      Top = 48
      Width = 265
      Height = 22
      Style = csOwnerDrawFixed
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 0
      OnChange = ComboBoxProfilesChange
      OnDrawItem = ComboBoxProfilesDrawItem
    end
    object PanelFromProfile: TPanel
      Left = 96
      Top = 16
      Width = 265
      Height = 25
      Alignment = taLeftJustify
      BevelOuter = bvNone
      ParentColor = True
      TabOrder = 1
    end
  end
  object ButtonReset: TButton
    Left = 8
    Top = 431
    Width = 73
    Height = 35
    Anchors = [akLeft, akBottom]
    Caption = 'Reset'
    Default = True
    TabOrder = 6
    OnClick = ButtonResetClick
  end
end
