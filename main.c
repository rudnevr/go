#include "up_down.h"
#include "hook.h"
#include <windows.h>
#include "stdio.h"
#include <stdlib.h>


void setHook(){
    printf("start\n");
    KeyboardHook = SetWindowsHookEx(
            WH_KEYBOARD_LL,
            HookProcedure,
            GetModuleHandle(NULL),
            0
            );
//    if (!KeyboardHook){
//        printf("no kbhook");
//    }
//    else {
//        printf("kbhook success");
        MSG Msg;
        while (GetMessage(&Msg, NULL, 0, 0) > 0)
        {
            TranslateMessage(&Msg);
            DispatchMessage(&Msg);
        }
//    }
}

void readFile() {
    FILE *fptr;
    char ch;
    fptr = fopen("input.txt", "r");
    if (fptr == NULL)
    {
        printf("Cannot open file \n");
        exit(0);
    }
    ch = fgetc(fptr);
    while (ch != EOF)
    {
        if (ch != '\t'){
            printf ("%c", ch);
        }
        else
            printf ("%s", "tab");
        ch = fgetc(fptr);
    }
    fclose(fptr);
}

int main (void) {
    //    Sleep(5000);
    //    down(VK_LCONTROL);
    //    press('J');
    //    press(VK_LCONTROL);
    //    printf("%d"+VK_LCONTROL);
//    readFile();
   setHook();
//    unhookKeyboard();
    return 0;
}