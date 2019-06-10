#include "Wnd.h"
#include "Resource.h"

std::unordered_map<UINT, std::function<bool(HWND, WPARAM, LPARAM)>> * Wnd::WndProcs = nullptr;

Wnd::Wnd(HINSTANCE hInstance, int width, int height) : 
	m_hInstance(hInstance),
	m_width(width),
	m_height(height)
{
	LoadStringW(hInstance, IDS_APP_TITLE, m_szTitle, 20);
	LoadStringW(hInstance, IDC_DESKTOPPET, m_szWindowClass, 20);
	WndProcs = new std::unordered_map<UINT, std::function<bool(HWND, WPARAM, LPARAM)>>(128);
	
	WNDCLASSEX wcex;
	wcex.lpfnWndProc = Wnd::WndProc;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInstance;
	wcex.hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_DESKTOPPET));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDI_DESKTOPPET);
	wcex.lpszClassName = m_szTitle;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	RegisterClassEx(&wcex);
	//创建窗口
	m_hWnd = CreateWindowEx(WS_EX_APPWINDOW, m_szTitle, m_szWindowClass, WS_POPUP,
		GetSystemMetrics(SM_CXSCREEN) / 2 - m_width / 2, GetSystemMetrics(SM_CYSCREEN) / 2 - m_height / 2,
		m_width, m_height, NULL, NULL, m_hInstance, 0);

	if (!m_hWnd)
	{
		return ;
	}

	//修改窗口style
	LONG style = ::GetWindowLong(m_hWnd, GWL_STYLE);
	style = style & ~WS_CAPTION &~WS_SYSMENU &~WS_SIZEBOX | SWP_NOSIZE; //| CS_OWNDC
	SetWindowLong(m_hWnd, GWL_STYLE, style);

	//修改窗口ex_style
	LONG ex_style = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
	ex_style = ex_style | WS_EX_LAYERED | WS_EX_LAYERED;/*| WS_EX_TOPMOST 创建后失效*/
	SetWindowLong(m_hWnd, GWL_EXSTYLE, ex_style);
	SetLayeredWindowAttributes(m_hWnd, 0xffffffff, 0, LWA_COLORKEY);
	//置顶
	SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	ShowWindow(m_hWnd, SW_SHOWNORMAL);
	UpdateWindow(m_hWnd);
}

Wnd::~Wnd()
{
	if (WndProcs) {
		delete WndProcs;
		WndProcs = nullptr;
	}
}

const HWND & Wnd::GetHWND() const
{
	return m_hWnd;
}

const HINSTANCE & Wnd::GetHInstance() const
{
	return m_hInstance;
}

LRESULT Wnd::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (WndProcs->find(message) != WndProcs->end()&& WndProcs->at(message)(hWnd, wParam, lParam))
	{
		return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

void Wnd::RegisterWndProc(UINT message, std::function<bool(HWND, WPARAM, LPARAM)> wndProc)
{
	if ((*WndProcs).find(message)== (*WndProcs).end()) {
		(*WndProcs)[message] = wndProc;
	}
	else {
		//函数合并
		auto lastFunc = (*WndProcs)[message];
		(*WndProcs)[message] = [=](auto a, auto b, auto c){return lastFunc(a, b, c)&&wndProc(a, b, c); };
	}
}
