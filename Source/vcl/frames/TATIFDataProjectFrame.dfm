object ATIFDataProjectFrame: TATIFDataProjectFrame
  Left = 0
  Top = 0
  Width = 1140
  Height = 743
  Align = alClient
  TabOrder = 0
  ExplicitWidth = 451
  ExplicitHeight = 305
  object GroupBox1: TGroupBox
    Left = 0
    Top = 0
    Width = 1140
    Height = 353
    Align = alTop
    Caption = 'Raw AT IF Data Properties'
    TabOrder = 0
    ExplicitWidth = 451
    DesignSize = (
      1140
      353)
    object DataRootFolderE: TSTDStringLabeledEdit
      Left = 16
      Top = 42
      Width = 4704
      Height = 21
      Anchors = [akLeft, akTop, akRight]
      EditLabel.Width = 82
      EditLabel.Height = 13
      EditLabel.Caption = 'DataRootFolderE'
      Enabled = False
      TabOrder = 0
      ExplicitWidth = 4015
    end
    object ScanDataBtn: TButton
      Left = 16
      Top = 80
      Width = 81
      Height = 25
      Caption = 'Scan Data'
      TabOrder = 1
      OnClick = ScanDataBtnClick
    end
    object GroupBox2: TGroupBox
      Left = 16
      Top = 120
      Width = 417
      Height = 105
      Caption = 'Quick Info'
      TabOrder = 2
      object Label1: TLabel
        Left = 10
        Top = 22
        Width = 98
        Height = 13
        Caption = 'Number of Ribbons: '
      end
      object Label2: TLabel
        Left = 10
        Top = 48
        Width = 100
        Height = 13
        Caption = 'Number of Sections: '
      end
      object Label3: TLabel
        Left = 10
        Top = 73
        Width = 81
        Height = 13
        Caption = 'Number of Tiles: '
      end
      object Label4: TLabel
        Left = 236
        Top = 22
        Width = 98
        Height = 13
        Caption = 'Number of Sessions:'
      end
      object NrOfRibbonsLbl: TLabel
        Left = 134
        Top = 22
        Width = 6
        Height = 13
        Caption = '0'
      end
      object NrOfSectionsLbl: TLabel
        Left = 134
        Top = 48
        Width = 6
        Height = 13
        Caption = '0'
      end
      object NrOfSessionsLbl: TLabel
        Left = 360
        Top = 22
        Width = 6
        Height = 13
        Caption = '0'
      end
      object NrOfTilesLbl: TLabel
        Left = 134
        Top = 73
        Width = 6
        Height = 13
        Caption = '0'
      end
      object Label5: TLabel
        Left = 236
        Top = 48
        Width = 101
        Height = 13
        Caption = 'Number of Channels:'
      end
      object NrOfChannelsLbl: TLabel
        Left = 360
        Top = 48
        Width = 6
        Height = 13
        Caption = '0'
      end
      object Label6: TLabel
        Left = 236
        Top = 73
        Width = 117
        Height = 13
        Caption = 'Number of State Tables:'
      end
      object StateTablesLbl: TLabel
        Left = 360
        Top = 73
        Width = 23
        Height = 13
        Caption = '0 (0)'
      end
    end
    object PopulatePB: TProgressBar
      Left = 103
      Top = 88
      Width = 4617
      Height = 17
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 3
      ExplicitWidth = 3928
    end
    object GroupBox3: TGroupBox
      Left = 451
      Top = 120
      Width = 462
      Height = 105
      Caption = 'Docker and Render Backends'
      TabOrder = 4
      object Label7: TLabel
        Left = 10
        Top = 29
        Width = 158
        Height = 13
        Caption = 'Render Python Docker Container'
      end
      object Label8: TLabel
        Left = 244
        Top = 29
        Width = 73
        Height = 13
        Caption = 'Render Service'
      end
      object RenderPythonContainersCB: TComboBox
        Left = 10
        Top = 48
        Width = 207
        Height = 21
        TabOrder = 0
        Text = 'RenderPythonContainersCB'
        OnChange = RenderPythonContainersCBChange
      end
      object RenderServicesCB: TComboBox
        Left = 244
        Top = 48
        Width = 215
        Height = 21
        TabOrder = 1
        Text = 'ComboBox1'
      end
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 353
    Width = 1140
    Height = 390
    Align = alClient
    TabOrder = 1
    ExplicitWidth = 451
    ExplicitHeight = 291
    object PageControl1: TPageControl
      Left = 1
      Top = 1
      Width = 1138
      Height = 388
      ActivePage = TabSheet1
      Align = alClient
      TabOrder = 0
      ExplicitWidth = 449
      ExplicitHeight = 289
      object TabSheet1: TTabSheet
        Caption = 'State Tables'
        ExplicitWidth = 441
        ExplicitHeight = 261
        object GroupBox4: TGroupBox
          Left = 0
          Top = 0
          Width = 185
          Height = 360
          Align = alLeft
          Caption = 'Generate State Tables'
          TabOrder = 0
          ExplicitHeight = 261
          DesignSize = (
            185
            360)
          object CreateStateTablesBtn: TButton
            Left = 14
            Top = 317
            Width = 158
            Height = 25
            Anchors = [akLeft, akBottom]
            Caption = 'Generate'
            TabOrder = 0
            OnClick = CreateStateTablesBtnClick
            ExplicitTop = 218
          end
          object StartSectionE: TIntegerLabeledEdit
            Left = 11
            Top = 40
            Width = 62
            Height = 21
            EditLabel.Width = 62
            EditLabel.Height = 13
            EditLabel.Caption = 'Start Section'
            TabOrder = 1
            Text = '1'
            Value = 1
          end
          object EndSectionE: TIntegerLabeledEdit
            Left = 11
            Top = 83
            Width = 62
            Height = 21
            EditLabel.Width = 56
            EditLabel.Height = 13
            EditLabel.Caption = 'End Section'
            TabOrder = 2
            Text = '2'
            Value = 2
          end
        end
        object GroupBox5: TGroupBox
          Left = 185
          Top = 0
          Width = 200
          Height = 360
          Align = alLeft
          Caption = 'Raw Data Render Stacks'
          TabOrder = 1
          ExplicitHeight = 261
          DesignSize = (
            200
            360)
          object CreateRenderStacksBtn: TButton
            Left = 16
            Top = 317
            Width = 169
            Height = 25
            Anchors = [akLeft, akBottom]
            Caption = 'Generate'
            TabOrder = 0
            OnClick = CreateStateTablesBtnClick
            ExplicitTop = 218
          end
          object IntegerLabeledEdit1: TIntegerLabeledEdit
            Left = 9
            Top = 83
            Width = 56
            Height = 21
            EditLabel.Width = 56
            EditLabel.Height = 13
            EditLabel.Caption = 'End Section'
            TabOrder = 1
            Text = '2'
            Value = 2
          end
          object IntegerLabeledEdit2: TIntegerLabeledEdit
            Left = 9
            Top = 40
            Width = 56
            Height = 21
            EditLabel.Width = 62
            EditLabel.Height = 13
            EditLabel.Caption = 'Start Section'
            TabOrder = 2
            Text = '1'
            Value = 1
          end
        end
      end
      object TabSheet2: TTabSheet
        Caption = 'FlatField Corrected'
        ImageIndex = 1
        ExplicitLeft = 0
        ExplicitTop = 0
        ExplicitWidth = 1000
        ExplicitHeight = 394
        object Panel3: TPanel
          Left = 0
          Top = 0
          Width = 946
          Height = 41
          Align = alTop
          TabOrder = 0
          ExplicitWidth = 1000
          object CreateMediansBtn: TButton
            Left = 11
            Top = 10
            Width = 121
            Height = 25
            Caption = 'Create Medians'
            TabOrder = 0
            OnClick = CreateStateTablesBtnClick
          end
          object ApplyMediansBtn: TButton
            Left = 138
            Top = 10
            Width = 128
            Height = 25
            Caption = 'Apply Medians'
            TabOrder = 1
            OnClick = CreateStateTablesBtnClick
          end
        end
      end
    end
  end
end
