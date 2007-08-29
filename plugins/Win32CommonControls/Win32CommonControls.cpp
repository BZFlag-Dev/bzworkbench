// Win32CommonControls.cpp : Defines the initialization routines for the DLL.
//


// Windows Header Files:
#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <commctrl.h>
#include <commoncontrols.h>

#include <vector>
#include <string>

#include "BZWBAPI.h"

BZWB_GET_PLUGIN_VERSION

HWND	theAppWindow = NULL;
HINSTANCE	theInstance = NULL;

typedef struct 
{
	std::string					filename;
	std::vector<std::string>	extensions;
	int							extension;
	std::string					title;
}trStandardFileOpen;

bool getStdFileOpen ( trStandardFileOpen &data )
{
	OPENFILENAME	ofn;

	memset(&ofn,0,sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = theAppWindow;
	ofn.hInstance = theInstance;

	char filter[1024] = {0};

	int pos = 0;

	for (unsigned int i = 0; i < data.extensions.size(); i++)
	{
		int size = 0;
		char temp[128] = {0};
		sprintf(temp,"%s Files (*.%s)",data.extensions[i].c_str(),data.extensions[i].c_str());
		size += 11 + (data.extensions[i].size()*2);
		temp[size] = 0;
		size++;
		sprintf(temp+size,"*.%s",data.extensions[i].c_str());
		size += 2 + (data.extensions[i].size());
		temp[size] = 0;
		size++;		
		memcpy(filter + pos,temp,size);
		pos += size;
	}
	strcat(filter+pos,"All Files (*.*)");
	pos += 15;
	filter[pos] = 0;
	pos++;
	strcat(filter+pos,"*.*");

	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = data.extension;
	ofn.nMaxFile = _MAX_PATH;
	ofn.nMaxFileTitle = _MAX_PATH + _MAX_EXT;

	char defaultExtension[_MAX_EXT];
	if ( data.extensions.size() )
		strcpy(defaultExtension,data.extensions[data.extension].c_str());
	else
		strcpy(defaultExtension,"*");

	ofn.lpstrDefExt = defaultExtension;

	char filename[MAX_PATH];
	char title[MAX_PATH];

	strcpy(filename,data.filename.c_str());
	strcpy(title,data.title.c_str());

	ofn.lpstrFile = filename;
	ofn.lpstrFileTitle = title;
	ofn.Flags = OFN_HIDEREADONLY;

	bool good = GetOpenFileName(&ofn) != 0;
	if (good)
	{
		data.filename = ofn.lpstrFile;
	}
	else
	{
		DWORD err = CommDlgExtendedError();
	}

	return good;
}


class  Win32OpenDialog : public bzw_OpenDialogControlHandler
{
public:
	Win32OpenDialog():bzw_OpenDialogControlHandler(){};

	virtual bool handle ( void )
	{
		std::string realExtension = extension;

		char filter[512];
		sprintf(filter,"%s files (%s)|%s|All Files (*.*)|*.*||",realExtension.c_str()+1,extension.c_str(),extension.c_str());

		realExtension.erase(realExtension.begin());
		
		std::string path = directory + file;

		trStandardFileOpen openData;

		openData.extensions.push_back(realExtension);
		openData.extension = 0;
		openData.filename = path;
		//openData.title = description;

/*		CWnd *mainWindow = CWnd::FromHandle(theAppWindow);
		mainWindow->MessageBox("open dialgog");

		CFileDialog oFileDialog(true,realExtension.c_str(),path.c_str(),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, mainWindow);

		if (oFileDialog.DoModal() == IDOK ) */
		if (getStdFileOpen(openData))
		{
			file = openData.filename;
			sucsessful = false;
		}
		else
			sucsessful = false;

		return true;
	}
};

Win32OpenDialog	openDialog;

BZWB_PLUGIN_CALL int bzwb_Load (const char* /*commandLine*/, void * instance )
{
	theAppWindow = (HWND) bzwb_getOSMainWindowHandle();
	theInstance = (HINSTANCE)instance;

	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwICC = ICC_LISTVIEW_CLASSES;
	InitCtrls.dwSize = sizeof(INITCOMMONCONTROLSEX);
	BOOL bRet = InitCommonControlsEx(&InitCtrls);

	bzwb_registerCommonControlHandler(openDialog.type, &openDialog);
	return 0;
}

BZWB_PLUGIN_CALL int bzwb_Unload (void)
{
	bzwb_removeCommonControlHandlerr(openDialog.type, &openDialog);
	return 0;
}
