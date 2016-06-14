
#include "stdafx.h"
#include "Win32RBT.h"
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <math.h>

#define MAX_LOADSTRING 100
#define SECOND_TIMER 400

// Variabile globale
HINSTANCE hInst;								
TCHAR szTitle[MAX_LOADSTRING];					// Titlul ferestrei
TCHAR szWindowClass[MAX_LOADSTRING];			// Numele clasei ferestrei principale

// Declaratiile functiilor
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
int k=0, f = 0;
int mat[100][10];
char num[100][100];
struct nod
{
	char culoare;
	int val;
	nod *st,*dr,*par;
};
struct tree
{
	nod * rad;
};
tree *t;
nod *make_x(int x)
{
	nod *p;
	p=(nod *)malloc(sizeof(nod));
	p->val=x;
	p->st=NULL;
	p->dr=NULL;
	p->par=NULL;
	return p;
}
void rotire_stanga(tree *t,nod *x)
{
	nod * y=(nod*)malloc(sizeof(nod));
	y=x->dr;
	x->dr=y->st;
	if(y->st!=NULL)
		y->st->par=x;
	y->par=x->par;
	if(x->par==NULL)
		t->rad=y;
	else
		if(x==x->par->st)
			x->par->st=y;
		else
			x->par->dr=y;
	y->st=x;
	x->par=y;
}
void rotire_dreapta(tree *t , nod *x)
{
	nod *y=(nod*)malloc(sizeof(nod));
	y=x->st;
	x->st=y->dr;
	if(y->dr!=NULL)
		y->dr->par=x;
	y->par=x->par;
	if(x->par==NULL)
		t->rad=y;
	else
		if(x==x->par->st)
			x->par->st=y;
		else
			x->par->dr=y;
	y->dr=x;
	x->par=y;
}
void inserare_in(tree *t,nod *x,HWND hwnd)
{
	if(t->rad==NULL)
	{
		x->par=NULL;
		t->rad=x;
	}
	else
	{
		nod*k=NULL;
		nod*p=t->rad;
		while(p!=NULL)
		{
			k=p;
			if(x->val<p->val)
				p=p->st;
			else
				if(x->val>p->val)
					p=p->dr;
		}
		x->par=k;
		if(x->val<k->val)
				k->st=x;
			else
				k->dr=x;
	}
}
void inserare(tree *t,nod *x,HWND hwnd)
{
	inserare_in(t,x,hwnd);
	x->culoare='R';
	nod *y;
	while(x!=t->rad && x->par->culoare=='R')
	{
		if(x->par==x->par->par->st) // parintele este fiu de stanga
		{
			y=x->par->par->dr;
			if(y!=NULL)
			{
				if(y->culoare=='R')
				{
					//cazul 1
					::MessageBox(hwnd,"cazul 1 -- schimbare culori","INFO",MB_OK|MB_ICONEXCLAMATION);
					x->par->culoare='N';
					y->culoare='N';
					x->par->par->culoare='R';
					x=x->par->par;
				}
			}
			else
				if(y==NULL || y->culoare=='N')
			{
				if(x==x->par->dr)
				{
					//cazul 2
					::MessageBox(hwnd,"cazul 2 -- rotire stanga","INFO",MB_OK|MB_ICONEXCLAMATION);
					x=x->par;
					rotire_stanga(t,x);
				}
				//cazul 3
				::MessageBox(hwnd,"cazul 3 -- schimba culoare 2 parinti + rotire dreapta","INFO",MB_OK|MB_ICONEXCLAMATION);
				x->par->culoare='N';
				x->par->par->culoare='R';
				rotire_dreapta(t,x->par->par);
			}
		}
		else
		{
			y=x->par->par->st;
			if(y!=NULL)
			{
				if(y->culoare=='R')
				{
					::MessageBox(hwnd,"cazul 1 -- schimbare culoare","INFO",MB_OK|MB_ICONEXCLAMATION);
					x->par->culoare='N';
					y->culoare='N';
					x->par->par->culoare='R';
					x=x->par->par;
				}
			}
			else
			{
			if(y==NULL || y->culoare=='N')
			{
				if(x==x->par->st)
				{
					x=x->par;
					::MessageBox(hwnd,"cazul 2 -- rotire dreapta","INFO",MB_OK|MB_ICONEXCLAMATION);
					rotire_dreapta(t,x);
				}
				::MessageBox(hwnd,"cazul 3 -- schimbare culoare 2 parinti + rotire stanga","INFO",MB_OK|MB_ICONEXCLAMATION);
				x->par->culoare='N';
				x->par->par->culoare='R';
				rotire_stanga(t,x->par->par);
			}
			}
		}
	}
	t->rad->culoare='N';
}
int adancime(nod*x,int r)
{
	if(x==NULL)
		return r;
	if(adancime(x->st,r+1)>=adancime(x->dr,r+1))
		return adancime(x->st,r+1);
	else
		return adancime(x->dr,r+1);
}
void afisare(nod *a)
{
	if(a==NULL)
		printf("FRUNZA - NEGRU\n");
	else
	{
		printf("%d -",a->val);
		if(a->culoare=='R')
			printf("ROSU\n");
		else
			printf("NEGRU\n");
		afisare(a->st);
		afisare(a->dr);
	}
}
void eliberare(tree *t,nod *p)
{
	if(p!=NULL)
	{
		eliberare(t,p->st);
		eliberare(t,p->dr);
		p=NULL;
		free(p);
	}
}

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MSG msg;
	HACCEL hAccelTable;
	
	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WIN32RBT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Initializam aplicatia
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32RBT));
	t=(tree*)malloc(sizeof(tree));
	t->rad=NULL;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}


