#include "hook.h"
#include "up_down.h"
#include <stdio.h>
#include <windows.h>

HHOOK KeyboardHook;

BOOL shift=FALSE;
BOOL ctrl=FALSE;
BOOL alt=FALSE;
BOOL tab=FALSE;
BOOL caps=FALSE;

//check the keys pressed
//check the window context
//check the conf existing
//check the match
//make the action
//clear the state of keys and window context

LRESULT CALLBACK HookProcedure(int nCode, WPARAM wParam, LPARAM lParam) {
    KBDLLHOOKSTRUCT *p = (KBDLLHOOKSTRUCT *)lParam;
    if (nCode == HC_ACTION)
    {
           printf("code %lu\n", p->vkCode);

        LPTSTR te= mapCodeToText(p->vkCode);
        if(te != NULL && GetAsyncKeyState(VK_LSHIFT) && GetAsyncKeyState(VK_LMENU)){
            printf("%s mapped to %lu\n", te, p->vkCode);
            EnumWindows(&EnumWindowsProc, (LPARAM)te);
            return -1;
        }
        else{
        if (p->vkCode==VK_F11)
        {
            press(VK_LSHIFT);
            pressCtrlCode('T');
            return -1;
        }
                if (p->vkCode==VK_F8)
                {
                                    press(VK_LSHIFT);
                    down(VK_LMENU);
                    press(VK_TAB);
                    press(VK_LMENU);
                    pressCtrlCode('T');
                    return -1;
                }

        if (p->vkCode==VK_F12)
                {
                    press(VK_LSHIFT);
                    UnhookWindowsHookEx(KeyboardHook);
//                    exit(0);
                    return -1;
                }
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd,  LPARAM lParam){
    char cWindow[1000];
    GetWindowTextA(hwnd,cWindow,sizeof(cWindow));
//    printf("%s\r\n", cWindow);
    //    KBDLLHOOKSTRUCT *p = (LPTSTR)lParam;
    if(strstr(cWindow, (LPTSTR)lParam)) {
        SetActiveWindow(hwnd);
        SetForegroundWindow(hwnd);
        printf("%s_", cWindow);
        return FALSE;
    }
    return TRUE;
}

void rethrowKey(int code){
}

LPTSTR mapCodeToText(int code){
    if (code==VK_F2) {return "IntelliJ";}
    if (code==VK_F1) {return "Firefox";}
    if (code==VK_F3) {return "Chrome";}
    if (code==VK_F4) {return "cmd";}
    if (code==VK_F6) {return "Sky";}
    if (code==VK_F5) {return "PyCharm";}
    printf("ret null");
    return NULL;
}
BOOL contains(LPTSTR caption,LPTSTR te){
    if( strstr(caption, te) != NULL) {
        return TRUE;
    }
    return FALSE;
}
void unhookKeyboard() {
    UnhookWindowsHookEx(KeyboardHook);
    exit(0);
}
