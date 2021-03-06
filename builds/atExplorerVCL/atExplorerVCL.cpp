#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("..\..\Source\vcl\forms\TCreateLocalVolumesForm.cpp", CreateLocalVolumesForm);
USEFORM("..\..\Source\vcl\forms\TCreateMediansForm.cpp", CreateMediansForm);
USEFORM("..\..\Source\vcl\forms\TImageForm.cpp", ImageForm);
USEFORM("..\..\Source\vcl\forms\TRenderAPIChecker.cpp", RenderAPIChecker);
USEFORM("..\..\Source\vcl\forms\TCreateATIFDataStateTablesForm.cpp", CreateATIFDataStateTablesForm);
USEFORM("..\..\Source\vcl\forms\TCreateACQRenderStacksForm.cpp", CreateACQRenderStacksForm);
USEFORM("..\..\Source\vcl\forms\TCreateATIFDataProjectForm.cpp", CreateATIFDataProjectForm);
USEFORM("..\..\Source\vcl\frames\TCreateStackThreadFrame.cpp", CreateStackThreadFrame); /* TFrame: File Type */
USEFORM("..\..\Source\vcl\frames\TPointMatchCollectionFrame.cpp", PointMatchCollectionFrame); /* TFrame: File Type */
USEFORM("..\..\Source\vcl\frames\TRenderProjectFrame.cpp", RenderProjectFrame); /* TFrame: File Type */
USEFORM("..\..\Source\vcl\frames\TATProjectOptionsFrame.cpp", ATProjectOptionsFrame); /* TFrame: File Type */
USEFORM("..\..\Source\vcl\forms\TSelectPointMatchCollectionProjectForm.cpp", SelectPointmatchContextProjectForm);
USEFORM("..\..\Source\vcl\forms\TSelectRenderProjectParametersForm.cpp", SelectRenderProjectParametersForm);
USEFORM("..\..\Source\vcl\forms\TSimpleTextInputDialog.cpp", NewValueE);
USEFORM("..\..\Source\vcl\frames\TATIFDataProjectFrame.cpp", ATIFDataProjectFrame); /* TFrame: File Type */
USEFORM("..\..\Source\vcl\frames\TTextFileFrame.cpp", TextFileFrame); /* TFrame: File Type */
USEFORM("..\..\Source\vcl\frames\TTiffStackProjectFrame.cpp", TiffStackProjectFrame); /* TFrame: File Type */
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//   Package source.

#pragma argsused
extern "C" int _libmain(unsigned long reason)
{
	return 1;
}
//---------------------------------------------------------------------------
#pragma comment(lib, "dslFoundation.lib")

#pragma comment(lib, "dslFoundation")
#pragma comment(lib, "dslVCLCommon.lib")

#pragma comment(lib, "dslVCLComponents.bpi")
#pragma comment(lib, "dslVCLVisualComponents.bpi")


#pragma comment(lib, "atExplorerFoundation.lib")

#pragma comment(lib, "poco_foundation.lib")
#pragma comment(lib, "tinyxml2.lib")
#pragma comment(lib, "libcurl_imp.lib")

//#pragma comment(lib, "DCEFBrowser.bpi")



