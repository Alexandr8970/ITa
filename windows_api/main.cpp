#include "windows.h"

int main()
{
    /*
    HWND hwndStart;
    hwndStart = ::FindWindow(NULL, "Start");

    RECT lpRect;
    POINT pt;
    pt.x = 10;
    pt.y = 10;
    GetWindowRect(hwndStart, &lpRect);
    SendMessage(hwndStart, WM_LBUTTONDOWN, MK_LBUTTON, MAKELONG(pt.x,pt.y));
    SendMessage(hwndStart, WM_LBUTTONDOWN, 0,MAKELONG(pt.x,pt.y));
    */

    
    HWND hwndCalculator;
    hwndCalculator = ::FindWindow(NULL, "Calculator");
    //SendMessage(hwndCalculator, WM_SYSCOMMAND, SC_CLOSE, 0);
    ::MessageBoxA(hwndCalculator, "Test", "Test", MB_OK | MB_ABORTRETRYIGNORE);


    //PostMessage(calc, WM_KEYDOWN, (LPARAM) VK_ACCEPT,0);
    return 0;
}
