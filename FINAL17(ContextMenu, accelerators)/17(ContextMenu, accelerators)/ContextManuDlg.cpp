#include "ContextMenuDlg.h"

//1 - MAGE
//2 - ARCHER
//3 - WARRIOR
//0 - WTF

// зроби 2 масива дл€ кожноњ команди. в першому масив≥ - резерв . в другому - команда т≥кущ€€
#define ID_MAGE WM_USER
#define ID_ARCHER WM_USER+1
#define ID_WARRIOR WM_USER+2
#define ID_MAGE2 WM_USER+3
#define ID_ARCHER2 WM_USER+4
#define ID_WARRIOR2 WM_USER+5
#define ID_DELETE WM_USER+6
#define ID_DELETE2 WM_USER+7

BOOL CALLBACK AboutProc(HWND, UINT, WPARAM, LPARAM);
CContextMenuDlg* CContextMenuDlg::ptr = NULL;

CContextMenuDlg::CContextMenuDlg(void)
{
	ptr = this;
}

void CContextMenuDlg::Cls_OnClose(HWND hwnd)
{
	DestroyWindow(hwnd);
	PostQuitMessage(0);
}

void CContextMenuDlg::Read(TCHAR* file)
{
	wfstream input_file(file, wfstream::in) ; 

	for(int i = 0 ; i<6; i++)
		input_file >> logic[i]; 

	int tmp;
	do
	{
		input_file >> tmp;
		reserv1.push_back(tmp);
	}while(tmp != 777);

	while (input_file >> tmp)
	{
		reserv2.push_back(tmp);
	}
	input_file.close();
	Refresh();
}
void CContextMenuDlg::Save(TCHAR* file)
{
	wfstream output_file(file, fstream::out) ; 

	for(int i = 0 ; i<6; i++)
		output_file << logic[i] << endl; 

	for ( it1=reserv1.begin() ; it1 < reserv1.end(); it1++ )
	{
		output_file << *it1 << endl; 
	}
	output_file << 777 << endl;
	for ( it2=reserv2.begin() ; it2 < reserv2.end(); it2++ )
	{
		output_file << *it2 << endl; 
	}
	output_file.close();
}
void CContextMenuDlg::Random()
{
	srand(time(0));
	for(int i = 0; i<6; i++)
	{
		int r = rand()%3+1;
		if(r == 1)
			SendMessage(PicArr[i], STM_SETIMAGE, WPARAM(IMAGE_BITMAP), LPARAM(MAGE));
		else if (r == 2)
			SendMessage(PicArr[i], STM_SETIMAGE, WPARAM(IMAGE_BITMAP), LPARAM(ARCHER));
		else
			SendMessage(PicArr[i], STM_SETIMAGE, WPARAM(IMAGE_BITMAP), LPARAM(WARRIOR));
		logic[i] = r;
	}
}
void CContextMenuDlg::Refresh()
{
	for(int i = 0; i<6; i++)
	{
		if(logic[i] == 1)
			SendMessage(PicArr[i], STM_SETIMAGE, WPARAM(IMAGE_BITMAP), LPARAM(MAGE));
		else if(logic[i] == 2)
			SendMessage(PicArr[i], STM_SETIMAGE, WPARAM(IMAGE_BITMAP), LPARAM(ARCHER));
		else if(logic[i] == 3)
			SendMessage(PicArr[i], STM_SETIMAGE, WPARAM(IMAGE_BITMAP), LPARAM(WARRIOR));
		else SendMessage(PicArr[i], STM_SETIMAGE, WPARAM(IMAGE_BITMAP), LPARAM(UNKNOWN));
	}
}
void CContextMenuDlg::Clear_all()
{
	for(int n = 0; n<3 ; n++)
	{
		if(logic[n] != -1)
		{
			SendMessage(PicArr[n], STM_SETIMAGE, WPARAM(IMAGE_BITMAP), LPARAM(UNKNOWN));
			reserv1.push_back(logic[n]);
			logic[n] = -1;
		}
	}
	for(int n = 3; n<6 ; n++)
	{
		if(logic[n] != -1)
		{
			SendMessage(PicArr[n], STM_SETIMAGE, WPARAM(IMAGE_BITMAP), LPARAM(UNKNOWN));
			reserv2.push_back(logic[n]);
			logic[n] = -1;
		}
	}
}
BOOL CContextMenuDlg::Check_clear()
{
	BOOL rez = true;
	for(int i = 0 ; i < 6; i++)
	{
		if(logic[i] != -1)
		{
			rez = false;
		}
	}
	return rez;
}

