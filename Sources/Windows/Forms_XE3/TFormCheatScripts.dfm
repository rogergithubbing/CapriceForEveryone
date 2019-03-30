object FormCheatScripts: TFormCheatScripts
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  Caption = 'Cheat scripts'
  ClientHeight = 337
  ClientWidth = 411
  Color = clBtnFace
  Constraints.MinHeight = 360
  Constraints.MinWidth = 427
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnDestroy = FormDestroy
  OnShow = FormShow
  DesignSize = (
    411
    337)
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl: TPageControl
    Left = 8
    Top = 64
    Width = 272
    Height = 265
    ActivePage = TabSheetInstructions
    Anchors = [akLeft, akTop, akRight, akBottom]
    OwnerDraw = True
    TabHeight = 20
    TabOrder = 0
    OnDrawTab = PageControlDrawTab
    object TabSheetScripts: TTabSheet
      Caption = 'Scripts'
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object ListBoxScripts: TListBox
        Left = 0
        Top = 0
        Width = 264
        Height = 232
        Align = alClient
        BevelInner = bvNone
        BevelOuter = bvNone
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = []
        IntegralHeight = True
        ItemHeight = 19
        ParentFont = False
        TabOrder = 0
        OnClick = ListBoxScriptsClick
        OnDblClick = ListBoxScriptsDblClick
      end
    end
    object TabSheetCode: TTabSheet
      Caption = 'Code'
      ImageIndex = 1
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object MemoCode: TMemo
        Left = 0
        Top = 0
        Width = 264
        Height = 235
        Align = alClient
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Courier New'
        Font.Style = []
        ParentFont = False
        ScrollBars = ssBoth
        TabOrder = 0
        WantReturns = False
        WordWrap = False
      end
    end
    object TabSheetInstructions: TTabSheet
      Caption = 'Instructions'
      ImageIndex = 2
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object MemoInstructions: TMemo
        Left = 0
        Top = 0
        Width = 264
        Height = 235
        Cursor = crArrow
        Align = alClient
        BorderStyle = bsNone
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentColor = True
        ParentFont = False
        ReadOnly = True
        ScrollBars = ssVertical
        TabOrder = 0
      end
    end
  end
  object ButtonExecute: TButton
    Left = 295
    Top = 80
    Width = 105
    Height = 57
    Anchors = [akTop, akRight]
    Caption = 'Execute'
    TabOrder = 1
    OnClick = ButtonExecuteClick
  end
  object ButtonNew: TButton
    Left = 296
    Top = 231
    Width = 105
    Height = 33
    Anchors = [akRight, akBottom]
    Caption = 'New'
    TabOrder = 2
    OnClick = ButtonNewClick
  end
  object GroupBoxSelectedScript: TGroupBox
    Left = 8
    Top = 8
    Width = 393
    Height = 49
    Anchors = [akLeft, akTop, akRight]
    Caption = ' Selected script '
    TabOrder = 3
    DesignSize = (
      393
      49)
    object LabelSelectedScript: TLabel
      Left = 8
      Top = 16
      Width = 377
      Height = 25
      Alignment = taCenter
      Anchors = [akLeft, akTop, akRight]
      AutoSize = False
      Caption = 'New script'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
  end
  object ButtonSave: TButton
    Left = 296
    Top = 263
    Width = 105
    Height = 33
    Anchors = [akRight, akBottom]
    Caption = 'Save'
    TabOrder = 4
    OnClick = ButtonSaveClick
  end
  object ButtonDelete: TButton
    Left = 296
    Top = 295
    Width = 105
    Height = 33
    Anchors = [akRight, akBottom]
    Caption = 'Delete'
    TabOrder = 5
    OnClick = ButtonDeleteClick
  end
  object ButtonStop: TButton
    Left = 295
    Top = 144
    Width = 105
    Height = 33
    Anchors = [akTop, akRight]
    Caption = 'Stop'
    TabOrder = 6
    OnClick = ButtonStopClick
  end
  object SaveDialog: TSaveDialog
    Options = [ofHideReadOnly, ofNoChangeDir, ofPathMustExist, ofEnableSizing]
    Left = 368
    Top = 168
  end
end
