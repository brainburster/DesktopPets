﻿// DesktopPet.cpp : 定义应用程序的入口点。
//

#include "header.h"
#include "DesktopPet.h"
#include "MaidCat.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	DesktopPet<MaidCat> desktopPet(hInstance);

    return desktopPet.Run();
}

