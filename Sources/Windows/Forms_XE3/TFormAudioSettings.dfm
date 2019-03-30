object FormAudioSettings: TFormAudioSettings
  Left = 480
  Top = 192
  BorderIcons = []
  Caption = 'Audio settings'
  ClientHeight = 420
  ClientWidth = 457
  Color = clBtnFace
  Constraints.MaxHeight = 458
  Constraints.MinHeight = 458
  Constraints.MinWidth = 473
  DoubleBuffered = True
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnShow = FormShow
  DesignSize = (
    457
    420)
  PixelsPerInch = 96
  TextHeight = 13
  object LabelSpeaker: TLabel
    Left = 8
    Top = 8
    Width = 105
    Height = 16
    Alignment = taCenter
    AutoSize = False
    Caption = 'Speaker'
  end
  object LabelFloppy: TLabel
    Left = 120
    Top = 8
    Width = 73
    Height = 16
    Alignment = taCenter
    AutoSize = False
    Caption = 'Floppy'
  end
  object LabelTape: TLabel
    Left = 192
    Top = 8
    Width = 73
    Height = 16
    Alignment = taCenter
    AutoSize = False
    Caption = 'Tape'
  end
  object LabelTMPI: TLabel
    Left = 264
    Top = 8
    Width = 73
    Height = 16
    Alignment = taCenter
    AutoSize = False
    Caption = 'TMPI'
  end
  object LabelPlayCity: TLabel
    Left = 344
    Top = 8
    Width = 105
    Height = 16
    Alignment = taCenter
    AutoSize = False
    Caption = 'PlayCity'
  end
  object ButtonCancel: TButton
    Left = 359
    Top = 368
    Width = 89
    Height = 41
    Anchors = [akRight, akBottom]
    Cancel = True
    Caption = 'Cancel'
    Default = True
    ModalResult = 2
    TabOrder = 7
    OnClick = ButtonCancelClick
  end
  object ButtonOK: TButton
    Left = 239
    Top = 368
    Width = 113
    Height = 41
    Anchors = [akRight, akBottom]
    Caption = 'Apply'
    ModalResult = 1
    TabOrder = 6
    OnClick = ButtonOKClick
  end
  object GroupBoxDevice: TGroupBox
    Left = 8
    Top = 296
    Width = 440
    Height = 57
    Anchors = [akLeft, akTop, akRight]
    Caption = ' Device '
    TabOrder = 5
    DesignSize = (
      440
      57)
    object ComboBoxDevices: TComboBox
      Left = 8
      Top = 24
      Width = 424
      Height = 22
      Style = csOwnerDrawFixed
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 0
      OnDrawItem = ComboBoxDevicesDrawItem
    end
  end
  object GroupBoxGainSpeaker: TGroupBox
    Left = 8
    Top = 24
    Width = 105
    Height = 241
    TabOrder = 0
    object TrackBarGainSpeaker: TTrackBar
      Left = 32
      Top = 8
      Width = 41
      Height = 150
      Hint = 'Speaker Gain'
      Max = 0
      Min = -100
      Orientation = trVertical
      PageSize = 10
      Frequency = 10
      TabOrder = 0
      ThumbLength = 19
      TickMarks = tmBoth
      OnChange = TrackBarGainSpeakerChange
    end
    object EditGainSpeaker: TEdit
      Left = 32
      Top = 160
      Width = 41
      Height = 21
      Hint = 'Speaker Gain'
      Alignment = taCenter
      MaxLength = 3
      NumbersOnly = True
      TabOrder = 1
      Text = '0'
      OnChange = EditGainSpeakerChange
      OnClick = EditGainSpeakerClick
    end
    object RadioButtonSpeakerStereo: TRadioButton
      Left = 8
      Top = 192
      Width = 89
      Height = 17
      Caption = 'Line OUT'
      TabOrder = 2
      OnClick = RadioButtonSpeakerStereoClick
    end
    object RadioButtonSpeakerMono: TRadioButton
      Left = 8
      Top = 208
      Width = 89
      Height = 17
      Caption = 'Internal'
      TabOrder = 3
      OnClick = RadioButtonSpeakerMonoClick
    end
  end
  object GroupBoxGainFloppy: TGroupBox
    Left = 128
    Top = 24
    Width = 57
    Height = 193
    TabOrder = 1
    object TrackBarGainFloppy: TTrackBar
      Left = 8
      Top = 8
      Width = 41
      Height = 150
      Hint = 'Floppy drive Gain'
      Max = 0
      Min = -100
      Orientation = trVertical
      PageSize = 10
      Frequency = 10
      TabOrder = 0
      ThumbLength = 19
      TickMarks = tmBoth
      OnChange = TrackBarGainFloppyChange
    end
    object EditGainFloppy: TEdit
      Left = 8
      Top = 160
      Width = 41
      Height = 21
      Hint = 'Floppy drive Gain'
      Alignment = taCenter
      MaxLength = 3
      NumbersOnly = True
      TabOrder = 1
      Text = '0'
      OnChange = EditGainFloppyChange
      OnClick = EditGainFloppyClick
    end
  end
  object GroupBoxGainTape: TGroupBox
    Left = 200
    Top = 24
    Width = 57
    Height = 193
    TabOrder = 2
    object TrackBarGainTape: TTrackBar
      Left = 8
      Top = 8
      Width = 41
      Height = 150
      Hint = 'Tape deck Gain'
      Max = 0
      Min = -100
      Orientation = trVertical
      PageSize = 10
      Frequency = 10
      TabOrder = 0
      ThumbLength = 19
      TickMarks = tmBoth
      OnChange = TrackBarGainTapeChange
    end
    object EditGainTape: TEdit
      Left = 8
      Top = 160
      Width = 41
      Height = 21
      Hint = 'Tape deck Gain'
      Alignment = taCenter
      MaxLength = 3
      NumbersOnly = True
      TabOrder = 1
      Text = '0'
      OnChange = EditGainTapeChange
      OnClick = EditGainTapeClick
    end
  end
  object GroupBoxGainTMPI: TGroupBox
    Left = 272
    Top = 24
    Width = 57
    Height = 193
    TabOrder = 3
    object TrackBarGainTMPI: TTrackBar
      Left = 8
      Top = 8
      Width = 41
      Height = 150
      Hint = 'Techni-Musique Gain'
      Max = 0
      Min = -100
      Orientation = trVertical
      PageSize = 10
      Frequency = 10
      TabOrder = 0
      ThumbLength = 19
      TickMarks = tmBoth
      OnChange = TrackBarGainTMPIChange
    end
    object EditGainTMPI: TEdit
      Left = 8
      Top = 160
      Width = 41
      Height = 21
      Hint = 'Techni-Musique Gain'
      Alignment = taCenter
      MaxLength = 3
      NumbersOnly = True
      TabOrder = 1
      Text = '0'
      OnChange = EditGainTMPIChange
      OnClick = EditGainTMPIClick
    end
  end
  object GroupBoxGainPlayCity: TGroupBox
    Left = 344
    Top = 24
    Width = 105
    Height = 257
    TabOrder = 4
    object TrackBarGainPlayCity: TTrackBar
      Left = 32
      Top = 8
      Width = 41
      Height = 150
      Hint = 'PlayCity Gain'
      Max = 0
      Min = -100
      Orientation = trVertical
      PageSize = 10
      Frequency = 10
      TabOrder = 0
      ThumbLength = 19
      TickMarks = tmBoth
      OnChange = TrackBarGainPlayCityChange
    end
    object EditGainPlayCity: TEdit
      Left = 32
      Top = 160
      Width = 41
      Height = 21
      Hint = 'PlayCity Gain'
      Alignment = taCenter
      MaxLength = 3
      NumbersOnly = True
      TabOrder = 1
      Text = '0'
      OnChange = EditGainPlayCityChange
      OnClick = EditGainPlayCityClick
    end
    object RadioButtonPlayCityStereo: TRadioButton
      Left = 8
      Top = 192
      Width = 89
      Height = 17
      Caption = 'Line OUT'
      TabOrder = 2
      OnClick = RadioButtonPlayCityStereoClick
    end
    object RadioButtonPlayCityMono: TRadioButton
      Left = 8
      Top = 208
      Width = 89
      Height = 17
      Caption = 'Internal'
      TabOrder = 3
      OnClick = RadioButtonPlayCityMonoClick
    end
    object CheckBoxPlayCityInvert: TCheckBox
      Left = 8
      Top = 232
      Width = 89
      Height = 17
      Caption = 'Invert'
      TabOrder = 4
      OnClick = CheckBoxPlayCityInvertClick
    end
  end
end
