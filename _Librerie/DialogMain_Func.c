#include "..\\_Librerie\\Switch.h"


BOOL CALLBACK Dlg_Proc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		HANDLE_MSG(hDlg, WM_INITDIALOG, Dlg_OnInitDialog);
		HANDLE_MSG(hDlg, WM_COMMAND, Dlg_OnCommand);
		HANDLE_MSG(hDlg, WM_CLOSE, Dlg_OnClose);

        default:  break;
	}
return FALSE;
}

BOOL Dlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
    int i;

    ///Nascondo il prompt
    //Stealth(1);

    ///DIALOG MAIN
    ghwndApp = hwnd;
    SetClassLong(hwnd, GCL_HICON, (LONG) LoadIcon(ghinstApp, MAKEINTRESOURCE(IDI_ICON)));

    ///IMMAGINI
    hSwitch = GetDlgItem(hwnd, IDC_SWITCH);

    ///PORTE DELLO SWITCH
    for(i = 0; i < NPORTE; i++)
        hPorta[i] = GetDlgItem(hwnd, (40000+i));

    ///BOTTONI DEL GENERA
    for(i = 0; i < NGENERA; i++)
        hGenera[i] = GetDlgItem(hwnd, (40012+i));

    ///BOTTONI
    hMostraCam = GetDlgItem(hwnd, IDC_MOSTRACAM);
    hInviaPacc = GetDlgItem(hwnd, IDC_INVIAPACC);
    hLeggiFile = GetDlgItem(hwnd, IDC_LEGGIFILE);
    hPower     = GetDlgItem(hwnd, IDC_POWER);

    ///EDIT BOX DELLA CAM
    hCam[0]    = GetDlgItem(hwnd, IDC_CAM);
    hCam[1]    = GetDlgItem(hwnd, IDC_CAM2);
    hCam[2]    = GetDlgItem(hwnd, IDC_CAM3);

    ///EDIT BOX UTILI
    hNumPorta  = GetDlgItem(hwnd, IDC_NPORTA);
    hMACdest   = GetDlgItem(hwnd, IDC_MACDEST);
    hMACsorg   = GetDlgItem(hwnd, IDC_MACSORG);
    hNomeFile  = GetDlgItem(hwnd, IDC_NOMEFILE);

    ///STATIC PER LA VERIFICA DELLA LETTURA
    hLettura   = GetDlgItem(hwnd, IDC_STATICLETTURA);
    ShowWindow(hLettura, 0);

    ///RANDOM
    srand(time(NULL));

    ///AZZERO TUTTO
    Reset();

    ///VARI SETTAGGI
    SetBottoni(FALSE);
    SetFont();
return TRUE;
}

void Dlg_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    switch(id)
    {
        case IDC_POWER:
            SetBottoni(stateBotton);
        break;

        case IDC_MOSTRACAM:
            CAM();
        break;

        case IDC_LEGGIFILE:
        {
            char szInputText[20];
            char szNomeFile[50];
            GetWindowText(hNomeFile, szInputText, 20);
            sprintf(szNomeFile, ".\\_Traffico\\%s", szInputText);

            LeggiPckDaFile(szNomeFile);
        }
        break;

        case IDC_GENPORTA:
            GeneraPorta();
        break;

        case IDC_GENDEST:
            GeneraMAC(hMACdest);
        break;

        case IDC_GENSORG:
            GeneraMAC(hMACsorg);
        break;

        case IDC_INVIAPACC:
            InvioPacchetto();
        break;
    }
}

void Dlg_OnClose(HWND hwnd)
{
    //if (MessageBox(hwnd, "Vuoi uscire?", "Sicuro?", MB_OKCANCEL) == IDOK)
    Stealth(0);
    EndDialog(hwnd,0);
}
