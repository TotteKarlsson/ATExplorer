
////This is called from a thread and need to be synchronized with the UI main thread
//void __fastcall TMainForm::onImage()
//{
//	TMemoryStream* imageMem = mRC.getImageMemory();
//    if(!imageMem)
//    {
//    	return;
//    }
//
//    if(!fileExists(mRC.getImageLocalCachePathAndFileName()))
//    {
//        Log(lError) << "File does not exist: " <<mRC.getImageLocalCachePathAndFileName();
//        return;
//    }
//
//    mCurrentImageFile = mRC.getImageLocalCachePathAndFileName().c_str();
//	double val = CustomImageRotationE->getValue();
//    if(val != 0)
//    {
//		paintRotatedImage(val);
//    }
//    else
//    {
//        try
//        {
//            //Create a temporary stream
//            unique_ptr<TMemoryStream> stream = unique_ptr<TMemoryStream>(new TMemoryStream());
//            stream->LoadFromFile(mCurrentImageFile.c_str());
//            if(stream->Size)
//            {
//            	stream->Position = 0;
//                Image1->Picture->LoadFromStream(stream.get());
//            }
//        }
//        catch(...)
//        {
//            Log(lError) << "Failed to load image: "<<mCurrentImageFile;
//            return;
//        }
//    }
//
//    if(gImageForm && gImageForm->Visible)
//    {
//    	gImageForm->load(mCurrentImageFile);
//    }
//
//    Image1->Refresh();
//
//    Log(lDebug3) << "WxH = " <<Image1->Picture->Width << "x" << Image1->Picture->Height;
//    this->Image1->Cursor = crDefault;
//}
//
////---------------------------------------------------------------------------
//void __fastcall TMainForm::ClickZ(TObject *Sender)
//{
//    if(mZs->ItemIndex == -1)
//    {
//    	return;
//    }
//
//    int z = toInt(stdstr(mZs->Items->Strings[mZs->ItemIndex]));
//
//    //Fetch data using URL
//	mRC.init(gAU.CurrentOwner.getValue(), gAU.CurrentProject.getValue(), gAU.CurrentStack.getValue(), "jpeg-image", z, mCurrentROI, mScaleE->getValue(), MinIntensityE->getValue(), MaxIntensityE->getValue());
//
//    if(VisualsPC->Pages[VisualsPC->TabIndex] == TabSheet2)
//    {
//		this->Image1->Cursor = crHourGlass;
//        StringList paras;
//        paras.append(string("&maxTileSpecsToRender=") + stdstr(maxTileSpecsToRenderE->Text));
//
//    	//Image pops up in onImage callback
//	    mRC.getImageInThread(z, paras);
//    }
//    else
//    {
//		OpenInNDVIZBtnClick(NULL);
//    }
//
//    URLE->setValue(createNDVIZURL());
//
//}
//
////---------------------------------------------------------------------------
//void __fastcall TMainForm::mScaleEKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
//{
//	if(Key == VK_RETURN)
//    {
//        mCurrentROI = RegionOfInterest(XCoordE->getValue(), YCoordE->getValue(), Width->getValue(), Height->getValue(), mScaleE->getValue());
//		ClickZ(Sender);
//    }
//}
//
////---------------------------------------------------------------------------
//double TMainForm::getImageStretchFactor()
//{
//	if((mScaleE->getValue() * Height->getValue() * Width->getValue()) == 0)
//    {
//    	Log(lError) << "Tried to divide by zero!";
//    	return 1;
//    }
//
//    if(Image1->Height < Image1->Width)
//    {
//    	return Image1->Height / (mScaleE->getValue() * Height->getValue());
//    }
//    else
//    {
//		return Image1->Width / (mScaleE->getValue() * Width->getValue());
//    }
//}
//
////---------------------------------------------------------------------------
//TCanvas* TMainForm::getCanvas()
//{
//	return PaintBox1->Canvas;
//}
//
////---------------------------------------------------------------------------
//void __fastcall TMainForm::FormMouseDown(TObject *Sender, TMouseButton Button,
//          TShiftState Shift, int X, int Y)
//{
//	if(mRenderEnabled == false)
//    {
//    	return;
//    }
//
//	if(Button == TMouseButton::mbRight)
//    {
//        return;
//    }
//
//	double stretchFactor = getImageStretchFactor();
//	if(Button == TMouseButton::mbMiddle)
//    {
//       	Screen->Cursor = crSize;
//		mTopLeftSelCorner = Mouse->CursorPos;
//		mTopLeftSelCorner = this->Image1->ScreenToClient(mTopLeftSelCorner);
//
//		//Convert to world image coords (minus offset)
//	    mTopLeftSelCorner = controlToImage(mTopLeftSelCorner, mScaleE->getValue(), stretchFactor);
//        return;
//    }
//
//    Drawing = true;
//    getCanvas()->MoveTo(X , Y);
//    Origin = Point(X, Y);
//    MovePt = Origin;
//
//    //For selection
//	mTopLeftSelCorner = Mouse->CursorPos;
//	mTopLeftSelCorner = this->Image1->ScreenToClient(mTopLeftSelCorner);
//
//	//Convert to world image coords (minus offset)
//    mTopLeftSelCorner = controlToImage(mTopLeftSelCorner, mScaleE->getValue(), stretchFactor);
//}
//
////---------------------------------------------------------------------------
//void __fastcall TMainForm::Image1MouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
//{
//	TPoint p = this->Image1->ScreenToClient(Mouse->CursorPos);
//
//    double stretchF = getImageStretchFactor();
//	XE->Caption = IntToStr((int) (p.X * stretchF) + XCoordE->getValue()) ;
//	YE->Caption = IntToStr((int) (p.Y * stretchF) + YCoordE->getValue());
////    ImageWidthL->Caption = IntToStr((int) Image1->Width);
////    ImageHeightL->Caption = IntToStr((int) Image1->Height);
//
//	//Convert to world image coords (minus offset)
//    double stretchFactor = getImageStretchFactor();
//    if(stretchFactor)
//    {
//	    p = controlToImage(p, mScaleE->getValue(), stretchFactor);
////	  	mX->setValue(p.X);
////		mY->setValue(p.Y);
//    }
//
//	if(GetAsyncKeyState(VK_MBUTTON) < 0)
//    {
//    	//Move the picture
////        Image1->Top = Image1->Top + 1;
//    }
//}
//
////---------------------------------------------------------------------------
//void __fastcall TMainForm::FormMouseUp(TObject *Sender, TMouseButton Button,
//          TShiftState Shift, int X, int Y)
//{
//    double stretchFactor = getImageStretchFactor();
//	if(Button == TMouseButton::mbMiddle)
//    {
//	  	Screen->Cursor = crDefault;
//    	TPoint p2;
//		p2 = Mouse->CursorPos;
//		p2 = this->Image1->ScreenToClient(p2);
//	    p2 = controlToImage(p2, mScaleE->getValue(), stretchFactor);
//
//		//Convert to world image coords (minus offset)
//		XCoordE->setValue(XCoordE->getValue() + (mTopLeftSelCorner.X - p2.X));
//		YCoordE->setValue(YCoordE->getValue() + (mTopLeftSelCorner.Y - p2.Y));
//
//		mCurrentROI = RegionOfInterest(XCoordE->getValue(), YCoordE->getValue(), Width->getValue(), Height->getValue(), mScaleE->getValue());
//       	ClickZ(Sender);
//    }
//
//	if(!Drawing || (Button == TMouseButton::mbRight))
//    {
//    	return;
//    }
//
//	Drawing = false;
//
//    //For selection
//	mBottomRightSelCorner = this->Image1->ScreenToClient(Mouse->CursorPos);
//
//	//Convert to world image coords (minus offset)
//    mBottomRightSelCorner = controlToImage(mBottomRightSelCorner, mScaleE->getValue(), stretchFactor);
//
//	//Check if selection indicate a 'reset'
//	if(mBottomRightSelCorner.X - mTopLeftSelCorner.X <= 0 || mBottomRightSelCorner.Y - mTopLeftSelCorner.Y <= 0)
//    {
//    	resetButtonClick(NULL);
//		return;
//    }
//
//	mCurrentROI = RegionOfInterest(	XCoordE->getValue() + mTopLeftSelCorner.X,
//    				 				YCoordE->getValue() + mTopLeftSelCorner.Y,
//                                    mBottomRightSelCorner.X - mTopLeftSelCorner.X,
//                                    mBottomRightSelCorner.Y - mTopLeftSelCorner.Y,
//                                    mScaleE->getValue());
////	XCoordE->setValue(XCoordE->getValue() + mTopLeftSelCorner.X);
////	YCoordE->setValue(YCoordE->getValue() + mTopLeftSelCorner.Y);
////    Width->setValue(mBottomRightSelCorner.X - mTopLeftSelCorner.X);
////    Height->setValue(mBottomRightSelCorner.Y - mTopLeftSelCorner.Y);
//
////	mCurrentROI = RegionOfInterest(XCoordE->getValue(), YCoordE->getValue(), Width->getValue(), Height->getValue());
//    updateScale();
//	roiChanged();
//
//    updateROIs();
//	ClickZ(NULL);
//    checkCache();
//}
//
////---------------------------------------------------------------------------
//void __fastcall TMainForm::FormMouseMove(TObject *Sender, TShiftState Shift,
//          int X, int Y)
//{
//	if(Drawing)
//  	{
//		DrawShape(Origin, MovePt, pmNotXor);
//		MovePt = Point(X, Y);
//		DrawShape(Origin, MovePt, pmNotXor);
//  	}
//
//	Image1MouseMove(Sender, Shift, X, Y);
//}
//
////---------------------------------------------------------------------------
//void __fastcall TMainForm::DrawShape(TPoint TopLeft, TPoint BottomRight, TPenMode AMode)
//{
//  	getCanvas()->Pen->Mode = AMode;
//	getCanvas()->Rectangle(TopLeft.x, TopLeft.y, BottomRight.x, BottomRight.y);
//}
//
//int	TMainForm::getCurrentZ()
//{
//	int ii = mZs->ItemIndex;
//    if(ii == -1)
//    {
//    	return -1;
//    }
//
//    return toInt(stdstr(mZs->Items->Strings[ii]));
//}
//
//void __fastcall TMainForm::resetButtonClick(TObject *Sender)
//{
//	try
//    {
//	    //mScaleE->setValue(0.05 * ScaleConstantE->getValue());
//        mCurrentROI = mRC.getLayerBoundsForZ(getCurrentZ());
//
//        XCoordE->setValue(mCurrentROI.getX1());
//        YCoordE->setValue(mCurrentROI.getY1());
//        Width->setValue(mCurrentROI.getWidth());
//        Height->setValue(mCurrentROI.getHeight());
//	    updateScale();
//
//        ClickZ(NULL);
//        Log(lDebug1) << "Origin: (X0,Y0) = (" << XCoordE->getValue() + Width->getValue()/2.<<"," <<YCoordE->getValue() + Height->getValue()/2.<<")";
//	    checkCache();
//        updateROIs();
//    }
//    catch(...)
//    {}
//}
//
////---------------------------------------------------------------------------
//void __fastcall TMainForm::FetchSelectedZsBtnClick(TObject *Sender)
//{
//    TButton* b = dynamic_cast<TButton*>(Sender);
//
//    if(b == FetchSelectedZsBtn)
//    {
//        if(mCreateCacheThread.isRunning())
//        {
//            mCreateCacheThread.stop();
//        }
//        else
//        {
//            int z = toInt(stdstr(mZs->Items->Strings[0]));
//            RenderClient rs(IdHTTP1, RenderServiceParameters(BaseURLE->getValue(), RenderPort->getValue()), gAU.LocalCacheFolder.getValue());
//            rs.init(gAU.CurrentOwner.getValue(), gAU.CurrentProject.getValue(),
//                gAU.CurrentStack.getValue(), "jpeg-image", z, mCurrentROI, mScaleE->getValue(), MinIntensityE->getValue(), MaxIntensityE->getValue());
//
//            //Create image URLs
//            StringList urls;
//            for(int i = 0; i < mZs->Count; i++)
//            {
//                int	z = toInt(stdstr(mZs->Items->Strings[i]));
//                urls.append(rs.getURLForZ(z));
//            }
//
//    	    StringList paras;
//	        paras.append(string("&maxTileSpecsToRender=") + stdstr(maxTileSpecsToRenderE->Text));
//            mCreateCacheThread.setup(urls, gAU.LocalCacheFolder.getValue());
//            mCreateCacheThread.addParameters(paras);
//            mCreateCacheThread.start();
//            CreateCacheTimer->Enabled = true;
//        }
//    }
//    else if(b == ClearCacheBtn)
//    {
//        //Clear cache for the current owner/project/stack
//        Path p(gAU.LocalCacheFolder.getValue());
//
//        p.append(joinPath(gAU.CurrentOwner.getValue(), gAU.CurrentProject.getValue()));
//        p.append(gAU.CurrentStack.getValue());
//
//        Log(lInfo) << "Deleting local cache for stack: " << p.toString();
//        boost::filesystem::remove_all(p.toString());
//        checkCache();
//    }
//}
//
////---------------------------------------------------------------------------
//void __fastcall TMainForm::GetValidZsBtnClick(TObject *Sender)
//{
//	//Fetch valid zs for current project
//   	RenderClient rs(IdHTTP1, BaseURLE->getValue());
//    rs.init(gAU.CurrentOwner.getValue(), gAU.CurrentProject.getValue(),	gAU.CurrentStack.getValue());
//    StringList zs = rs.getValidZs();
//
//	Log(lInfo) << "Fetched "<<zs.count()<<" valid z's";
//
//	Zs_GB->Caption = " Z Values (" + IntToStr((int) zs.count()) + ") ";
//
//    //Populate list box
//	populateCheckListBox(zs, mZs);
//    mZs->CheckAll(cbChecked);
//
//}
//
////---------------------------------------------------------------------------
//void __fastcall TMainForm::mUpdateZsBtnClick(TObject *Sender)
//{
//	//Fetch valid zs for current project
//   	RenderClient rs(IdHTTP1, BaseURLE->getValue());
//    rs.init(gAU.CurrentOwner.getValue(), gAU.CurrentProject.getValue(), gAU.CurrentStack.getValue());
//    StringList zs = rs.getZs();
//
//    if(zs.size() > 1)
//    {
//    	//Populate list boxes
//	    Log(lInfo) << "Valid Z's: "<<zs[0];
//    	Log(lInfo) << "Missing Z's: "<<zs[1];
//    }
//}
//
////---------------------------------------------------------------------------
//void __fastcall TMainForm::GetOptimalBoundsBtnClick(TObject *Sender)
//{
//	RenderClient rs(IdHTTP1, BaseURLE->getValue());
//    rs.init(gAU.CurrentOwner.getValue(), gAU.CurrentProject.getValue(),	gAU.CurrentStack.getValue());
//
//    vector<int> zs = rs.getValidZs();
//    RegionOfInterest box = rs.getOptimalXYBoxForZs(zs);
//    Log(lInfo) << "XMin = " << box.getX1();
//    Log(lInfo) << "XMax = " << box.getX2();
//    Log(lInfo) << "YMin = " << box.getY1();
//    Log(lInfo) << "YMax = " << box.getY2();
//
//   	vector<RegionOfInterest> bounds = rs.getLayerBounds();
//    for(int i = 0; i < bounds.size(); i++)
//    {
//	    Log(lDebug3) <<"Bounds:" << bounds[i].getZ()<<","<<bounds[i].getX1()<<","<<bounds[i].getX2()<<","<<bounds[i].getY1()<<","<<bounds[i].getY2();
//    }
//}
//
////---------------------------------------------------------------------------
//void __fastcall TMainForm::mZoomBtnClick(TObject *Sender)
//{
//	TButton* b = dynamic_cast<TButton*>(Sender);
//
//	double zoomFactor = mZoomFactor->getValue();
//    if(b == mZoomOutBtn)
//    {
//		zoomFactor *= (-1.0);
//    }
//
//	//Modify bounding box with x%
//    mCurrentROI = RegionOfInterest(XCoordE->getValue(), YCoordE->getValue(), Width->getValue(), Height->getValue());
//    mCurrentROI.zoom(zoomFactor);
//
//	XCoordE->setValue(mCurrentROI.getX1());
//    YCoordE->setValue(mCurrentROI.getY1());
//    Width->setValue( mCurrentROI.getWidth());
//    Height->setValue(mCurrentROI.getHeight());
//
//    updateScale();
//	ClickZ(Sender);
//    checkCache();
//}
//
////---------------------------------------------------------------------------
//void TMainForm::updateScale()
//{
////    mCurrentROI = RegionOfInterest(XCoordE->getValue(), YCoordE->getValue(), Width->getValue(), Height->getValue());
//
//    //Scale the scaling
//    double scale  = (double) Image1->Height / (double) mCurrentROI.getHeight();
//    Log(lDebug5) << "Image Scale: " << scale;
//	if(scale < 0.005)
//    {
//    	scale = 0.009;
//    }
//    else if(scale > 1.0)
//    {
//    	scale = 1.0;
//    }
//	mScaleE->setValue(scale);
//    mCurrentROI.setScale(scale);
//}
//
////--------------------------------------------------------------------------
//void __fastcall TMainForm::OwnerCBChange(TObject *Sender)
//{
//	//Update Projects CB
//    //Get selected owner
//    if(OwnerCB->ItemIndex == -1)
//    {
//		return;
//    }
//
//    //Disable uninitialized sections of the UI
//	enableDisableGroupBox(imageParasGB, false);
//	enableDisableGroupBox(Zs_GB, false);
//    enableDisableGroupBox(StackGenerationGB, false);
//   	mRenderEnabled = false;
//	StackCB->Clear();
//
//    string owner = stdstr(OwnerCB->Items->Strings[OwnerCB->ItemIndex]);
//    gAU.CurrentOwner.setValue(owner);
//
//    //Populate projects
//    StringList p = mRC.getProjectsForOwner(gAU.CurrentOwner.getValue());
//    if(p.size())
//    {
//		populateDropDown(p, ProjectCB);
//    }
//}
//
////---------------------------------------------------------------------------
//void __fastcall TMainForm::ProjectCBChange(TObject *Sender)
//{
//	//Update Stacks CB
//    //Get selected owner
//    if(ProjectCB->ItemIndex == -1)
//    {
//		return;
//    }
//
//    //Disable uninitialized sections of the UI
//	enableDisableGroupBox(imageParasGB, false);
//	enableDisableGroupBox(Zs_GB, false);
//    enableDisableGroupBox(StackGenerationGB, false);
//	mRenderEnabled = false;
//    string owner = stdstr(OwnerCB->Items->Strings[OwnerCB->ItemIndex]);
//    string project = stdstr(ProjectCB->Items->Strings[ProjectCB->ItemIndex]);
//    gAU.CurrentProject.setValue(project);
//
//    //Populate stacks
//	updateStacksForCurrentProject();
//}
//
//void TMainForm::updateStacksForCurrentProject()
//{
//    gAU.CurrentOwner = stdstr(OwnerCB->Items->Strings[OwnerCB->ItemIndex]);
//
//    //Populate stacks
//    StringList stacks = mRC.getStacksForProject(gAU.CurrentOwner, gAU.CurrentProject);
//    if(stacks.size())
//    {
//		StackCB->ItemIndex = populateDropDown(stacks, 		StackCB);
//		populateCheckListBox(stacks, 	StacksForProjectCB);
//    }
//	StackCBChange(NULL);
//}
//
//void __fastcall TMainForm::StackCBChange(TObject *Sender)
//{
//    if(StackCB->ItemIndex == -1)
//    {
//		return;
//    }
//
//    string stack = stdstr(StackCB->Items->Strings[StackCB->ItemIndex]);
//	gAU.CurrentStack.setValue(stack);
//	mRC.getProject().init(gAU.CurrentOwner.getValue(), gAU.CurrentProject.getValue(), gAU.CurrentStack.getValue());
//
//   	GetValidZsBtnClick(NULL);
//
//	ClickZ(NULL);
//    updateROIs();
//    //Update stack generation page
//	//User changed stack.. Clear check list box and select current one
//    for(int i = 0; i < StacksForProjectCB->Items->Count; i++)
//    {
//    	StacksForProjectCB->Checked[i] = (StacksForProjectCB->Items->Strings[i] == StackCB->Text) ? true : false;
//    }
//
//	StackCB->Hint = vclstr(stack);
//
//    //Disable uninitialized sections of the UI
//	enableDisableGroupBox(imageParasGB, true);
//	enableDisableGroupBox(Zs_GB, true);
//	mRenderEnabled = true;
//}
//
////---------------------------------------------------------------------------
//void __fastcall TMainForm::CreateCacheTimerTimer(TObject *Sender)
//{
//	if(mCreateCacheThread.isRunning())
//    {
//		FetchSelectedZsBtn->Caption = "Stop";
//    }
//    else
//    {
//		CreateCacheTimer->Enabled = false;
//		FetchSelectedZsBtn->Caption = "Generate";
//    }
//}
//
////---------------------------------------------------------------------------
//void __fastcall TMainForm::IntensityKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
//{
//	if(Key != VK_RETURN)
//    {
//    	return;
//    }
//    int minInt = MinIntensityE->getValue();
//    int maxInt = MaxIntensityE->getValue();
//    if(mZs->ItemIndex == -1)
//    {
//        return;
//    }
//
//    int z = toInt(stdstr(mZs->Items->Strings[mZs->ItemIndex]));
//    ClickZ(NULL);
//}
//
////---------------------------------------------------------------------------
//void __fastcall TMainForm::CheckAll1Click(TObject *Sender)
//{
//	TCheckListBox* lb = dynamic_cast<TCheckListBox*>(ZsPopUpMenu->PopupComponent);
//
//    if(!lb)
//    {
//    	return;
//    }
//
//    for(int i = 0; i < lb->Count; i++)
//    {
//    	lb->Checked[i] = true;
//    }
//}
//
////---------------------------------------------------------------------------
//void __fastcall TMainForm::UncheckAll1Click(TObject *Sender)
//{
//	TCheckListBox* lb = dynamic_cast<TCheckListBox*>(ZsPopUpMenu->PopupComponent);
//
//    if(!lb)
//    {
//    	return;
//    }
//
//    for(int i = 0; i < lb->Count; i++)
//    {
//    	lb->Checked[i] = false;
//    }
//}
//
////---------------------------------------------------------------------------
//void __fastcall TMainForm::Checkrange1Click(TObject *Sender)
//{
//	TSelectZsForm* sz = new TSelectZsForm(this);
//    sz->populate(mZs);
//    int mr = sz->ShowModal();
//
//    if(mr == mrOk)
//    {
//    	//Check/Uncheck items
//    	//First uncheck all
//        for(int i = 0; i < mZs->Count; i++)
//	    {
//    		mZs->Checked[i] = false;
//	    }
//
//		StringList us(sz->getSelected());
//
//        //Then select the selected ones
//        for(int i = 0; i < mZs->Count; i++)
//        {
//        	string val(stdstr(mZs->Items->Strings[i]));
//        	if(us.contains(val))
//            {
//				mZs->Checked[i] = true;
//            }
//        }
//    }
//    delete sz;
//}
//
////---------------------------------------------------------------------------
//void __fastcall TMainForm::StackFilterCBClick(TObject *Sender)
//{
//	StacksForProjectCB->Clear();
//    StringList s = mRC.getStacksForProject(gAU.CurrentOwner, gAU.CurrentProject);
//
//	if(CustomFilterCB->Checked)
//    {
//        for(int i = 0; i < s.count(); i++)
//        {
//            if(contains(stdstr(CustomFilterE->Text), s[i]))
//            {
//                StacksForProjectCB->AddItem(s[i].c_str(), NULL);
//            }
//        }
//    }
//    else
//    {
//        for(int i = 0; i < s.count(); i++)
//        {
//        	StacksForProjectCB->AddItem(s[i].c_str(), NULL);
//        }
//    }
//}

