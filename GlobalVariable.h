#pragma once

extern int BtnNum;
extern TCHAR szBuffer[MAX_PATH];
extern HWND hwnd;
extern HWND hStatic2;
extern HWND hStatic4;
extern HWND hStatic5;
extern HWND hStatic6;
extern HWND hStatic7;
extern HWND hStatic8;
extern HWND hComboBox;
extern RECT rect;
extern HPEN hpen;
extern void DrawLine(HWND hWnd, double* num, int n);
extern double* gnum;
extern double* gnum1;
extern double* rnum;      //ʵ��
extern double* lnum;      //����
extern double* pnum;      //��λ
extern double* anum;      //��ֵ
extern int gn;
extern int gn1;
extern POINT* aptt;
extern POINT* apt;
extern POINT* apts;
extern int CanvasStatus;