//Inregistreaza Clasa

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32RBT));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_WIN32RBT);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}


//Se afiseaza fereastra principala

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; 

   hWnd = CreateWindow(szWindowClass, "Creare Arbore", WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);


   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void desen(HWND hwnd,tree *t,nod *p,int h,int l,int &k,int start1,int start2)
{
	int dist=pow((long double)2,h+1)-2;
	int fin=dist/2*30;
	int x1,x2,y1,y2;
	int i;
	char *text;
	if(p!=NULL)
	{
		
		if(p->par==NULL)
		{
			mat[k][0]=p->val;
			mat[k][1]=start1;
			mat[k][2]=50;
			mat[k][3]=start2;
			mat[k][4]=100;
			if(p->culoare=='R')
				mat[k][5]=1;
			else
				mat[k][5]=0;
			k++;
		}
		else
			if(p==p->par->dr)
			{
				mat[k][0]=p->val;
				mat[k][1]=start1+fin;
				start1=start1+fin;
				mat[k][2]=50+100*l;
				mat[k][3]=start2+fin;
				start2=start2+fin;
				mat[k][4]=100+100*l;
				if(p->culoare=='R')
					mat[k][5]=1;
				else
					mat[k][5]=0;
				for(i=0;i<k;i++)
					if(p->par->val==mat[i][0])
						break;
				mat[k][6]=mat[i][1]+25;
				mat[k][7]=mat[i][4];
				k++;
			}
			else
			{
				mat[k][0]=p->val;
				mat[k][1]=start1-fin;
				start1=start1-fin;
				mat[k][2]=50+100*l;
				mat[k][3]=start2-fin;
				start2=start2-fin;
				mat[k][4]=100+100*l;
				if(p->culoare=='R')
					mat[k][5]=1;
				else
					mat[k][5]=0;
				for(i=0;i<k;i++)
					if(p->par->val==mat[i][0])
						break;
				mat[k][6]=mat[i][1]+25;
				mat[k][7]=mat[i][4];
				k++;
			}
		desen(hwnd,t,p->st,h-1,l+1,k,start1,start2);
		desen(hwnd,t,p->dr,h-1,l+1,k,start1,start2);
	}
}
void afisare (char *sir,HWND hWnd)
{
	FILE *f;
	int x,nr=0,i;
	int a=strlen(sir);
	if(a!=NULL)
	{
		x=atoi(sir);
		BOOL ver=true;
		for(i=0;i<k;i++)
			if(mat[i][0]==x)
				ver=false;
		if(ver)
		{
			inserare(t,make_x(x),hWnd);
			desen(hWnd,t,t->rad,adancime(t->rad,0),0,nr,650,700);
			k=nr;
		}
		else
				{
					::MessageBox(hWnd,"Introduceti alta valoare!","Nod Existent",MB_OK|MB_ICONEXCLAMATION);
				}

	}
	else
		::MessageBox(hWnd, _T("Introduceti o valoare valida!"), _T("NOD NULL"), MB_OK|MB_ICONEXCLAMATION);

}

//
//  WM_COMMAND	- Meniul aplicatiei
//  WM_PAINT	- Coloreaza fereastra principala
//  WM_DESTROY	- afiseaza un mesaj de inchidere
//
//