////---------------------------------------------------------------------------
//string createProcessedImageFileName(const string& pic)
//{
//	string fName(getFileNameNoExtension(pic));
//	string fPath(getFilePath(pic));
//    string fExt(getFileExtension(pic));
//    string newName(joinPath(fPath, fName + "_processed") + "." + fExt);
//    return newName;
//}
//
////---------------------------------------------------------------------------
//void __fastcall TMainForm::ClickImageProcCB(TObject *Sender)
//{
//	ClickZ(NULL);
//}
//
////---------------------------------------------------------------------------
//void __fastcall TMainForm::CustomFilterEKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
//{
//	if(Key != VK_RETURN)
//    {
//    	return;
//    }
//
//    StacksForProjectCB->Clear();
//    StringList s = mRC.getStacksForProject(gAU.CurrentOwner, gAU.CurrentProject);
//    if(CustomFilterCB->Checked)
//    {
//        for(int i = 0; i < s.count(); i++)
//        {
//            if(contains(stdstr(CustomFilterE->Text), s[i]))
//            {
//                StacksForProjectCB->AddItem(s[i].c_str(), NULL);
//            }
//        }
//    }
//}
//
////---------------------------------------------------------------------------
//void __fastcall TMainForm::OpenaClone1Click(TObject *Sender)
//{
//	if(!gImageForm)
//    {
//    	gImageForm = new TImageForm(gAU.AppRegistryRoot, "", this);
//    }
//
//	gImageForm->Show();
//}
//
////---------------------------------------------------------------------------
//void __fastcall TMainForm::CreateTIFFStackCBClick(TObject *Sender)
//{
//	DeleteTempTiffsCB->Enabled = CreateTIFFStackCB->Checked;
//}

