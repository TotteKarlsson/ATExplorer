object ATESettingsForm: TATESettingsForm
  Left = 0
  Top = 0
  BorderStyle = bsSizeToolWin
  Caption = 'AT Explorer Settings'
  ClientHeight = 397
  ClientWidth = 667
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnClose = FormClose
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object TreeView1: TTreeView
    Left = 0
    Top = 0
    Width = 177
    Height = 356
    Align = alLeft
    AutoExpand = True
    HotTrack = True
    Indent = 19
    ReadOnly = True
    RowSelect = True
    TabOrder = 0
    OnChange = TreeView1Change
  end
  object Panel1: TPanel
    Left = 0
    Top = 356
    Width = 667
    Height = 41
    Align = alBottom
    TabOrder = 1
    DesignSize = (
      667
      41)
    object Button1: TButton
      Left = 495
      Top = 8
      Width = 75
      Height = 25
      Anchors = [akTop, akRight]
      Caption = 'Ok'
      ModalResult = 1
      TabOrder = 0
    end
    object Button2: TButton
      Left = 576
      Top = 8
      Width = 75
      Height = 25
      Anchors = [akTop, akRight]
      Caption = 'Cancel'
      ModalResult = 2
      TabOrder = 1
    end
  end
end
