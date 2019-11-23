package main

/*
#include <Windows.h>
#include <Winuser.h>
*/
import "C"

import (
	"fmt"
	"syscall"
	//~ "time"
	"unsafe"
	"golang.org/x/sys/windows"
	"github.com/AllenDang/w32"
)

// String returns a human-friendly display name of the hotkey
// such as "Hotkey[Id: 1, Alt+Ctrl+O]"
var (
	user32                  = windows.NewLazySystemDLL("user32.dll")
	procSetWindowsHookEx    = user32.NewProc("SetWindowsHookExA")
	procLowLevelKeyboard    = user32.NewProc("LowLevelKeyboardProc")
	procCallNextHookEx      = user32.NewProc("CallNextHookEx")
	procUnhookWindowsHookEx = user32.NewProc("UnhookWindowsHookExA")
	procGetMessage          = user32.NewProc("GetMessageW")
	procTranslateMessage    = user32.NewProc("TranslateMessage")
	procDispatchMessage     = user32.NewProc("DispatchMessageW")
	procSendInput           = user32.NewProc("SendInput")
	keyboardHook            HHOOK
)

const (
	INPUT_MOUSE           = 0
	INPUT_KEYBOARD        = 1
	INPUT_HARDWARE        = 2
	WH_KEYBOARD_LL        = 13
	WH_KEYBOARD           = 2
	WM_KEYDOWN            = 256
	WM_SYSKEYDOWN         = 260
	WM_KEYUP              = 257
	WM_SYSKEYUP           = 261
	WM_KEYFIRST           = 256
	WM_KEYLAST            = 264
	PM_NOREMOVE           = 0x000
	PM_REMOVE             = 0x001
	PM_NOYIELD            = 0x002
	WM_LBUTTONDOWN        = 513
	WM_RBUTTONDOWN        = 516
	NULL                  = 0
	MOUSEEVENTF_RIGHTDOWN = 0x0008
	MOUSEEVENTF_RIGHTUP   = 0x0010
)

type (
	DWORD uint32
	WPARAM uintptr
	LPARAM uintptr
	LRESULT uintptr
	HANDLE uintptr
	HINSTANCE HANDLE
	HHOOK HANDLE
	HWND HANDLE
)

// http://msdn.microsoft.com/en-us/library/windows/desktop/ms646270(v=vs.85).aspx
type INPUT struct {
	Type uint32
	Mi   MOUSEINPUT
	Ki   KEYBDINPUT
	Hi   HARDWAREINPUT
}

// http://msdn.microsoft.com/en-us/library/windows/desktop/ms646273(v=vs.85).aspx
type MOUSEINPUT struct {
	Dx          int32
	Dy          int32
	MouseData   uint32
	DwFlags     uint32
	Time        uint32
	DwExtraInfo uintptr
}

// http://msdn.microsoft.com/en-us/library/windows/desktop/ms646271(v=vs.85).aspx
type KEYBDINPUT struct {
	WVk         uint16
	WScan       uint16
	DwFlags     uint32
	Time        uint32
	DwExtraInfo uintptr
}

// http://msdn.microsoft.com/en-us/library/windows/desktop/ms646269(v=vs.85).aspx
type HARDWAREINPUT struct {
	UMsg    uint32
	WParamL uint16
	WParamH uint16
}

type KbdInput struct {
	typ uint32
	ki  KEYBDINPUT
}

type MouseInput struct {
	typ uint32
	mi  MOUSEINPUT
}

type HardwareInput struct {
	typ uint32
	hi  HARDWAREINPUT
}

type HOOKPROC func(int, WPARAM, LPARAM) LRESULT

type KBDLLHOOKSTRUCT struct {
	VkCode      DWORD
	ScanCode    DWORD
	Flags       DWORD
	Time        DWORD
	DwExtraInfo uintptr
}

// http://msdn.microsoft.com/en-us/library/windows/desktop/dd162805.aspx
type POINT struct {
	X, Y int32
}

// http://msdn.microsoft.com/en-us/library/windows/desktop/ms644958.aspx
type MSG struct {
	Hwnd    HWND
	Message uint32
	WParam  uintptr
	LParam  uintptr
	Time    uint32
	Pt      POINT
}

func SetWindowsHookEx(idHook int, lpfn HOOKPROC, hMod HINSTANCE, dwThreadId DWORD) HHOOK {
	ret, _, _ := procSetWindowsHookEx.Call(
		uintptr(idHook),
		uintptr(syscall.NewCallback(lpfn)),
		uintptr(hMod),
		uintptr(dwThreadId),
	)
	return HHOOK(ret)
}