////---------------------------------------------------------------------------
//void __fastcall TMainForm::CustomRotationEKeyDown(TObject *Sender, WORD &Key,
//          TShiftState Shift)
//{
//	if(Key == VK_RETURN)
//    {
//		//Do a rotation
//		ClickZ(NULL);
//    }
//}
//
//void __fastcall TMainForm::TestRenderServiceBtnClick(TObject *Sender)
//{
//    mRC.setBaseURL(BaseURLE->getValue());
//    mRC.getProject().init(gAU.CurrentOwner.getValue(), gAU.CurrentProject.getValue(), gAU.CurrentStack.getValue());
//
//    //Populate owners
//    StringList o = mRC.getOwners();
//    if(o.size())
//    {
//		populateDropDown(o, OwnerCB);
//    }
//}

//void __fastcall TMainForm::DcefBrowser1BeforeBrowse(ICefBrowser * const browser,
//          ICefFrame * const frame, ICefRequest * const request,
//          bool isRedirect, bool &Cancel)
//{
//    Log(lDebug) << "browsing..";
//}
////---------------------------------------------------------------------------
//void __fastcall TMainForm::DcefBrowser1ConsoleMessage(ICefBrowser * const browser,
//          const ustring message, const ustring source, int line,
//          bool &Cancel)
//{
//    Log(lDebug) <<"Console Message";
//}
////---------------------------------------------------------------------------
//
//void __fastcall TMainForm::DcefBrowser1StateChange(ICefBrowser * const browser,
//          const TBrowserDataChangeKind Kind, const UnicodeString Value)
//
//{
////    Log(lDebug) <<"State Change: " << stdstr(DcefBrowser1->URL);
////    URLE->setValue(stdstr(DcefBrowser1->URL));
////    parseURLUpdate(stdstr(DcefBrowser1->URL));
//}

