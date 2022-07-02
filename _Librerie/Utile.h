#ifndef UTILE_H_INCLUDED
#define UTILE_H_INCLUDED

#include <ctype.h>

void SetBottoni(BOOL stato);
void Reset();
void NascondiImg();

void GeneraPorta();
void GeneraMAC(HWND dove);
void CarattereMaiuscolo(char* s);

void LuceDiretta(int portaIn, int portaOut);
void LuceBroadcast(int portaIn, int* portaOut, int nPortaout);
void AccendiSpegni(int porta);

void Font_Imposta(char* pSNome, LOGFONT* plf, int iDimensione);
void SetFont();
void Stealth(int nascosto);

int FromMACtoByte(char* szmac, int offs, int idx);
int htoi(char* strnum);
int isMACequal(unsigned char* mac1, unsigned char* mac2);
int validateMAC(const char *s);

void Elimina(int indice);

#endif // UTILE_H_INCLUDED
