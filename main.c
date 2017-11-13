#include "hello_fn.h"
#include <windows.h>

int main (void) {
    Sleep(5000);
    down(VK_LCONTROL);
    press('J');
    press(VK_LCONTROL);
  return 0;
}