HWND textbox,buton,reinit,info;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	
	switch (message)
	{
	case WM_CREATE:
		textbox=CreateWindow("EDIT","",WS_CHILD|WS_VISIBLE|WS_BORDER,10,10,120,30,hWnd,NULL,NULL,NULL);
		buton=CreateWindow("BUTTON","Adaugare",WS_CHILD|WS_VISIBLE|WS_BORDER,140,10,80,30,hWnd,(HMENU)1,NULL,NULL);
		reinit=CreateWindow("BUTTON","Resetare",WS_CHILD|WS_VISIBLE|WS_BORDER,10,60,80,30,hWnd,(HMENU)2,NULL,NULL);
		info=CreateWindow("BUTTON","Informatii",WS_CHILD|WS_VISIBLE|WS_BORDER,140,60,80,30,hWnd,(HMENU)5,NULL,NULL);
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Optiunile din meniu:
		switch (wmId)
		{
		case 1:			
			LPCSTR buffer[256];
			SendMessage(textbox,	WM_GETTEXT,sizeof(buffer) / sizeof(buffer[0]),reinterpret_cast<LPARAM>(buffer));
			afisare((char *)buffer,hWnd);
			DestroyWindow(textbox);
			textbox=CreateWindow("EDIT","",WS_CHILD|WS_VISIBLE|WS_BORDER,10,10,120,30,hWnd,NULL,NULL,NULL);
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case 2:
			{
				int msgboxID = MessageBox( NULL,"Ati selectat stergerea arborelui.\nDoriti sa continuati?","Confirmare",
					MB_ICONWARNING | MB_YESNO | MB_DEFBUTTON2);
				switch (msgboxID)
				{
				case IDYES:
					for(int i=0;i<k;i++)
						for(int j=0;j<10;j++)
							mat[i][j]=0;
					eliberare(t,t->rad);
					t=NULL;
					t=(tree*)malloc(sizeof(tree));
					t->rad=NULL;
				break;
				}
			}
			break;
		case 5:
			MessageBox(NULL,"REGULI DE INSERARE : \n\nOrice nod este rosu sau negru.\nRadacina trebuie sa fie neaparat neagra.\nFiecare frunza este neagra.\nDaca un nod este rosu, atunci ambii fii ai sai sunt negri.\nPentru orice subarbore pornind de la acelasi nod, numarul de noduri negre este acelasi.","INFO",MB_OK|MB_ICONINFORMATION);
			break;
		}
		break;
	case WM_PAINT:
		int i;
		hdc = BeginPaint(hWnd, &ps);
		char *tex;
		tex=(char*)malloc(10*sizeof(char));
		for(i=0;i<k;i++)
		{
		
		// Se coloreaza:
			HPEN hLinePen,hPenOld;
					
			COLORREF qLineColor;
			wsprintf(tex,"%d", mat[i][0] );
			TextOut(hdc, mat[i][1]+15,mat[i][2]+15,tex,strlen(tex));
			if(mat[i][5]==1)
				qLineColor = RGB(250,10 ,10);
			else
				qLineColor = RGB(0,0 ,0);
			hLinePen = CreatePen(PS_SOLID, 2, qLineColor);
			hPenOld = (HPEN)SelectObject(hdc, hLinePen);
				Arc(hdc,mat[i][1],mat[i][2],mat[i][3],mat[i][4],0,0,0,0);
			SelectObject(hdc, hPenOld);
			DeleteObject(hLinePen);
			if(mat[i][6]!=0)
			{
				qLineColor = RGB(0,0 ,0);
				hLinePen = CreatePen(PS_SOLID, 2, qLineColor);
				hPenOld = (HPEN)SelectObject(hdc, hLinePen);
				MoveToEx(hdc, mat[i][6], mat[i][7], NULL);
				LineTo(hdc, mat[i][1]+25, mat[i][2]);
				SelectObject(hdc, hPenOld);
				DeleteObject(hLinePen);
			}
		}
		EndPaint(hWnd, &ps);
		SetTimer(hWnd,SECOND_TIMER,SECOND_TIMER,NULL);
		break;
	case WM_TIMER:
		if(wParam== SECOND_TIMER)
			InvalidateRect(hWnd,NULL,TRUE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
LPCTSTR text;
switch(wParam){
case VK_BACK: text="Ati apasat BackSpace"; break;
case VK_TAB: text="Ati apasat Tab"; break;
case VK_LEFT: text="Ati apasat SageataStinga"; break;
case VK_RIGHT: text="Ati apasat SageataDreapta"; break;
case VK_UP: text="Ati apasat SageataSus"; break;
case VK_DOWN: text="Ati apasat SageataJos"; break;
case VK_RETURN: text="Ati apasat Enter"; break;
case VK_ESCAPE: text="Ati apasat Escape"; break;
case VK_CONTROL: text="Ati apasat Control"; break;
default: text="Ati apasat alta tasta";
}
MessageBox(hWnd, text, "Mesaj",MB_OK|MB_ICONEXCLAMATION);
return DefWindowProc(hWnd, message, wParam, lParam);
break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Fereastra About
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
