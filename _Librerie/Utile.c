#include "..\\_Librerie\\Switch.h"

/**
*Reset dei componenti, variabili, strutture dati principali
*/
void Reset()
{
    ListBox_ResetContent(hCam[0]);
    ListBox_ResetContent(hCam[1]);
    ListBox_ResetContent(hCam[2]);
    NascondiImg();
    free(pt0);
    free(pf0);
    nF = 0;
    nInserito = 0;
}

/**
*Nasconde le immaggini delle porte dello switch
*/
void NascondiImg()
{
    int i;
    for(i = 0; i < NPORTE; i++)
    {
        ShowWindow(hPorta[i], SW_HIDE);
        UpdateWindow(ghwndApp);
    }
}

/**
*Disattiva o attiva i bottoni in base allo stato on/off dello switch
*/
void SetBottoni(BOOL stato)
{
    int i;

    for(i = 0; i < NGENERA; i++)
        EnableWindow(hGenera[i],stato);

    EnableWindow(hInviaPacc,stato);
    EnableWindow(hLeggiFile,stato);
    EnableWindow(hMostraCam,stato);
    EnableWindow(hCam[0]   ,stato);
    EnableWindow(hCam[1]   ,stato);
    EnableWindow(hCam[2]   ,stato);
    EnableWindow(hNumPorta ,stato);
    EnableWindow(hMACdest  ,stato);
    EnableWindow(hMACsorg  ,stato);
    EnableWindow(hNomeFile ,stato);
    stateBotton = !stato;

    SetWindowText(hNomeFile, "traffic.txt");

    if(stato)
    {
        ListBox_AddString(hCam[0],"TRAFFICO");
        ListBox_AddString(hCam[0],"----------------------------------------------------------------");
        ListBox_AddString(hCam[1],"LEARNING & FORWARDING");
        ListBox_AddString(hCam[1],"--------------------------------------------------------------");
        ListBox_AddString(hCam[2],"FILTERING DATABASE");
        ListBox_AddString(hCam[2],"-----------------------------------------");
    }
    else
    {
        Reset();
    }
}

/**
*Genera un valore casuale che identifica la porta
*/
void GeneraPorta()
{
    char szPorta[1];
    srand(time(NULL));

    int nPorta = rand() %8 + 1;

    itoa(nPorta,szPorta,10);
    SetWindowText(hNumPorta, szPorta);
}

/**
*Genera un indirizzo MAC casuale
*/
void GeneraMAC(HWND dove)
{
    char szMAC[20] = {0};
    int val[6];
    int i;

    for(i = 0; i < 6; i++)
        val[i] = rand() %256;

    sprintf(szMAC, "%02X %02X %02X %02X %02X %02X", val[0] ,val[1] ,val[2] ,val[3] ,val[4] ,val[5]);
    SetWindowText(dove, szMAC);
}

/**
*Verifica la validita' dell'indirizzo MAC
*/
int validateMAC(const char *s)
{
    int i;

    for(i = 0; i < 17; i++)
    {
        if(i % 3 != 2 && !isxdigit(s[i])) ///caratteri
            return 0;

        if(i % 3 == 2 && (s[i] != ':' && s[i] != '-' && s[i] != ' ')) ///i punti che dividono l'indirizzo
            return 0;
    }

    if(s[17] != '\0') ///terminatore al 17 carattere
        return 0;

return 1;
}

/**
*Trasforma ogni carattere della stringa passata in un carattere maiuscolo
*/
void CarattereMaiuscolo(char* s)
{
    int i;
    int lunghezza = strlen(s);

    for(i = 0; i < lunghezza; i++)
        s[i] = toupper(s[i]);
}

/**
*Simula le luci dello switch in modo diretto
*/
void LuceDiretta(int portaIn, int portaOut)
{
    AccendiSpegni(portaIn);
    AccendiSpegni(portaOut);
}

/**
*Simula le luci dello switch quando si verifica un caso di broadcast
*/
void LuceBroadcast(int portaIn, int* portaOut, int nPortaout)
{
    int i;

    AccendiSpegni(portaIn);

    for(i = 0; i < nPortaout; i++)
    {
        ShowWindow(hPorta[portaOut[i]], SW_SHOW);
        UpdateWindow(ghwndApp);
    }
    Sleep(TEMPOLUCE);

    NascondiImg();
}

