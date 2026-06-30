#include<Windows.h>
#include<CommCtrl.h>
#include<cstdio>
#include"resource.h"

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, DlgProc, NULL);
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		SendMessage(GetDlgItem(hwnd, IDC_SPIN_PREFIX), UDM_SETRANGE, 0, MAKEWORD(32, 0));
		break;
	case WM_COMMAND:
	{
		HWND hIPaddress = GetDlgItem(hwnd, IDC_IPADDRESS);
		HWND hIPmask = GetDlgItem(hwnd, IDC_IPMASK);
		HWND hEditPrefix = GetDlgItem(hwnd, IDC_EDIT_PREFIX);
		DWORD dwIPaddress = 0;
		DWORD dwIPmask = 0;
		DWORD dwIPprefix = 0;
		CHAR  szIPprefix[32] = {};
		switch (LOWORD(wParam))
		{
		case IDC_IPADDRESS:
			SendMessage(hIPaddress, IPM_GETADDRESS, 0, (LPARAM)&dwIPaddress);
			if (FIRST_IPADDRESS(dwIPaddress) < 128)SendMessage(hIPmask, IPM_SETADDRESS, 0, MAKEIPADDRESS(255,0,0,0));
			else if (FIRST_IPADDRESS(dwIPaddress) < 192)SendMessage(hIPmask, IPM_SETADDRESS, 0, MAKEIPADDRESS(255,255,0,0));
			else if (FIRST_IPADDRESS(dwIPaddress) < 224)SendMessage(hIPmask, IPM_SETADDRESS, 0, MAKEIPADDRESS(255,255,255,0));
			break;
		case IDC_IPMASK:
			SendMessage(hIPmask, IPM_GETADDRESS, 0, (LPARAM)&dwIPmask);
			for (dwIPprefix = 0; dwIPmask; dwIPprefix++, dwIPmask <<= 1);
			sprintf(szIPprefix, "%i", dwIPprefix);
			SendMessage(hEditPrefix, WM_SETTEXT, 0, (LPARAM)szIPprefix);
			break;
		case IDC_EDIT_PREFIX:
		{
			if (HIWORD(wParam) == EN_CHANGE)
			{
				SendMessage(hEditPrefix, WM_GETTEXT, 31, (LPARAM)szIPprefix);
				INT iPrefix = atoi(szIPprefix);	//atoi() - ANSI/ASCII string to INT.
				//ANSI/ASCII-string (NULL-Terminated Line) - строка, заканчивающаяся нулем (C-string).
				//Массив элементов 'CHAR', последним элементом которого является '\0';
				if (iPrefix > 32)SendMessage(hEditPrefix, WM_SETTEXT, 0, (LPARAM)"32");
			}
		}
		break;
		case IDOK:
			break;
		case IDCANCEL:EndDialog(hwnd, 0);
		}
	}
	break;
	case WM_CLOSE:EndDialog(hwnd, 0);
	}
	return FALSE;
}