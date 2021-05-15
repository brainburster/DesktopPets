#include "RightMenu.h"
#include "resource.h"
#include "logger.h"

RightMenu::RightMenu(Wnd* wnd)
{
	hMenu = LoadMenu(wnd->GetHInstance(), MAKEINTRESOURCE(IDR_MENU1));
	if (hMenu) {
		hMenu = GetSubMenu(hMenu, 0);
	}
	//��ʾ�Ҽ��˵�
	wnd->RegisterWndProc(WM_CONTEXTMENU, [&](auto wparam, auto lparam) {
		TrackPopupMenu(hMenu,
			TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RIGHTBUTTON,
			LOWORD(lparam),//x
			HIWORD(lparam),//y
			0,
			(HWND)wparam,
			NULL
		);
		return true;
		});

	//�����Ҽ��˵�
	wnd->RegisterWndProc(WM_COMMAND, [wnd, this](auto wparam, auto lparam) {
		switch (LOWORD(wparam))
		{
		case ID_ABOUT:
			//�������ڶԻ���
			DialogBox((HINSTANCE)GetWindowLongW(wnd->GetHWND(), GWLP_HINSTANCE), MAKEINTRESOURCE(IDD_ABOUTBOX), wnd->GetHWND(), [](auto hDlg, auto message, auto wparam, auto lparam) {
				switch (message)
				{
				case WM_INITDIALOG:
					break;
				case WM_COMMAND:
					switch (wparam)
					{
					case IDOK:
						EndDialog(hDlg, 0);
						break;
					}
					break;
				case WM_CLOSE:
					EndDialog(hDlg, 0);
					break;
				default:
					return INT_PTR(false);
				}
				return INT_PTR(true);
				});
			break;
		case ID_SETTING:
			Logger::Log(L"��û��", wnd->GetHWND());
			break;
		case ID_QUIT:
			SendMessage(wnd->GetHWND(), WM_CLOSE, NULL, NULL);
			break;
		default:
			return false;
			break;
		}
		return true;
		});
}

RightMenu::~RightMenu()
{
	DestroyMenu(hMenu);
}
