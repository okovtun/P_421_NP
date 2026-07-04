#include<Windows.h>
#include<CommCtrl.h>
#include<cstdio>
#include<iostream>
#include"resource.h"
using std::cin;
using std::cout;
using std::endl;;

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
CHAR* FormatIPaddress(CONST CHAR szMessage[], CHAR szBuffer[], DWORD dwIPaddress);
CHAR* FormatNumber(CONST CHAR szMessage[], CHAR szBuffer[], DWORD dwIPaddress);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, DlgProc, NULL);
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HWND hIPaddress = GetDlgItem(hwnd, IDC_IPADDRESS);
	HWND hIPmask = GetDlgItem(hwnd, IDC_IPMASK);
	HWND hEditPrefix = GetDlgItem(hwnd, IDC_EDIT_PREFIX);
	DWORD dwIPaddress = 0;
	DWORD dwIPmask = 0;
	DWORD dwIPprefix = 0;
	CHAR  szIPprefix[32] = {};
	switch (uMsg)
	{
	case WM_INITDIALOG:
		SendMessage(GetDlgItem(hwnd, IDC_SPIN_PREFIX), UDM_SETRANGE, 0, MAKEWORD(32, 0));
		//AllocConsole();
		freopen("CONOUT$", "w", stdout);
		break;
	case WM_COMMAND:
	{
		/*HWND hIPaddress = GetDlgItem(hwnd, IDC_IPADDRESS);
		HWND hIPmask = GetDlgItem(hwnd, IDC_IPMASK);
		HWND hEditPrefix = GetDlgItem(hwnd, IDC_EDIT_PREFIX);
		DWORD dwIPaddress = 0;
		DWORD dwIPmask = 0;
		DWORD dwIPprefix = 0;
		CHAR  szIPprefix[32] = {};*/
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_RESET:
			SendMessage(hIPaddress, IPM_SETADDRESS, 0, 0);
			SendMessage(hEditPrefix, WM_SETTEXT, 0, (LPARAM)"");
			break;
		case IDC_IPADDRESS:
			SendMessage(hIPaddress, IPM_GETADDRESS, 0, (LPARAM)&dwIPaddress);
			SendMessage(hEditPrefix, WM_GETTEXT, 32, (LPARAM)szIPprefix);
			dwIPprefix = atoi(szIPprefix);
			/*if (FIRST_IPADDRESS(dwIPaddress) < 128)SendMessage(hIPmask, IPM_SETADDRESS, 0, MAKEIPADDRESS(255,0,0,0));
			else if (FIRST_IPADDRESS(dwIPaddress) < 192)SendMessage(hIPmask, IPM_SETADDRESS, 0, MAKEIPADDRESS(255,255,0,0));
			else if (FIRST_IPADDRESS(dwIPaddress) < 224)SendMessage(hIPmask, IPM_SETADDRESS, 0, MAKEIPADDRESS(255,255,255,0));*/
			if (dwIPprefix == 0 || dwIPprefix == 8 || dwIPprefix == 24)
			{
				if (FIRST_IPADDRESS(dwIPaddress) < 128)SendMessage(hEditPrefix, WM_SETTEXT, 0, (LPARAM)"8");
				else if (FIRST_IPADDRESS(dwIPaddress) < 192)SendMessage(hEditPrefix, WM_SETTEXT, 0, (LPARAM)"16");
				else if (FIRST_IPADDRESS(dwIPaddress) < 224)SendMessage(hEditPrefix, WM_SETTEXT, 0, (LPARAM)"24");
			}
			break;

		case IDC_IPMASK:
			/*
			SendMessage(hIPmask, IPM_GETADDRESS, 0, (LPARAM)&dwIPmask);
			for (dwIPprefix = 0; dwIPmask; dwIPprefix++, dwIPmask <<= 1);//Тело этого цикла состоит из одного пустого выражения
			sprintf(szIPprefix, "%i", dwIPprefix);
			//https://legacy.cplusplus.com/reference/cstdio/sprintf/?kw=sprintf
			//https://legacy.cplusplus.com/reference/cstdio/printf/
			SendMessage(hEditPrefix, WM_SETTEXT, 0, (LPARAM)szIPprefix);
			*/
			break;
		case IDC_EDIT_PREFIX:
		{
			if (HIWORD(wParam) == EN_CHANGE)
			{
				SendMessage(hEditPrefix, WM_GETTEXT, 31, (LPARAM)szIPprefix);
				INT iPrefix = atoi(szIPprefix);	//atoi() - ANSI/ASCII string to INT.
				//ANSI/ASCII-string (NULL-Terminated Line) - строка, заканчивающаяся нулем (C-string).
				//Массив элементов 'CHAR', последним элементом которого является '\0';
				if (iPrefix > 32)
				{
					SendMessage(hEditPrefix, WM_SETTEXT, 0, (LPARAM)"32");
					iPrefix = 32;
				}
				dwIPmask = UINT_MAX;	//все биты в Маске устанавливаем в 1
				for (INT i = 0; i < 32 - iPrefix; dwIPmask <<= 1, i++);
				SendMessage(hIPmask, IPM_SETADDRESS, 0, dwIPmask);
			}
		}
		break;
		case IDOK:
		{
			CHAR szInfo[256] = {};
			CHAR szNetworkAddress[256] = {};
			CHAR szBroadcastAddress[256] = {};
			CHAR szIPamount[256] = {};
			CHAR szHostAmount[256] = {};
			SendMessage(hIPaddress, IPM_GETADDRESS, 0, (LPARAM)&dwIPaddress);
			SendMessage(hIPmask, IPM_GETADDRESS, 0, (LPARAM)&dwIPmask);
			DWORD dwNetworkAddress = dwIPaddress & dwIPmask;
			DWORD dwBroadcastAddress = dwIPaddress | ~dwIPmask;
			DWORD dwIPamount = 0;
			DWORD dwHostAmount = 0;

			sprintf
			(
				szInfo,
				"%s;\n%s;\n%s;\n%s;",
				FormatIPaddress("Адрес сети:\t\t\t", szNetworkAddress, dwNetworkAddress),
				FormatIPaddress("Широковещательный адрес:\t", szBroadcastAddress, dwBroadcastAddress),
				FormatNumber("Количество IP-адресов:\t", szIPamount, dwIPamount),
				FormatNumber("Количество узлов:\t\t", szHostAmount, dwHostAmount)
			);
			SendMessage(GetDlgItem(hwnd, IDC_STATIC_INFO), WM_SETTEXT, 0, (LPARAM)szInfo);
		}
		break;
		case IDCANCEL:EndDialog(hwnd, 0);
		}
	}
	break;
	case WM_NOTIFY:
	{
		//if (wParam == IDC_IPMASK)
		{
			SendMessage(hIPmask, IPM_GETADDRESS, 0, (LPARAM)&dwIPmask);
			for (dwIPprefix = 0; dwIPmask; dwIPprefix++, dwIPmask <<= 1);//Тело этого цикла состоит из одного пустого выражения
			sprintf(szIPprefix, "%i", dwIPprefix);
			//https://legacy.cplusplus.com/reference/cstdio/sprintf/?kw=sprintf
			//https://legacy.cplusplus.com/reference/cstdio/printf/
			SendMessage(hEditPrefix, WM_SETTEXT, 0, (LPARAM)szIPprefix);
		}
	}
	break;
	case WM_CLOSE:EndDialog(hwnd, 0);
	}
	return FALSE;
}
CHAR* FormatIPaddress(CONST CHAR szMessage[], CHAR szBuffer[], DWORD dwIPaddress)
{
	ZeroMemory(szBuffer, strlen(szBuffer) + 1);
	sprintf
	(
		szBuffer,
		"%s%i.%i.%i.%i",
		szMessage,
		FIRST_IPADDRESS(dwIPaddress),
		SECOND_IPADDRESS(dwIPaddress),
		THIRD_IPADDRESS(dwIPaddress),
		FOURTH_IPADDRESS(dwIPaddress)
	);
	return szBuffer;
}
CHAR* FormatNumber(CONST CHAR szMessage[], CHAR szBuffer[], DWORD dwNumber)
{
	ZeroMemory(szBuffer, strlen(szBuffer) + 1);
	sprintf
	(
		szBuffer,
		"%s%i",
		szMessage,
		dwNumber
	);
	return szBuffer;
}