INT Check(POINT p1, HWND hDialog)
{
	RECT wind;
	int rez = 0;
	for(int i = 0 ; i<6; i++)
	{
		GetWindowRect(GetDlgItem(hDialog, IDC_STATIC1+i), &wind);
		if(p1.x > wind.left && p1.x < wind.right && p1.y> wind.top && p1.y < wind.bottom)
			return i;
	}
	return -1;
}

BOOL CContextMenuDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) 
{
	hDialog = hwnd;
	hMenu = LoadMenu(GetModuleHandle(0), MAKEINTRESOURCE(IDR_MAIN));
	SetMenu(hwnd, hMenu);
	for(int i = 0; i<6; i++)
		PicArr[i] = GetDlgItem(hDialog, IDC_STATIC1+i);

	MAGE = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_MAGE));
	ARCHER = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_ARCHER));
	WARRIOR = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_WARRIOR));
	UNKNOWN = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_UNKNOWN));

	Random();

	subMenu = CreatePopupMenu();

	subMenu2 = CreatePopupMenu();
	AppendMenu(subMenu2, MF_STRING, ID_ARCHER2, TEXT("ARCHER"));
	AppendMenu(subMenu2, MF_STRING, ID_MAGE2, TEXT("MAGE"));
	AppendMenu(subMenu2, MF_STRING, ID_WARRIOR2, TEXT("WARRIOR"));

	ContextMenu1 = CreatePopupMenu();
	AppendMenu(ContextMenu1, MF_POPUP, (UINT_PTR)subMenu, TEXT("ADD"));
	AppendMenu(ContextMenu1, MF_STRING, ID_DELETE, TEXT("DELETE"));

	ContextMenu2 = CreatePopupMenu();
	AppendMenu(ContextMenu2, MF_POPUP, (UINT_PTR)subMenu2, TEXT("ADD"));
	AppendMenu(ContextMenu2, MF_STRING, ID_DELETE2, TEXT("DELETE"));
	return TRUE;
}

