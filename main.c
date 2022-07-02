#include ".\\_Librerie\\Switch.h"


int WINAPI WinMain(HINSTANCE hinstExe, HINSTANCE hinstPrev, LPSTR lpszCmdLine, int nCmdShow)
{
    ghinstApp = hinstExe;

    DialogBox(hinstExe, MAKEINTRESOURCE(IDD_DIALOGMAIN), NULL, Dlg_Proc);

return(0);
}
