﻿// DesktopPet.cpp : 定义应用程序的入口点。
//

#include "header.h"
#include "Resource.h"
#include "DesktopPet.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	DesktopPet desktopPet(hInstance, TEXT("1.bmp"), 400, 700);

    return desktopPet.Run();
}