// ќбработчик сообщени€ WM_COMMAND будет вызван при выборе пункта меню
void CContextMenuDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	INT n = 0;
	//обробка меню головного 
	switch(id)
	{
	case ID_FILE_RANDOM:
		Random();
		reserv1.clear();
		reserv2.clear();
		break;
	case ID_FILE_EXIT:
		DestroyWindow(hwnd);
		PostQuitMessage(0);
		break;
	case ID_FILE_CLEAR:
		Clear_all();
		break;
	case ID_HELP_ABOUT:
		DialogBox(GetModuleHandle(0), MAKEINTRESOURCE(IDD_ABOUT), NULL,  AboutProc); 
		break;
	case ID_FILE_SAVE40011:
		{
			TCHAR   szFile[MAX_PATH] = TEXT("\0");
			OPENFILENAME   ofn;
			HANDLE hFile = INVALID_HANDLE_VALUE;

			memset( &(ofn), 0, sizeof(ofn));
			ofn.lStructSize   = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFile = szFile;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrFilter = TEXT("Pos (*.pos)\0 *.pos\0");   
			ofn.lpstrTitle = TEXT("Save File As");
			ofn.Flags = OFN_HIDEREADONLY; 
			ofn.lpstrDefExt = TEXT("pos");

			//get the filename the user wants to save to
			if (GetSaveFileName(&ofn)) 
			{
				//save
				Save(ofn.lpstrFile);
			}
		}
		break;

	case ID_FILE_OPEN40012:
		{
			TCHAR   szFile[MAX_PATH] = TEXT("\0");
			char    *szFileContent = NULL;
			OPENFILENAME   ofn;
			HANDLE hFile = INVALID_HANDLE_VALUE;
			DWORD dwFileSize = 0, bytesToRead = 0, bytesRead = 0;
			memset( &(ofn), 0, sizeof(ofn));
			ofn.lStructSize   = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFile = szFile;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrFilter = TEXT("Pos (*.pos)\0 *.pos\0");   
			ofn.lpstrTitle = TEXT("Open File");
			ofn.Flags = OFN_EXPLORER;

			//get the filename the user wants to open
			if (GetOpenFileName(&ofn))
			{
				//open
				reserv1.clear();
				reserv2.clear();
				Read(ofn.lpstrFile);
			}

		}
		break;


	}
	n = Check(p1, hwnd);
	wchar_t *tmp = new wchar_t[128];
	wsprintf(tmp, L"%d", n);
	SetWindowText(hwnd, tmp);
	delete [] tmp;
	if((id == ID_MAGE || id == ID_ARCHER || id == ID_WARRIOR || id == ID_DELETE) && n!= -1)
	{

		switch(id)
		{
		case ID_MAGE:
			it1 = find(reserv1.begin(), reserv1.end(), 1);
			if(it1 != reserv1.end() )
				reserv1.erase(it1);
			if(logic[n] != -1)

				reserv1.push_back(logic[n]);

			SendMessage(PicArr[n], STM_SETIMAGE, WPARAM(IMAGE_BITMAP), LPARAM(MAGE));
			logic[n]=1;
			break;
		case ID_ARCHER:
			it1 = find(reserv1.begin(), reserv1.end(), 2);
			if(it1 != reserv1.end() )
				reserv1.erase(it1);
			if(logic[n] != -1)
				reserv1.push_back(logic[n]);
			SendMessage(PicArr[n], STM_SETIMAGE, WPARAM(IMAGE_BITMAP), LPARAM(ARCHER));
			logic[n]=2;
			break;
		case ID_WARRIOR:
			it1 = find(reserv1.begin(), reserv1.end(), 3);
			if(it1 != reserv1.end() )
				reserv1.erase(it1);
			if(logic[n] != -1)
				reserv1.push_back(logic[n]);
			SendMessage(PicArr[n], STM_SETIMAGE, WPARAM(IMAGE_BITMAP), LPARAM(WARRIOR));
			logic[n]=3;
			break;
		case ID_DELETE:
			SendMessage(PicArr[n], STM_SETIMAGE, WPARAM(IMAGE_BITMAP), LPARAM(UNKNOWN));
			reserv1.push_back(logic[n]);
			logic[n] = -1;
			break;
		}
	}

	if((id == ID_MAGE2 || id == ID_ARCHER2 || id == ID_WARRIOR2 || id == ID_DELETE2) && n!= -1)
	{

		switch(id)
		{
		case ID_MAGE2:
			it2 = find(reserv2.begin(), reserv2.end(), 1);
			if(it2 != reserv2.end() )
				reserv2.erase(it2);
			if(logic[n] != -1)
				reserv2.push_back(logic[n]);

			SendMessage(PicArr[n], STM_SETIMAGE, WPARAM(IMAGE_BITMAP), LPARAM(MAGE));
			logic[n]=1;
			break;
		case ID_ARCHER2:
			it2 = find(reserv2.begin(), reserv2.end(), 2);
			if(it2 != reserv2.end() )
				reserv2.erase(it2);
			if(logic[n] != -1)
				reserv2.push_back(logic[n]);
			SendMessage(PicArr[n], STM_SETIMAGE, WPARAM(IMAGE_BITMAP), LPARAM(ARCHER));
			logic[n]=2;
			break;
		case ID_WARRIOR2:
			it2 = find(reserv2.begin(), reserv2.end(), 3);
			if(it2 != reserv2.end() )
				reserv2.erase(it2);
			if(logic[n] != -1)
				reserv2.push_back(logic[n]);
			SendMessage(PicArr[n], STM_SETIMAGE, WPARAM(IMAGE_BITMAP), LPARAM(WARRIOR));
			logic[n]=3;
			break;
		case ID_DELETE2:
			SendMessage(PicArr[n], STM_SETIMAGE, WPARAM(IMAGE_BITMAP), LPARAM(UNKNOWN));
			reserv2.push_back(logic[n]);
			logic[n] = -1;
			break;
		}
	}
}

