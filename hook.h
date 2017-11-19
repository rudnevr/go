#include <windows.h>

HHOOK KeyboardHook;
LRESULT CALLBACK HookProcedure(int nCode, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK EnumWindowsProc(HWND   hwnd,  LPARAM lParam);
void unhookKeyboard();
void mapCodeToText(int code);
BOOL contains(LPTSTR caption);