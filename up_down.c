#include "up_down.h"
#include <stdio.h>
#include <windows.h>

void down(int code){
    INPUT input;
    WORD vkey = code;
    input.type = INPUT_KEYBOARD;
//    input.ki.wScan = MapVirtualKey(vkey, MAPVK_VK_TO_VSC);
    input.ki.wScan = 0;
    input.ki.time = 0;
    input.ki.dwExtraInfo = 0;
    input.ki.wVk = vkey;
    input.ki.dwFlags = 0;
    SendInput(1, &input, sizeof(INPUT));
    SendInput(1, &input, sizeof(INPUT));
}

void press(int code){
    INPUT input;
    WORD vkey = code;
    input.type = INPUT_KEYBOARD;
//    input.ki.wScan = MapVirtualKey(vkey, MAPVK_VK_TO_VSC);
    input.ki.wScan = 0;
    input.ki.time = 0;
    input.ki.dwExtraInfo = 0;
    input.ki.wVk = vkey;
    input.ki.dwFlags = 0;
    SendInput(1, &input, sizeof(INPUT));
    input.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(INPUT));
}

void pressCtrlCode(int code){
        down(VK_LCONTROL);
        press(code);
        press(VK_LCONTROL);
}