#include "DesktopPet.h"
#include "logger.h"
#include "Resource.h"

std::unordered_map<UINT, std::function<bool(HWND, WPARAM, LPARAM)>> * DesktopPet::WndProcs = nullptr;

DesktopPet::DesktopPet(HINSTANCE hInstance):
	m_hInstance(hInstance)
{
	LoadStringW(hInstance, IDS_APP_TITLE, m_szTitle, 20);
	LoadStringW(hInstance, IDC_DESKTOPPET, m_szWindowClass, 20);
	HDC device = GetDC(m_hWnd);
	image = (HBITMAP)LoadImage(0, TEXT("1.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	GetObject(image, sizeof(BITMAP), &bm);
	hdcImage = CreateCompatibleDC(device);
	SelectObject(hdcImage, image);
	WndProcs = new std::unordered_map<UINT, std::function<bool(HWND, WPARAM, LPARAM)>>(128);
	Logger::Log(L"我构造了");
}

DesktopPet::~DesktopPet()
{
	DeleteDC(hdcImage);
	DeleteObject(image);
	if (WndProcs) {
		delete WndProcs;
		WndProcs = nullptr;
	}
	Logger::Log(L"我析构了");
}

int DesktopPet::Run()
{
	if (!Start()) {
		return 0;
	}
	Loop();
	End();
	return m_msg.wParam;
}

bool DesktopPet::Start()
{
	//只允许存在一个实例
	auto handle = CreateMutex(NULL, TRUE, m_szWindowClass);
	auto err = GetLastError();
	if (err == ERROR_ALREADY_EXISTS)
	{
		Logger::Log(TEXT("已存在一个实例"));
		return false;
	}
	if (handle)
	{
		ReleaseMutex(handle);
	}
	//注册窗口类
	WNDCLASSEX wcex;
	wcex.lpfnWndProc = WndProc;
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
	m_hWnd = CreateWindowEx(WS_EX_APPWINDOW, m_szTitle, m_szWindowClass,WS_EX_OVERLAPPEDWINDOW,
		GetSystemMetrics(SM_CXSCREEN) / 2 - m_width / 2, GetSystemMetrics(SM_CYSCREEN) / 2 - m_height / 2,
		m_width, m_height, NULL, NULL, m_hInstance, 0);

	if (!m_hWnd)
	{
		return false;
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

	//渲染
	(*WndProcs)[WM_PAINT] = [this](auto hwnd, auto, auto) {
		BitBlt(GetDC(m_hWnd), 0, 0, bm.bmWidth, bm.bmHeight, hdcImage, 0, 0, SRCCOPY);
		return true;
	};
	//关闭
	(*WndProcs)[WM_DESTROY] = [this](auto hwnd, auto, auto) {
		close = true;
		return true;
	};
	//按住可拖拽
	(*WndProcs)[WM_LBUTTONDOWN] = [](auto hwnd, auto, auto) {
		SendMessage(hwnd, WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
		return true;
	};
	(*WndProcs)[WM_LBUTTONDBLCLK] = [](auto hwnd, auto, auto) {
		//
		return true;
	};
	(*WndProcs)[WM_RBUTTONDBLCLK] = [](auto hwnd, auto, auto) {
		//
		return true;
	};
	(*WndProcs)[WM_RBUTTONDOWN] = [](auto hwnd, auto, auto) {
		//
		return true;
	};
	return true;
}

void DesktopPet::End()
{
}

void DesktopPet::Loop()
{
	while (!close)
	{
		PeekMessage(&m_msg, m_hWnd, 0, 0, PM_REMOVE);
		TranslateMessage(&m_msg);
		DispatchMessage(&m_msg);
		Update();
	}
}

void DesktopPet::Update()
{
	//...
}

LRESULT CALLBACK DesktopPet::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (WndProcs->find(message) != WndProcs->end()) 
	{
		WndProcs->at(message)(hWnd, wParam, lParam);
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}
