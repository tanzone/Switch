#ifndef SWITCH_H_INCLUDED
#define SWITCH_H_INCLUDED

#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <windows.h>
#include <windowsx.h>
#include <time.h>
#include <stdio.h>
#include "..\\_Resource\\resource.h"
#include ".\\Utile.h"

#define NPORTE    (8)
#define NGENERA   (3)
#define TEMPOLUCE (400)
#define TEMPOCAM  (15000)

///STRUTTURE DI DATI
typedef struct
{
  int porta;
  unsigned char MACd[6];
  unsigned char MACs[6];
} TRAFFICO; TRAFFICO* pt0; TRAFFICO* pt;

typedef struct
{
  int porta;
  unsigned char MAC[6];
  int age;
} FILTERINGDB; FILTERINGDB* pf0; FILTERINGDB* pf;

int nF, nInserito;

///DIALOG
HINSTANCE ghinstApp;
HWND ghwndApp;

///IMMAGINI
HWND hSwitch;
HWND hPorta[8];

///BOTTONI
HWND hMostraCam;
HWND hGenera[3];
HWND hInviaPacc;
HWND hLeggiFile;
HWND hPower;

///EDIT BOX
HWND hCam[3];
HWND hNumPorta;
HWND hMACdest;
HWND hMACsorg;
HWND hNomeFile;

///STATIC
HWND hLettura;

BOOL stateBotton;

///Prototipi dialogMain
BOOL CALLBACK Dlg_Proc (HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam); ///Window procedur
BOOL Dlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
void Dlg_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void Dlg_OnClose(HWND hwnd);

///Prototipi PROF
void LeggiPckDaFile(char* nomeFile);
void SwitchUnoAllaVolta(TRAFFICO* unPkt);
void CAM(void);

///INSERIMENTO PACCHETTO DA PARTE DELL'UTENTE
void InvioPacchetto();

///ELABORAZIONE DEI PACCHETTI DATI
void AddPacchetto(char* szRiga);
void AnalisiPacchetto(int i);

#endif // SWITCH_H_INCLUDED
