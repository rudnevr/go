#include "hook.h"
#include <stdio.h>
#include <windows.h>

HHOOK KeyboardHook;
BOOL shift=FALSE;
BOOL caps=FALSE;
LPTSTR text;
LRESULT CALLBACK HookProcedure(int nCode, WPARAM wParam, LPARAM lParam) {
    KBDLLHOOKSTRUCT *p = (KBDLLHOOKSTRUCT *)lParam;
    if (nCode == HC_ACTION)
    {
        if (p->vkCode == VK_LSHIFT || p->vkCode == VK_RSHIFT)
        {
            if (wParam == WM_KEYDOWN)
            {
                shift = TRUE;
                printf("shift keydown");
            }
            else
            {
                shift = FALSE;
            }
        }
        mapCodeToText(p->vkCode);
        if(text!=NULL && shift){
            EnumWindows(&EnumWindowsProc, lParam);
            return -1;
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd,  LPARAM lParam){
    char cWindow[1000];
    GetWindowTextA(hwnd,cWindow,sizeof(cWindow));
    printf("%s\r\n", cWindow);
    //    KBDLLHOOKSTRUCT *p = (KBDLLHOOKSTRUCT *)lParam;
    if(contains(cWindow)) {
        SetActiveWindow(hwnd);
        SetForegroundWindow(hwnd);
        printf("%s_", cWindow);
        //        BringWindowToTop(hwnd);
        //        SwitchToThisWindow(hwnd,TRUE);
        return FALSE;
    }
    return TRUE;
}

void mapCodeToText(int code){
    if (code==VK_F2) text = "IntelliJ";
    if (code==VK_F3) text = "Chrome";
}
BOOL contains(LPTSTR caption){
    printf("in contains");
    if( strstr(caption, text) != NULL) {
        printf("in strstr");
        text = NULL;
        return TRUE;
    }
    return FALSE;
}
void unhookKeyboard() {
    UnhookWindowsHookEx(KeyboardHook);
    exit(0);
}
