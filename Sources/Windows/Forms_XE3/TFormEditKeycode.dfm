object FormEditKeycode: TFormEditKeycode
  Left = 576
  Top = 275
  BorderIcons = []
  BorderStyle = bsSingle
  Caption = 'Edit keycode'
  ClientHeight = 272
  ClientWidth = 407
  Color = clBtnFace
  Font.Charset = ANSI_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Verdana'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnHide = FormHide
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object PageControlKeycode: TPageControl
    Left = 0
    Top = 0
    Width = 407
    Height = 222
    ActivePage = TabSheetKeyboard
    Align = alClient
    Style = tsButtons
    TabOrder = 0
    object TabSheetKeyboard: TTabSheet
      Caption = 'Keyboard'
      DesignSize = (
        399
        191)
      object LabelCurrentKeycode: TLabel
        Left = 8
        Top = 0
        Width = 153
        Height = 33
        AutoSize = False
        Caption = 'Current keycode'
        Layout = tlCenter
      end
      object LabelNewKeycode: TLabel
        Left = 8
        Top = 40
        Width = 153
        Height = 33
        AutoSize = False
        Caption = 'New keycode'
        Layout = tlCenter
      end
      object LabelFromList: TLabel
        Left = 168
        Top = 136
        Width = 49
        Height = 13
        Caption = 'From list'
      end
      object GroupBoxModifiers: TGroupBox
        Left = 8
        Top = 88
        Width = 145
        Height = 89
        Caption = ' Key Modifiers '
        TabOrder = 0
        object CheckBoxModifierShift: TCheckBox
          Left = 16
          Top = 24
          Width = 73
          Height = 17
          Caption = 'Shift'
          TabOrder = 0
          OnClick = CheckBoxModifierShiftClick
        end
        object CheckBoxModifierCtrl: TCheckBox
          Left = 16
          Top = 48
          Width = 65
          Height = 17
          Caption = 'Ctrl'
          TabOrder = 1
          OnClick = CheckBoxModifierCtrlClick
        end
      end
      object ComboBoxNewKeycode: TComboBox
        Left = 168
        Top = 152
        Width = 227
        Height = 22
        Style = csOwnerDrawFixed
        Anchors = [akLeft, akTop, akRight]
        TabOrder = 1
        OnChange = ComboBoxNewKeycodeChange
        OnClick = ComboBoxNewKeycodeClick
        OnDrawItem = ComboBoxNewKeycodeDrawItem
      end
      object ButtonNewKeycode: TButton
        Left = 168
        Top = 96
        Width = 217
        Height = 33
        Caption = 'From keyboard'
        TabOrder = 2
        OnClick = ButtonNewKeycodeClick
      end
      object PanelCurrentKeycode: TPanel
        Left = 168
        Top = 0
        Width = 217
        Height = 33
        BevelInner = bvLowered
        ParentColor = True
        TabOrder = 3
      end
      object PanelNewKeycode: TPanel
        Left = 168
        Top = 40
        Width = 217
        Height = 33
        BevelInner = bvLowered
        ParentColor = True
        TabOrder = 4
      end
    end
    object TabSheetMouse: TTabSheet
      Caption = 'Mouse'
      ImageIndex = 1
      ExplicitLeft = 0
      ExplicitTop = 32
      ExplicitWidth = 389
      ExplicitHeight = 176
      object CheckBoxMouseButton: TCheckBox
        Left = 8
        Top = 23
        Width = 241
        Height = 17
        Caption = 'Fullscreen Mouse button'
        TabOrder = 0
        OnClick = CheckBoxMouseButtonClick
      end
    end
  end
  object PanelButtons: TPanel
    Left = 0
    Top = 222
    Width = 407
    Height = 50
    Align = alBottom
    BevelOuter = bvNone
    ParentColor = True
    TabOrder = 1
    DesignSize = (
      407
      50)
    object ButtonCancel: TButton
      Left = 285
      Top = 8
      Width = 114
      Height = 34
      Anchors = [akRight, akBottom]
      Caption = 'Cancel'
      Default = True
      ModalResult = 2
      TabOrder = 0
    end
    object ButtonOK: TButton
      Left = 165
      Top = 9
      Width = 114
      Height = 33
      Anchors = [akRight, akBottom]
      Caption = 'OK'
      ModalResult = 1
      TabOrder = 1
    end
  end
end