func CallNextHookEx(hhk HHOOK, nCode int, wParam WPARAM, lParam LPARAM) LRESULT {
	ret, _, _ := procCallNextHookEx.Call(
		uintptr(hhk),
		uintptr(nCode),
		uintptr(wParam),
		uintptr(lParam),
	)
	return LRESULT(ret)
}

func UnhookWindowsHookEx(hhk HHOOK) bool {
	ret, _, _ := procUnhookWindowsHookEx.Call(
		uintptr(hhk),
	)
	return ret != 0
}

func GetMessage(msg *MSG, hwnd HWND, msgFilterMin uint32, msgFilterMax uint32) int {
	ret, _, _ := procGetMessage.Call(
		uintptr(unsafe.Pointer(msg)),
		uintptr(hwnd),
		uintptr(msgFilterMin),
		uintptr(msgFilterMax))
	return int(ret)
}

func TranslateMessage(msg *MSG) bool {
	ret, _, _ := procTranslateMessage.Call(
		uintptr(unsafe.Pointer(msg)))
	return ret != 0
}

func DispatchMessage(msg *MSG) uintptr {
	ret, _, _ := procDispatchMessage.Call(
		uintptr(unsafe.Pointer(msg)))
	return ret
}

func LowLevelKeyboardProc(nCode int, wParam WPARAM, lParam LPARAM) LRESULT {
	ret, _, _ := procLowLevelKeyboard.Call(
		uintptr(nCode),
		uintptr(wParam),
		uintptr(lParam),
	)
	return LRESULT(ret)
}

func Start() {
	// defer user32.Release()
	keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)(func(nCode int, wparam WPARAM, lparam LPARAM) LRESULT {
		if nCode == 0 && wparam == WM_KEYDOWN {
			fmt.Println("key down:")
			kbdstruct := (*KBDLLHOOKSTRUCT)(unsafe.Pointer(lparam))
			code := byte(kbdstruct.VkCode)
			fmt.Printf("%q", code)
		}
		return CallNextHookEx(keyboardHook, nCode, wparam, lparam)
	}), 0, 0)
	var msg MSG
	/*kk
	for GetMessage(&msg, 0, 0, 0) != 0 {
		fmt.Println("in getmessage")
	}
	*/
	for {
		GetMessage(&msg, 0, 0, 0)
		TranslateMessage(&msg)
		DispatchMessage(&msg)
		fmt.Println("key pressed:")
	}
	UnhookWindowsHookEx(keyboardHook)
	keyboardHook = 0
}

func SendInput(inputs []INPUT) uint32 {
	var validInputs []C.INPUT
	for _, oneInput := range inputs {
		input := C.INPUT{_type: C.DWORD(oneInput.Type)}

		switch oneInput.Type {
		case INPUT_MOUSE:
			(*MouseInput)(unsafe.Pointer(&input)).mi = oneInput.Mi
		case INPUT_KEYBOARD:
			(*KbdInput)(unsafe.Pointer(&input)).ki = oneInput.Ki
		case INPUT_HARDWARE:
			(*HardwareInput)(unsafe.Pointer(&input)).hi = oneInput.Hi
		default:
			panic("unkown type")
		}

		validInputs = append(validInputs, input)
	}

	ret, _, _ := procSendInput.Call(
		uintptr(len(validInputs)),
		uintptr(unsafe.Pointer(&validInputs[0])),
		uintptr(unsafe.Sizeof(C.INPUT{})),
	)
	return uint32(ret)
}

func sendK(){
	//INPUT input;
	//WORD vkey = VK_LCONTROL; // see link below
	//input.type = INPUT_KEYBOARD;
	//input.ki.wScan = MapVirtualKey(vkey, MAPVK_VK_TO_VSC);
	//input.ki.time = 0;
	//input.ki.dwExtraInfo = 0;
	//input.ki.wVk = vkey;
	//input.ki.dwFlags = 0; // there is no KEYEVENTF_KEYDOWN
	//SendInput(1, &input, sizeof(INPUT));
	////	input.ki.dwFlags = KEYEVENTF_KEYUP;
	//SendInput(1, &input, sizeof(INPUT));
	var input C.INPUT = C.INPUT{_type: C.DWORD(C.INPUT_KEYBOARD)}
	var vkey uint= C.VK_LCONTROL
	(*KbdInput)(unsafe.Pointer(&input)).ki.WScan = C.MapVirtualKey(vkey, C.MAPVK_VK_TO_VSC)
}

func createInput() {
	var inputs []w32.INPUT
	inputs = append(inputs, w32.INPUT{
		Type: w32.INPUT_KEYBOARD,
		Ki: w32.KEYBDINPUT{
			DwFlags: 0,
		},
	})
	w32.SendInput(inputs)
}

func main() {

	fmt.Println("Start")
	go Start()
	fmt.Println("End")
	for {
	}
}