// ќбработчик сообщени€ WM_CONTEXTMENU будет вызван при щелчке правой кнопкой мыши в любой области диалогового окна
bool CContextMenuDlg::Cls_OnContextMenu(HWND hwnd, HWND hwndContext, UINT xPos, UINT yPos)
{
	p1.x = xPos;
	p1.y = yPos;
	INT n = Check(p1, hwnd);

	n = Check(p1, hwnd);
	wchar_t *tmp = new wchar_t[128];
	wsprintf(tmp, L"%d (%d,%d)", n, xPos, yPos);
	SetWindowText(hwnd, tmp);
	delete [] tmp;

	if (n >= 0 && n<=2)
	{
		if(logic[n] == -1)
			EnableMenuItem(ContextMenu1, ID_DELETE, MF_BYCOMMAND | MF_DISABLED);
		if(logic[n] >=0)	EnableMenuItem(ContextMenu1, ID_DELETE, MF_BYCOMMAND | MF_ENABLED);

		if(reserv1.empty())
			EnableMenuItem(ContextMenu1, (UINT_PTR)subMenu, MF_BYCOMMAND | MF_DISABLED);
		if(!reserv1.empty()) EnableMenuItem(ContextMenu1, (UINT_PTR)subMenu, MF_BYCOMMAND | MF_ENABLED);


		int del_count = GetMenuItemCount(subMenu);
		for(int i = 0; i < del_count; i++)
			DeleteMenu(subMenu, 0, MF_BYPOSITION);

		for ( it1=reserv1.begin() ; it1 < reserv1.end(); it1++ )
		{
			if(*it1 == 1)
				AppendMenu(subMenu, MF_STRING, ID_MAGE, TEXT("MAGE"));
			else if(*it1 == 2)
				AppendMenu(subMenu, MF_STRING, ID_ARCHER, TEXT("ARCHER"));
			else if(*it1 == 3) AppendMenu(subMenu, MF_STRING, ID_WARRIOR, TEXT("WARRIOR"));
		}

		TrackPopupMenu(ContextMenu1,TPM_LEFTALIGN, xPos, yPos, 0, hwnd, 0);
	}

	else if (n >= 3 && n<=5)
	{
		if(logic[n] == -1)
			EnableMenuItem(ContextMenu2, ID_DELETE2, MF_BYCOMMAND | MF_DISABLED);
		if(logic[n] >=0)	EnableMenuItem(ContextMenu2, ID_DELETE2, MF_BYCOMMAND | MF_ENABLED);

		if(reserv2.empty())
			EnableMenuItem(ContextMenu2, (UINT_PTR)subMenu2, MF_BYCOMMAND | MF_DISABLED);
		if(!reserv2.empty()) EnableMenuItem(ContextMenu2, (UINT_PTR)subMenu2, MF_BYCOMMAND | MF_ENABLED);


		int del_count = GetMenuItemCount(subMenu2);
		for(int i = 0; i < del_count; i++)
			DeleteMenu(subMenu2, 0, MF_BYPOSITION);

		for ( it2=reserv2.begin() ; it2 < reserv2.end(); it2++ )
		{
			if(*it2 == 1)
				AppendMenu(subMenu2, MF_STRING, ID_MAGE2, TEXT("MAGE"));
			else if(*it2 == 2)
				AppendMenu(subMenu2, MF_STRING, ID_ARCHER2, TEXT("ARCHER"));
			else if(*it2 == 3) AppendMenu(subMenu2, MF_STRING, ID_WARRIOR2, TEXT("WARRIOR"));
		}

		TrackPopupMenu(ContextMenu2,TPM_LEFTALIGN, xPos, yPos, 0, hwnd, 0);
	}
	return TRUE;
}
void CContextMenuDlg::Cls_OnInitMenuPopup(HWND hwnd, HMENU hMenu, UINT item, BOOL fSystemMenu) 
{
	if(Check_clear())
	{
		EnableMenuItem(hMenu, ID_FILE_CLEAR, MF_BYCOMMAND | MF_DISABLED);
	}
	else EnableMenuItem(hMenu, ID_FILE_CLEAR, MF_BYCOMMAND | MF_ENABLED);
}
BOOL CALLBACK AboutProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_CLOSE:
		EndDialog(hWnd, 0);
		return TRUE;
	}
	return FALSE;
}
BOOL CALLBACK CContextMenuDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
		HANDLE_MSG(hwnd, WM_CONTEXTMENU, ptr->Cls_OnContextMenu);
		HANDLE_MSG(hwnd, WM_INITMENUPOPUP, ptr->Cls_OnInitMenuPopup);
	}
	return FALSE;
}