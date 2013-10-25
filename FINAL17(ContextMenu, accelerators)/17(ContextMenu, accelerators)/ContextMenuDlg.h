#pragma once
#include "header.h"

class CContextMenuDlg
{
public:
	CContextMenuDlg(void);
public:
	static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	static CContextMenuDlg* ptr;
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	void Cls_OnClose(HWND hwnd);
	bool Cls_OnContextMenu(HWND hwnd, HWND hwndContext, UINT xPos, UINT yPos);
	void Cls_OnInitMenuPopup(HWND hwnd, HMENU hMenu, UINT item, BOOL fSystemMenu);
	void Random();
	BOOL Check_clear();
	void Clear_all();
	void Refresh();
	void Read(TCHAR* file);
	void Save(TCHAR* file);
	HWND hDialog, hPic1, hPic2, hPic3, hPic4, hPic5, hPic6, PicArr[6];
	HBITMAP MAGE, ARCHER, WARRIOR, UNKNOWN;
	HMENU hMenu, subMenu, subMenu2, ContextMenu1, ContextMenu2;
	POINT p1;
	std::vector<int> reserv1, reserv2;
	std::vector<int>::iterator it1, it2;
	int logic[6];
	//int slot_selected = 
};