/**
*Accende e spegne le luci con un tempo di sleep
*/
void AccendiSpegni(int porta)
{
    ShowWindow(hPorta[porta], SW_SHOW);
    UpdateWindow(ghwndApp);
    Sleep(TEMPOLUCE);
    ShowWindow(hPorta[porta], SW_HIDE);
    UpdateWindow(ghwndApp);
}


/**
*Elimino dalla CAM il dato che ha superato il tempo limite
*/
void Elimina(int indice)
{
    int i;

    if(nF == 1)
    {
        free(pf0);
        nF--;
        return;
    }

    for(i = indice+1; i < nF; i++)
        pf0[i-1] = pf0[i];

    nF--;
    pf0 = realloc(pf0,(nF)*sizeof(*pf0));
}


/**
*Trasforma una stringa contenente un indirizzo MAC in un valore intero
*/
int FromMACtoByte(char* szmac, int offs, int idx)
{
    char szByte[3];

        szByte[0] = szmac[offs+idx*3];
        szByte[1] = szmac[offs+idx*3+1];
        szByte[2] = 0;

return htoi(szByte);
}

/**
*Da valore esadecimale a valore intero
*/
int htoi(char* strnum)
{
    int value = 0;
    char *p;

    p = strnum;
    while(*p)
    {
        if (*p >= '0' && *p <= '9') value = value * 16 + *p - '0';
        else value = value * 16 + *p - 'A' + 10;
        p++;
    }
return value;
}

/**
*Verifica se due indirizzi MAC sono uguali
*/
int isMACequal(unsigned char* mac1, unsigned char* mac2)
{
    int i;

    for (i = 0; i < 6; i++)
    {
        if(*mac1 != *mac2) return 0;
        mac1++; mac2++;
    }
return 1;
}

/**
*Set dei vari font della dialog
*/
void Font_Imposta(char* pSNome, LOGFONT* plf, int iDimensione)
{
    plf->lfHeight		      =-iDimensione;
    plf->lfWidth		      =0;
    plf->lfEscapement	      =0;
    plf->lfOrientation	      =0;
    plf->lfWeight		      =FW_NORMAL;
    plf->lfItalic		      =0;
    plf->lfUnderline	      =0;
    plf->lfStrikeOut	      =0;
    plf->lfCharSet		      =DEFAULT_CHARSET;
    plf->lfOutPrecision	      =OUT_DEFAULT_PRECIS;
    plf->lfClipPrecision      =CLIP_DEFAULT_PRECIS;
    plf->lfQuality		      =DEFAULT_QUALITY;
    plf->lfPitchAndFamily     =DEFAULT_PITCH;
    strcpy(plf->lfFaceName,pSNome);
}

void SetFont()
{
    LOGFONT	lf;
    HFONT ghFont;

    Font_Imposta("Arial",&lf,16);
    ghFont = CreateFontIndirect(&lf);
    SetWindowFont(hNumPorta,ghFont,TRUE);
    SetWindowFont(hMACdest ,ghFont,TRUE);
    SetWindowFont(hMACsorg ,ghFont,TRUE);
    SetWindowFont(hNomeFile,ghFont,TRUE);
    SetWindowFont(hLettura ,ghFont,TRUE);

    Font_Imposta("Arial",&lf,14);
    ghFont = CreateFontIndirect(&lf);
    SetWindowFont(hCam[1],ghFont,TRUE);
    SetWindowFont(hCam[2]  ,ghFont,TRUE);

    Font_Imposta("Arial",&lf,15);
    ghFont = CreateFontIndirect(&lf);
    SetWindowFont(hCam[0]  ,ghFont,TRUE);
}

/**
*Nascondo il prompt
*/
void Stealth(int nascosto)
{
    //ShowWindow (GetConsoleWindow(), SW_HIDE);
    if(nascosto == 1)
        ShowWindow(FindWindowA("ConsoleWindowClass", NULL), SW_HIDE);

    else
    {
        ShowWindow(FindWindowA("ConsoleWindowClass", NULL), SW_SHOW);
        //EndDialog (FindWindowA("ConsoleWindowClass", NULL), 0);
        //DestroyWindow(FindWindowA("ConsoleWindowClass",NULL));
    }
}