//bool TMainForm::parseURLUpdate(const string& url)
//{
////     http://ibs-forrestc-ux1.corp.alleninstitute.org:8001/#!
////    {
////    	layers:
////    	{
////        	Rough_Aligned_1_DAPI_1:
////        	{
////            	type:image_source:render://http://ibs-forrestc-ux1.corp.alleninstitute.org/6_ribbon_experiments/M321160_Ai139_smallvol/Rough_Aligned_1_DAPI_1_max:0.30518
////          }
////      }
////    	_navigation:
////        {
////        	  pose:
////            {
////            	   position:
////                {
////                	voxelSize:[1_1_1]_voxelCoordinates:[7009.5009765625_52.7322998046875_6]
////                }
////            }
////            _zoomFactor:16.487212707001284
////        }
////    }
//    string data(stripCharacter('\'', url));
//    StringList strings(data, '{');
//
//    for(int i = 0; i < strings.count(); i++)
//    {
//	    Log(lDebug3) << "String " << i<<"\t"<<strings[i];
//        //Get string starting with voxelSize
//        if(startsWith("voxelSize", strings[i]))
//        {
//            //Extract records
//            StringList getZ(strings[i], ']');
//            if(getZ.count() >= 2)
//            {
//                StringList voxelCoords(getZ[1], ':');
//
//                if(voxelCoords.count() ==2)
//                {
//                	StringList z(voxelCoords[1], '_');
//                    if(z.count() >= 3)
//                    {
//                        Log(lDebug3) << "Z is "<< z[2];
//
//                        if(toInt(z[2]) > -1)
//                        {
//                        	mZs->ItemIndex = toInt(z[2]);
//                        }
//                    }
//                }
//            }
//        }
//    }
//    return true;
//}
//
//
////---------------------------------------------------------------------------
//void __fastcall TMainForm::ClearBrowserCacheBtnClick(TObject *Sender)
//{
////    DcefBrowser1->ReloadIgnoreCache();
//}
//
////---------------------------------------------------------------------------
//void __fastcall TMainForm::RenderTSEnter(TObject *Sender)
//{
//    Log(lInfo) << "Entering.";
//}
//
////---------------------------------------------------------------------------
//void __fastcall TMainForm::ScriptsPCChange(TObject *Sender)
//{
////    if(ScriptsPC->Pages[ScriptsPC->ActivePageIndex] == RenderTS)
////    {
////        //Populate..
////        TRenderPythonRemoteScriptFrame1->populate(mRC);
////    }
//}
//
////--------------------------------------------------------------------------
//void __fastcall TMainForm::PageControl1Change(TObject *Sender)
//{
//    if(ControlsPC->Pages[ControlsPC->ActivePageIndex] == TransformsTab)
//    {
//        //Populate..
//        TAffineTransformationFrame1->populate(mRC, TSSHFrame1->ScSSHShell1);
//        TAffineTransformationFrame1->RotationE->setValue(CustomImageRotationE->getValue());
//		TAffineTransformationFrame1->TranslateXE->setValue((-1) * (XCoordE->getValue() + Width->getValue()/2.));
//		TAffineTransformationFrame1->TranslateYE->setValue((-1) * (YCoordE->getValue() + Height->getValue()/2.));
//    }
//}

