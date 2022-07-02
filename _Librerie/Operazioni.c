#include "..\\_Librerie\\Switch.h"

/**
*Legge il file e costruisci le strutture di dati, stampa il contenuto del file
*/
void LeggiPckDaFile(char* nomeFile)
{
    FILE *fp;
    char szRiga[80];

    fp = fopen(nomeFile, "r");
    if(fp == NULL)
    {
        SetWindowText(hLettura, "Lettura del file fallita!");
        ShowWindow(hLettura, 1);
    }
    else
    {
        SetWindowText(hLettura, "File Corretto!");
        ShowWindow(hLettura, 1);
        fgets(szRiga, 80, fp);
        while(fgets(szRiga, 80, fp) != NULL)
        {
            CarattereMaiuscolo(szRiga);
            ListBox_AddString(hCam[0], szRiga);
            if(szRiga[0])
                AddPacchetto(szRiga);
        }
        fclose (fp);
    }
}

/**
*Aggiumge il pacchetto letto alla struttura dati
*/
void AddPacchetto(char* szRiga)
{
    int j;

    if(nInserito == 0)
    {
        pt0 = malloc(sizeof(TRAFFICO));
        pt = pt0;
    }
    else
    {
        pt0 = realloc(pt0,(nInserito+1)*sizeof(*pt0));
        pt = pt0 + nInserito;
    }

    pt->porta = szRiga[0]-'0';
    for(j = 0; j < 6; j++)
    {
        pt->MACd[j] = (unsigned char)FromMACtoByte(szRiga,2,j);
        pt->MACs[j] = (unsigned char)FromMACtoByte(szRiga,20,j);
    }
    AnalisiPacchetto(nInserito);
    nInserito++;
}

/**
*Prototipo che analizza un pacchetto alla volta
*/
void AnalisiPacchetto(int i)
{
    TRAFFICO unPkt;
    int j;

    // Un pacchetto alla volta
    unPkt.porta = (pt0+i)->porta;
    for (j = 0; j < 6; j++)
    {
      unPkt.MACd[j] = (pt0+i)->MACd[j];
      unPkt.MACs[j] = (pt0+i)->MACs[j];
    }
    // Lo Switch analizza pacchetti
    SwitchUnoAllaVolta(&unPkt);
}

/**
*Learning & forwarding
*/
void SwitchUnoAllaVolta(TRAFFICO* unPkt)
{
    char szOutput[100] = {0};
    int lucePorta[8]   = {9};
    int nPortaOut      = 0;
    int trovato        = 0;
    int i;

    for(i = 0; i < nF; i++)
      if((pf0+i)->porta == (unPkt)->porta)
        if(isMACequal((pf0+i)->MAC,(unPkt)->MACs))
        {
          trovato = 1;
          break;
        }

    if(!trovato)
    {
        if(nF == 0)
        {
            pf0 = malloc(sizeof(*pf0));
            pf = pf0;
        }
        else
        {
            pf0 = realloc(pf0,(nF+1)*sizeof(*pf0));
            pf = pf0+nF;
        }
        nF++;

        pf->porta = (unPkt)->porta;
        pf->age   = (int)GetTickCount();
        memcpy(pf->MAC,(unPkt)->MACs,sizeof(pf->MAC));
    }

    trovato = 0;
    ListBox_ResetContent(hCam[1]);
    ListBox_AddString(hCam[1],"LEARNING & FORWARDING");
    ListBox_AddString(hCam[1],"--------------------------------------------------------------");
    for(i = 0; i < nF; i++)
        if(isMACequal((pf0+i)->MAC,(unPkt)->MACd))
        {
            sprintf(szOutput,"Trama ricevuta su %d -> %d",(unPkt)->porta,(pf0+i)->porta);
            ListBox_AddString(hCam[1], szOutput);
            LuceDiretta((unPkt)->porta-1, (pf0+i)->porta-1);
            trovato = 1;
        }

    if(!trovato)
    {
        sprintf(szOutput,"Trama ricevuta su %d -> broadcast su ",(unPkt)->porta);
        ListBox_AddString(hCam[1], szOutput);
        for(i = 1; i < 9; i++)
            if(i != (unPkt)->porta)
            {
                lucePorta[i-1] = i-1;
                nPortaOut++;
                sprintf(szOutput,"%d ",i);
                ListBox_AddString(hCam[1], szOutput);
            }
        LuceBroadcast((unPkt)->porta-1, lucePorta, nPortaOut+1);
    }
    CAM();
}

/**
*Controllo se i dati inseriti sono validi e quindi lavorarli
*/
void InvioPacchetto()
{
    char porta   [100] = {0};
    char MacDest [100] = {0};
    char MacSorg [100] = {0};
    char szOutput[100] = {0};
    char szRiga  [100] = {0};

    GetWindowText(hNumPorta,porta  ,90);
    GetWindowText(hMACdest ,MacDest,90);
    GetWindowText(hMACsorg ,MacSorg,90);

    if(validateMAC(MacDest) && validateMAC(MacSorg) && porta[0] < '9' && porta[0] > '0')
    {
        SetWindowText(hNumPorta, ""); SetWindowText(hMACdest, ""); SetWindowText(hMACsorg, "");
        sprintf(szOutput, "%c;%s;%s\n", porta[0], MacDest, MacSorg);
        CarattereMaiuscolo(szOutput);
        ListBox_AddString(hCam[0], szOutput);

        ListBox_GetText(hCam[0], nInserito+2, szRiga);
        AddPacchetto(szRiga);
    }
    else
        MessageBox(NULL,"CONTENUTO DEL PACCHETTO INVALIDO!","Warning",MB_OK);
}

/**
*Visualizzazione della CAM dello switch
*/
void CAM(void)
{
    int i;
    int tempoTrascorso = 0;
    int tempoAttuale   = 0;
    char szOutput[50]  = {0};

    ListBox_ResetContent(hCam[2]);
    ListBox_AddString(hCam[2],"FILTERING DATABASE");
    ListBox_AddString(hCam[2],"-----------------------------------------");

    tempoAttuale = (int)GetTickCount();
    for (i = 0;i < nF; i++)
    {
        tempoTrascorso = tempoAttuale - (pf0+i)->age;
        if(tempoTrascorso >= TEMPOCAM)
            Elimina(i);

        if(nF != 0)
        {
            sprintf(szOutput,"\nP: %d MAC: %02X %02X %02X %02X %02X %02X",
                    (pf0+i)->porta,(pf0+i)->MAC[0],(pf0+i)->MAC[1],(pf0+i)->MAC[2],
                    (pf0+i)->MAC[3],(pf0+i)->MAC[4],(pf0+i)->MAC[5]);
            ListBox_AddString(hCam[2], szOutput);
        }

    }
}

