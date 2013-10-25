#include "ContextMenuDlg.h"


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpszCmdLine, int nCmdShow)
{
	CContextMenuDlg dlg;
	MSG msg;
	HWND hDialog = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, CContextMenuDlg::DlgProc);
	HACCEL hAccel = LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_ACCELERATOR1)); 
	ShowWindow(hDialog, nCmdShow);
	while(GetMessage(&msg, 0, 0, 0))
	{
		if(!TranslateAccelerator(hDialog, hAccel, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;


}