////---------------------------------------------------------------------------
//void __fastcall TMainForm::FormMouseWheel(TObject *Sender, TShiftState Shift,
//          int WheelDelta, TPoint &MousePos, bool &Handled)
//{
//    //Don' scroll z's if mouse is over Logmemo
//	TPoint MyPoint = BottomPanel->ScreenToClient(Mouse->CursorPos);
//	if(PtInRect(BottomPanel->ClientRect, MyPoint))
//    {
//        return;
//    }
//    int zIndex(mZs->ItemIndex);
//
//    if(WheelDelta > 0)
//    {
//		++zIndex;
//    }
//    else
//    {
//		--zIndex;
//    }
//
//    if(zIndex > -1 && zIndex < mZs->Count)
//    {
//		mZs->ItemIndex = zIndex;
//        ClickZ(NULL);
//    }
//    Log(lDebug5) << "WheelDelta: " << WheelDelta;
//    Handled = true;
//}
//
////---------------------------------------------------------------------------
//void __fastcall TMainForm::OpenInChromeBtnClick(TObject *Sender)
//{
//	ShellExecuteA(0,0, "chrome.exe", URLE->getValue().c_str(), 0, SW_SHOWMAXIMIZED);
//}
//
////---------------------------------------------------------------------------
//void __fastcall TMainForm::TSSHFrame1ConnectBtnClick(TObject *Sender)
//{
//	TSSHFrame1->ConnectBtnClick(Sender);
//}
//
////---------------------------------------------------------------------------
//void __fastcall TMainForm::VisualsPCChange(TObject *Sender)
//{
//    if(VisualsPC->Pages[VisualsPC->ActivePageIndex] == NdVizTS)
//    {
//        //Populate..
//        OpenInNDVIZBtnClick(NULL);
//    }
//    else
//    {
//	    ClickZ(Sender);
//    }
//}
//
////---------------------------------------------------------------------------
//void __fastcall TMainForm::PaintBox1Paint(TObject *Sender)
//{
//    if(ShutDownTimer->Enabled)
//    {
//        return;
//    }
//
//    if(ShowImageGridCB->Checked)
//    {
//        mImageGrid.paint();
//    }
//}
//
////---------------------------------------------------------------------------
//void __fastcall TMainForm::TAffineTransformationFrame1RotationEKeyDown(TObject *Sender,
//          WORD &Key, TShiftState Shift)
//{
//    if(Key == VK_RETURN)
//    {
//	    double val = TAffineTransformationFrame1->RotationE->getValue();
//	    paintRotatedImage(val);
//    }
//}
//
////---------------------------------------------------------------------------
//void TMainForm::paintRotatedImage(double angle)
//{
//    Image1->Align = alNone;
//    Image1->Picture = NULL;
//    TCanvas* c = Image1->Canvas;
//
//    wstring fName(mCurrentImageFile.begin(), mCurrentImageFile.end());
//    Gdiplus::Image image(fName.c_str());
//
//    //Get native image dimensions
//    Image1->Height = image.GetHeight();
//	Image1->Width = image.GetWidth();
//
//    Gdiplus::Graphics graphics(c->Handle);
//
//    Gdiplus::PointF center(Image1->Width/2, Image1->Height/2);
//    Gdiplus::Matrix matrix;
//
//    matrix.RotateAt(angle, center);
//    graphics.SetTransform(&matrix);
//
//    c->Brush->Color = clBlack;
//	c->Rectangle(0,0, PaintBox1->Width, PaintBox1->Height);
//
//    //draw rotated image
//    graphics.DrawImage(&image,0, 0, Image1->Width, Image1->Height);
//
//    Image1->Align = alClient;
//    Image1->Invalidate();
//}
//
////---------------------------------------------------------------------------
////void TMainForm::setBrightness(int b)
////{
////    Image1->Align = alNone;
////    Image1->Picture = NULL;
////    TCanvas* c = Image1->Canvas;
////
////    wstring fName(mCurrentImageFile.begin(), mCurrentImageFile.end());
////    Gdiplus::Image image(fName.c_str());
////
////    //Get native image dimensions
////    Image1->Height = image.GetHeight();
////	Image1->Width = image.GetWidth();
////
////    Gdiplus::Graphics graphics(c->Handle);
////
////    Gdiplus::PointF center(Image1->Width/2, Image1->Height/2);
////    Gdiplus::Matrix matrix;
////
////    matrix.RotateAt(angle, center);
////    graphics.SetTransform(&matrix);
////
////    c->Brush->Color = clBlack;
////	c->Rectangle(0,0, PaintBox1->Width, PaintBox1->Height);
////
////    //draw rotated image
////    graphics.DrawImage(&image,0, 0, Image1->Width, Image1->Height);
////
////    Image1->Align = alClient;
////    Image1->Invalidate();
////}
////
////---------------------------------------------------------------------------
//void __fastcall TMainForm::ToggleImageGridAExecute(TObject *Sender)
//{
//    Log(lDebug5) << "Action Component: " << stdstr(ToggleImageGridA->ActionComponent->Name);
//
//    TMenuItem* ac = dynamic_cast<TMenuItem*>(ToggleImageGridA->ActionComponent);
//    if(ac)
//    {
//        ShowImageGridCB->Checked = !ShowImageGridCB->Checked;
//    }
//    else
//    {
//   		PaintBox1Paint(NULL);
//    }
//}
//
////---------------------------------------------------------------------------
//void __fastcall TMainForm::CustomImageRotationEKeyDown(TObject *Sender, WORD &Key,
//          TShiftState Shift)
//{
//    if(Key == VK_RETURN)
//    {
//	    double val = CustomImageRotationE->getValue();
//	    paintRotatedImage(val);
//    }
//}

