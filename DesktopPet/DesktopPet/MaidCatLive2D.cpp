#include "MaidCatLive2D.h"
#include "Wnd.h"
#include "Live2d\LAppDelegate.hpp"
#include "Live2d\LAppPal.hpp"
#include "Live2d\LAppView.hpp"

class MaidCatLive2D::Impl
{
};

MaidCatLive2D::MaidCatLive2D(Wnd* wnd) :Character(wnd), pImpl{ std::make_unique<Impl>() }
{
	// アプリケーション初期化
	if (!LAppDelegate::GetInstance()->Initialize(wnd->GetHWND()))
	{// 初期化失敗
		LAppDelegate::GetInstance()->Release();
		LAppDelegate::ReleaseInstance();
		return;
	}

	wnd->RegisterWndProc(WM_LBUTTONDOWN, [](auto wParam, auto lParam) {
		auto lapp_delegate = LAppDelegate::GetInstance();
		if (lapp_delegate != NULL)
		{
			if (lapp_delegate->GetView() == NULL)
			{
				return false;
			}
			float mouseX = static_cast<float>(LOWORD(lParam));
			float mouseY = static_cast<float>(HIWORD(lParam));
			lapp_delegate->SetMouse(mouseX, mouseY);
			{
				lapp_delegate->SetCaptured(true);
				lapp_delegate->GetView()->OnTouchesBegan(mouseX, mouseY);
			}
		}
		return true;
		});
	wnd->RegisterWndProc(WM_LBUTTONUP, [wnd](auto, auto lParam) {
		auto lapp_delegate = LAppDelegate::GetInstance();
		if (lapp_delegate != NULL)
		{
			if (lapp_delegate->GetView() == NULL)
			{
				return true;
			}
			float mouseX = static_cast<float>(LOWORD(lParam));
			float mouseY = static_cast<float>(HIWORD(lParam));
			lapp_delegate->SetMouse(mouseX, mouseY);

			{
				if (lapp_delegate->GetCaptured())
				{
					lapp_delegate->SetCaptured(false);
					lapp_delegate->GetView()->OnTouchesEnded(mouseX, mouseY);
				}
			}
		}
		return true;
		});

	wnd->RegisterWndProc(WM_MOUSEMOVE, [wnd](auto, auto lParam) {
		auto lapp_delegate = LAppDelegate::GetInstance();
		if (lapp_delegate != NULL)
		{
			//if (!lapp_delegate->GetCaptured())
			//{
			//	return true;
			//}
			if (lapp_delegate->GetView() == NULL)
			{
				return true;
			}
			float mouseX = static_cast<float>(LOWORD(lParam));
			float mouseY = static_cast<float>(HIWORD(lParam));
			lapp_delegate->SetMouse(mouseX, mouseY);

			lapp_delegate->GetView()->OnTouchesMoved(mouseX, mouseY);
		}
		return true;
		});
}

MaidCatLive2D::~MaidCatLive2D()
{
}

void MaidCatLive2D::Draw()
{
	LAppDelegate::GetInstance()->Frame();
}

void MaidCatLive2D::Logic()
{
	POINT p;
	GetCursorPos(&p);
	RECT r1;
	GetWindowRect(wnd->GetHWND(), &r1);
	RECT r2;
	GetClientRect(wnd->GetHWND(), &r2);

	auto lapp_delegate = LAppDelegate::GetInstance();
	if (lapp_delegate != NULL)
	{
		//if (!lapp_delegate->GetCaptured())
		//{
		//	return ;
		//}
		if (lapp_delegate->GetView() == NULL)
		{
			return;
		}
		float mouseX = static_cast<float>(p.x - r1.right + r2.right);
		float mouseY = static_cast<float>(p.y - r1.bottom + r2.bottom);
		lapp_delegate->SetMouse(mouseX, mouseY);

		lapp_delegate->GetView()->OnTouchesMoved(mouseX, mouseY);
	}
}
