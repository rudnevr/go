#include "hook.h"
#include <stdio.h>
#include <windows.h>

HHOOK KeyboardHook;
BOOL shift=FALSE;
BOOL caps=FALSE;
LRESULT CALLBACK HookProcedure(int nCode, WPARAM wParam, LPARAM lParam) {
    KBDLLHOOKSTRUCT *p = (KBDLLHOOKSTRUCT *)lParam;
    if (nCode == HC_ACTION)
    {
        LPTSTR te= mapCodeToText(p->vkCode);
        if(te != NULL && GetAsyncKeyState(VK_LSHIFT)){
            printf(te);
            EnumWindows(&EnumWindowsProc, (LPARAM)te);
            return -1;
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd,  LPARAM lParam){
    char cWindow[1000];
    GetWindowTextA(hwnd,cWindow,sizeof(cWindow));
//    printf("%s\r\n", cWindow);
    //    KBDLLHOOKSTRUCT *p = (LPTSTR)lParam;
    if(contains(cWindow, (LPTSTR)lParam)) {
        SetActiveWindow(hwnd);
        SetForegroundWindow(hwnd);
//        printf("%s_", cWindow);
        return FALSE;
    }
    return TRUE;
}

LPTSTR mapCodeToText(int code){
    printf("%d", code);
    if (code==VK_F2) {
    return "IntelliJ";}
    if (code==VK_F3) {return "Chrome";}
    return NULL;
}
BOOL contains(LPTSTR caption,LPTSTR te){
//    printf("in contains");
    if( strstr(caption, te) != NULL) {
//        text = NULL;
        return TRUE;
    }
    return FALSE;
}
void unhookKeyboard() {
    UnhookWindowsHookEx(KeyboardHook);
    exit(0);
}