////---------------------------------------------------------------------------
//void __fastcall TMainForm::ToggleImageGridAUpdate(TObject *Sender)
//{
//    if(!Drawing)
//    {
//		PaintBox1->Invalidate();
//    }
//}
//
////---------------------------------------------------------------------------
//void __fastcall TMainForm::RzSpinButtons1DownLeftClick(TObject *Sender)
//{
//	CustomImageRotationE->setValue(CustomImageRotationE->getValue() - 0.5);
//	double val = CustomImageRotationE->getValue();
//	paintRotatedImage(val);
//}
//
////---------------------------------------------------------------------------
//void __fastcall TMainForm::RzSpinButtons1UpRightClick(TObject *Sender)
//{
//	CustomImageRotationE->setValue(CustomImageRotationE->getValue() +0.5);
//	double val = CustomImageRotationE->getValue();
//	paintRotatedImage(val);
//}
//
//void __fastcall TMainForm::TAffineTransformationFrame1ExecuteBtnClick(TObject *Sender)
//{
//	onSSHData = TAffineTransformationFrame1->onSSHData;
//  	TAffineTransformationFrame1->ExecuteBtnClick(Sender);
//}
//
////---------------------------------------------------------------------------
//LRESULT	TMainForm::onFinishedRenderRotate(TextMessage& msg)
//{
//    string test = msg.lparam;
//    CustomImageRotationE->setValue(0);
//    string currentStack = mRC.getCurrentProject().getSelectedStackName();
//
//	updateStacksForCurrentProject();
//	//    StackCB->H
//	ClearCacheBtn->Click();
//
//    //Click first z
//	mZs->ItemIndex = 0;
//    ClickZ(NULL);
//    return NULL;
//}
//
////Need to synchronize with main thread here
//void TMainForm::onProcessStarted(void* arg1, void* arg2)
//{
//    Log(lInfo) << "Process was started.";
//}
//
////Need to synchronize with main thread here
//void TMainForm::onProcessProgress(void* arg1, void* arg2)
//{
//    string* str = (string*) arg2;
//    Log(lInfo) << "Progress: " << (*str);
//}
//
////Need to synchronize with main thread here
//void TMainForm::onProcessFinished(void* arg1, void* arg2)
//{
//    Log(lInfo) << "Process finished.";
//    checkCache();
//}
//
////---------------------------------------------------------------------------
//void __fastcall TMainForm::CreateTiffStackAExecute(TObject *Sender)
//{
////    Process IMConvert("C:\\Program Files (x86)\\ImageMagick-7.0.8-Q16\\convert.exe", mRC.getImageLocalCachePath());
////    Process IMConvert("dir.exe", mRC.getImageLocalCachePath());
//    Process& IMConvert = mAProcess;
//
//    IMConvert.setExecutable("C:\\Program Files (x86)\\ImageMagick-7.0.8-Q16\\convert.exe");
//    IMConvert.setWorkingDirectory(mRC.getImageLocalCachePath());
//
//    //Extract selected filenames from checked z's
//    StringList sections = getCheckedItems(mZs);
//
//    //Creat output filename
//    string stackFName("stack_" + getUUID());
//
//    //Create commandline for imagemagicks convert program
//    stringstream cmdLine;
//    for(int i = 0; i < sections.count(); i++)
//    {
//        string fName(getFileNameNoPath(mRC.getImageLocalCachePathAndFileNameForZ(toInt(sections[i]))));
//        cmdLine << fName <<" ";
//    }
//
//	cmdLine << stackFName << ".tif";
//
//    Log(lInfo) << "Running convert on " << cmdLine.str();
//
//	IMConvert.setup(cmdLine.str(), mhCatchMessages);
//    IMConvert.start(true);
//}
//
//
//void __fastcall TMainForm::CreateMIPAExecute(TObject *Sender)
//{
//    string cvt("C:\\Program Files (x86)\\ImageMagick-7.0.8-Q16\\convert.exe");
//    Process& IMConvert = mAProcess;
//    IMConvert.reset();
//    IMConvert.setExecutable(cvt);
//    IMConvert.setWorkingDirectory(mRC.getImageLocalCachePath());
//
//    //Find all stacks for current ROI
//    StringList stackFiles(getFilesInFolder(mRC.getImageLocalCachePath(), "tif", false));
//
//    //Create MIP's for current stack file
//
//    string* temp = (string*) StacksCB->Items->Objects[StacksCB->ItemIndex];
//    if(!temp)
//    {
//        Log(lError) << "Failed to extract string item";
//        return;
//    }
//
//    string currentStack(*temp);
//    string mipFName(getFileNameNoExtension(currentStack));
//
//    mipFName += "_MIP.tif";
//    mipFName = replaceSubstring("stack_", "", mipFName);
//    stringstream cmdLine;
//    cmdLine << cvt <<" " << currentStack << " -evaluate-sequence max "<<mipFName;
//    Log(lInfo) << "Running convert on " << cmdLine.str();
//
//    IMConvert.setup(cmdLine.str(), mhCatchMessages);
//    IMConvert.start(true);
//}
//
//void TMainForm::onROIChanged(void* arg1, void* arg2)
//{
//    TThread::Synchronize(NULL, &roiChanged);
//}
//
//void __fastcall TMainForm::roiChanged()
//{
//    Log(lInfo) << "ROI was changed.";
//    XCoordE->setValue(mCurrentROI.getX1());
//    YCoordE->setValue(mCurrentROI.getY1());
//    Width->setValue(mCurrentROI.getWidth());
//    Height->setValue(mCurrentROI.getHeight());
//    mScaleE->setValue(mCurrentROI.getScale());
//    checkCache();
//}
//
//void TMainForm::checkCache()
//{
//    //OtherCB
//    StacksCB->Clear();
//    OtherCB->Clear();
//    StringList stackFiles(getFilesInFolder(mRC.getImageLocalCachePath(), "tif", false));
//    for(int i = 0; i < stackFiles.count(); i++)
//    {
//        if(startsWith("stack_", stackFiles[i]))
//        {
//            //Setup something robust here later on
//            string* item = new string(joinPath(mRC.getImageLocalCachePath(), stackFiles[i]));
//            stringstream itemCaption;
//            itemCaption << "Stack_"<<i + 1;
//            StacksCB->AddItem(vclstr(itemCaption.str()), (TObject*) item);
//        }
//    }
//}
////---------------------------------------------------------------------------
//void __fastcall TMainForm::OtherCBClick(TObject *Sender)
//{
//    //Open mip
//    TCheckListBox* lb = dynamic_cast<TCheckListBox*>(Sender);
//    if(lb == StacksCB)
//    {
//        string* temp = (string*) StacksCB->Items->Objects[StacksCB->ItemIndex];
//        if(!temp)
//        {
//            Log(lError) << "Failed to extract string item";
//            return;
//        }
//
//        string currentStack(replaceSubstring(".tif", "", replaceSubstring("stack_", "", *temp)));
//
//        //Populate mips for current stack
//        OtherCB->Clear();
//        StringList mipFiles(getFilesInFolder(mRC.getImageLocalCachePath(), "tif", false));
//        for(int i = 0; i < mipFiles.count(); i++)
//        {
//            if(endsWith("_MIP.tif", mipFiles[i]))
//            {
//                //Setup something robust here later on
//                string* item = new string(joinPath(mRC.getImageLocalCachePath(), mipFiles[i]));
//                if(item && contains(currentStack, *item))
//                {
//                    stringstream itemCaption;
//    	            itemCaption << "MIP_" << i + 1;
//	                OtherCB->AddItem(vclstr(itemCaption.str()), (TObject*) item);
//                }
//            }
//        }
//    }
//    else if(lb == OtherCB)
//    {
//        //Get item
//        TObject* item = lb->Items->Objects[lb->ItemIndex];
//
//        if(item)
//        {
//		    string* fName((string*) item);
//            TImageForm* iForm = new TImageForm(gAU.AppRegistryRoot, "", this);
//            iForm->load(*fName);
//            iForm->Show();
//        }
//    }
//}
//
//void TMainForm::updateROIs()
//{
//    StringList rois(mRC.getROIFoldersForCurrentStack());
//    populateCheckListBox(rois, ROI_CB);
//}
//
////---------------------------------------------------------------------------
//void __fastcall TMainForm::ROI_CBClick(TObject *Sender)
//{
//    //Switch ROI
//    if(ROI_CB->ItemIndex != -1)
//    {
//
//	    RegionOfInterest roi = RegionOfInterest(stdstr(ROI_CB->Items->Strings[ROI_CB->ItemIndex]), mCurrentROI.getScale());
//        RenderLayer rl(mRC.getProject(), roi, mRC.getCacheRoot());
//
//        mCurrentROI = roi;
//
//        mCurrentROI.setScale(rl.getLowestScaleInCache());
//        mScaleE->setValue(mCurrentROI.getScale());
//		ClickZ(Sender);
//    }
//}
//
////---------------------------------------------------------------------------
//void __fastcall TMainForm::Button1Click(TObject *Sender)
//{
//	updateROIs();
//}
