
/*------------------------Editeur de courbes-------------------------------------*/

#include <windows.h>
#include <stdio.h>
#include "resource.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>
#define carre(x) ((x)*(x))  // On définit le type carre qui calcul le carre.

typedef struct point{
	float x;
	float y;
	float w;
	struct point *suivant;
}
point;

typedef struct point *liste;

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

TCHAR szAppName[] = TEXT ("cours") ; // Nom du menu

point* vertex_attr; //liste vertex_attr;

int d;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
     HWND     hwnd ;
     MSG      msg ;
     WNDCLASS wndclass ;
     
     wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
     wndclass.lpfnWndProc   = WndProc ;
     wndclass.cbClsExtra    = 0 ;
     wndclass.cbWndExtra    = 0 ;
     wndclass.hInstance     = hInstance ;
     wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
     wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
     wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
     wndclass.lpszMenuName  = MAKEINTRESOURCE(IDR_MENU1) ;
     wndclass.lpszClassName = szAppName ;

     if (!RegisterClass (&wndclass))
     {
          MessageBox (NULL, TEXT ("Ce programme fonctionne exclusivement sous Windows NT!"),
                      szAppName, MB_ICONERROR) ;
          return 0 ;
     }
     
     hwnd = CreateWindow (szAppName, TEXT ("Editeur de courbes"),
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          NULL, NULL, hInstance, NULL) ;
     
     ShowWindow (hwnd, iCmdShow) ;
     UpdateWindow (hwnd) ;
     
     while (GetMessage (&msg, NULL, 0, 0))
     {
          TranslateMessage (&msg) ;
          DispatchMessage (&msg) ;
     }
     return msg.wParam ;
}


BOOL CALLBACK AttrProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
	switch (msg) 
	    {
		case WM_INITDIALOG :
			SetDlgItemInt(hwnd, IDC_POIDS, (int)vertex_attr->w, TRUE);
			SetDlgItemInt(hwnd, IDC_X, (int)vertex_attr->x, TRUE);
			SetDlgItemInt(hwnd, IDC_Y, (int)vertex_attr->y, TRUE);
			return 0;
		case WM_COMMAND :
			switch LOWORD(wParam)  {

				case IDOK :
					vertex_attr->w = (float) GetDlgItemInt(hwnd, IDC_POIDS, NULL, TRUE);
					vertex_attr->x = (float) GetDlgItemInt(hwnd, IDC_X, NULL, TRUE);
					vertex_attr->y = (float) GetDlgItemInt(hwnd, IDC_Y, NULL, TRUE);
					EndDialog(hwnd, 1);
					return 0;

				case IDCANCEL :
					EndDialog(hwnd, 0);
					return 0;
			}
			return 0;

	}
	return 0;
}


/*------------------------Crée Liste-------------------------------------*/

void creer_liste(liste *tete, float X, float Y)
{
	liste p;
	p=*tete;
	if(p==NULL)
	{
		p=(point*)malloc(sizeof(point));
		p->x=X;
		p->y=Y;
		p->w=1.0;
		p->suivant=NULL;
		*tete=p;
	}
	else
	{
		p=*tete;
		while (p->suivant != NULL)
			p=p->suivant;
		p->suivant=(point*)malloc(sizeof(point));
		p=p->suivant;
		p->x=X;
		p->y=Y;
		p->w=1.0;
		p->suivant=NULL;
	}
}


/*------------------------designer_point-------------------------------------*/
float dist(float x1, float y1, float x2, float y2)
{
	return sqrt (carre(x1 - x2) + carre(y1 - y2));
}
liste designer_point(liste pt, float X, float Y)
{
	float d, dmin;
	liste designe, courant;
	if (pt!= NULL)
	{
		courant=pt->suivant;
		designe=courant;
		dmin=dist(pt->x,pt->y,X,Y);
		while(courant!=NULL)
		{
			d=dist(courant->x,courant->y,X,Y);
			if(d<dmin)
			{
				dmin=d;
				designe=courant;
			}
			courant=courant->suivant;
		}
		return (designe);
	}
	else return (NULL);
}


/*------------------------SUPPRIME-------------------------------------*/
void supprime(liste *pt, liste designe){
	liste courant = *pt;
	if (courant==designe){
		courant=(*courant).suivant;
		free(designe);
		*pt=courant;
	}
	else{
		while((*courant).suivant!=designe){
			courant = (*courant).suivant;
		}
		(*courant).suivant = (*designe).suivant;
		free(designe);
	}
}
/*------------------------Effacer-------------------------------------*/
void effacer (liste *pt)
{
    liste courant;
    courant=*pt;
    while (courant!=NULL)
    {
    *pt=(*courant).suivant;
    free(courant);
    courant = *pt;
    }
}
/*------------------------Ajouter-------------------------------------*/
void ajouter(liste designe, float X, float Y){
	liste courant;
	courant=(point*)malloc(sizeof(point));
	courant->x=X;
	courant->y=Y;
	courant->w=1;
	courant->suivant=designe->suivant;
	designe->suivant=courant;
}

/*------------------------DECASTELJAU-------------------------------------*/

point decasteljau(liste pt, float u){
	int k=0;
	point T[20];
	liste courant=pt;
	while(courant != NULL){
		T[k].x=(*courant).x*(*courant).w;
		T[k].y=(*courant).y*(*courant).w;
		T[k].w=(*courant).w;
		k=k+1;
		courant=(*courant).suivant;
	}
	int n=k-1;
	for (int i=1; i<n; i++){
		for (int j=0; j<=n; j++){
			T[j].x=(1-u)*T[j].x+u*T[j+1].x;
			T[j].y=(1-u)*T[j].y+u*T[j+1].y;
			T[j].w=(1-u)*T[j].w+u*T[j+1].w;
		}
	}
	return T[0];
}

/*------------------------Courbe de Bésier-------------------------------------*/

void calc_bezier(liste pt, HDC hdc){
	float u=0.01;
	float pas=u;
	point p;
	MoveToEx(hdc,(int)pt->x,(int)pt->y,NULL); //pour positionner le curseur
	while (u<=1){
		p=decasteljau(pt,u);
		p.x=p.x/p.w;
		p.y=p.y/p.w;

		LineTo(hdc,(int)p.x,(int)p.y);
		u=u+pas;
	}
}


/*------------------------Séquence modale-------------------------------------*/

void seq_noeud(float U[], int n, int k ) {
    int i;
    for (i = 0; i <= k - 1; i++){
        U[i] = 0;
    }
    for (i = n + 1; i <= n + k; i++) {
        U[i] = 1;
    }

    for (i = k; i <= n + k; i++) {
        U[i] = ((i - k) + 1.0) /(n - k + 2);
    }
}
/*------------------------Calcul_r-------------------------------------*/
int Calc_r(float U[], float u, int n, int k) {
    int i;
    for (i = k - 1; i <= n; i++) {
        if (u >= U[i] && u <= U[i + 1]) {
            return(i);
        }
    }
}


/*------------------------Bsplin-------------------------------------*/
point DeBoorCox(liste pt, float U[], int n, int k, float u) {
    int i = 0, j, r;
    point tab[30];
    liste courant = pt;
    while (courant != NULL) {
        tab[i].x = courant->x * courant->w;
        tab[i].y = courant->y * courant->w;
        tab[i].w = courant->w;
        i++;
        courant = courant->suivant;
    }
    n = i - 1;
    r = Calc_r(U, u, n, k);
    for (j = 1; j <= (k - 1); j++) {
        for (i = r; i >= (r - k + j + 1); i--) {
            tab[i].x = ((u - U[i]) / (U[i + k - j] - U[i]))*tab[i].x + ((U[i + k - j] - u) / (U[i + k - j] - U[i]))*tab[i - 1].x;
            tab[i].y = ((u - U[i]) / (U[i + k - j] - U[i]))*tab[i].y + ((U[i + k - j] - u) / (U[i + k - j] - U[i]))*tab[i - 1].y;
            tab[i].w = ((u - U[i]) / (U[i + k - j] - U[i]))*tab[i].w + ((U[i + k - j] - u) / (U[i + k - j] - U[i]))*tab[i - 1].w;
        }
    }
    return (tab[r]);
}
/*------------------------Courbe de BSplin-------------------------------------*/
void calc_b_spline(liste pt, HDC hdc)  /// , int d
{
    int i;
    float U[30], u, pas;
    ///
    point tab[30];
    int n;
    i = 0;
    liste courant = pt;
    while (courant != NULL) {
        tab[i].x = courant->x * courant->w;
        tab[i].y = courant->y * courant->w;
        tab[i].w = courant->w;
        i++;
        courant = courant->suivant;
    }
    n = i - 1;
    int k = d + 1;
    ///
	u = (float) 0.01;
    pas = u;
    seq_noeud(U, n, k);
    // positionner le curseur
    MoveToEx(hdc, (int)pt->x, (int)pt->y, NULL);
    point p;
    while (u <= 1) {
        p = DeBoorCox(pt, U, n, (d + 1), u);
        p.x = p.x / p.w;
        p.y = p.y / p.w;
        LineTo(hdc, (int)p.x, (int)p.y);
        u = u + pas;
    }
    SelectObject(hdc, GetStockObject(BLACK_PEN));
}
/*------------------------Courbe de BSplin-------------------------------------*/
/*------------------------Bsplin-------------------------------------*/

LRESULT CALLBACK degre(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	TCHAR vIDC_degre[5]=TEXT("2");
	char D[5];
	int j;
	switch (msg) {
		case WM_INITDIALOG :
			//SetDlgItemText( hwnd, IDC_degre,vIDC_degre );
		
		return 0;
		case WM_COMMAND :
			switch LOWORD(wParam)  {

				case IDOK :
					//GetDlgItemText(hwnd,IDC_degre,vIDC_degre,5);
					d=(int)_wtof(vIDC_degre);
					EndDialog(hwnd, 1);										return 0;

				case IDCANCEL :
					EndDialog(hwnd, 0);
					return 0;
			}
			return 0;
	}
	return 0;
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
   {
     
     HMENU      hMenu ;
     static liste index,amont,point_a_inserer;
	 static liste pt,designe;
	 static int nombre,i,SAISIE,MODIFIER,MODIF,SUPPRIMER;	 // Definition des parametres
	 static int BEZIER,BSPLINE,AJOUTER,AJOUT,POIDS,BS;
	 float X,Y;

	 PAINTSTRUCT  ps ;
	 HDC hdc;

	 switch (message)
		{
		case WM_COMMAND:
          hMenu = GetMenu (hwnd) ;
          
          switch (LOWORD (wParam))
			{
         
			case ID_FICHIER_QUITTER :
                          			SendMessage (hwnd, WM_CLOSE, 0, 0) ;
                          			return 0 ;
          
		  
		        case ID_EDITION_SAISIE:
			   SAISIE=1;
			   MODIFIER=0;
			   SUPPRIMER=0;
                           		   AJOUTER=0;
			   return 0;
         
		        case ID_EDITION_MODIFIER:
			   MODIFIER=1;
			   SAISIE=0;
			   SUPPRIMER=0;
               		   AJOUTER=0;
			   return 0;
     
		       case ID_EDITION_SUPPRIMER:
			   MODIFIER=0;
			   SAISIE=0;
			   SUPPRIMER=1;
                           		   AJOUTER=0;
			   return 0;
		  
			case ID_EDITION_AJOUTER:
			   AJOUTER=1;
			   MODIFIER=0;
			   SAISIE=0;
			   SUPPRIMER=0;
			   return 0;
		  
            // Ce case permet Ã  l'utilisateur d'effacer tous les points 
            // de contrÃ´les ainsi que les courbes.
			case ID_EDITION_EFFACER:
                           		   MODIFIER=0;
			   SAISIE=0;
			   SUPPRIMER=0;
                                                AJOUTER=0;
			   BEZIER=0;
			   BSPLINE=0;
			   BS=0;	
			   effacer(&pt);
                          		   InvalidateRect (hwnd, NULL ,TRUE);
			   return 0;
           
			case ID_COURBE_BEZIER: 
		  	   BEZIER=1;
			   MODIFIER=0;
			   SAISIE=0;
			   SUPPRIMER=0;
                          		   AJOUTER=0;
			   BSPLINE=0;
			   BS=0;
			   InvalidateRect (hwnd, NULL ,TRUE);
			   return 0;
		  	

			case ID_COURBE_BSPLINE:
		 	   BSPLINE=1;
		  	   BEZIER=0;
			   MODIFIER=0;
			   SAISIE=0;
			   SUPPRIMER=0;
                           		   AJOUTER=0;
			   BS=0;
			   DialogBox(NULL,MAKEINTRESOURCE(IDD_DEGRE), hwnd, (DLGPROC)degre);
			   InvalidateRect (hwnd, NULL ,TRUE);
			   return 0;

			case ID_COURBE_BEZIERSPLINE:
			   BS=1;
			   BSPLINE=0;
		  	   BEZIER=0;
			   MODIFIER=0;
			   SAISIE=0;
			   SUPPRIMER=0;
                                                AJOUTER=0;
			   DialogBox(NULL,MAKEINTRESOURCE(IDD_DEGRE), hwnd, (DLGPROC)degre);
			   InvalidateRect (hwnd, NULL ,TRUE);
			   return 0;
			   
				 
			
            //Ce case permet Ã  l'utilisateur d'effacer uniquement
            // les courbes
			case  ID_COURBE_EFFACER:
			   BS=0;
			   BSPLINE=0;
		  	   BEZIER=0;
			   InvalidateRect (hwnd, NULL ,TRUE);
			   return 0;
		  }
          
		  	case WM_LBUTTONDOWN:
			  X=  (float)LOWORD (lParam);
			  Y=  (float) HIWORD(lParam) ;
			  		
	              		     if(SAISIE==1)
			        {
			        creer_liste(&pt,X,Y);
			        InvalidateRect (hwnd, NULL ,TRUE);
			
		                       }
            
		      if (MODIFIER==1)
		           {
			        designe=designer_point(pt,X,Y);
			          if (designe!=NULL)
			            {
			             (*designe).x=X;
			             (*designe).y=Y;
			             InvalidateRect (hwnd, NULL ,TRUE);
			             MODIF=1;
                         }
			          else 
                         MessageBox (NULL, TEXT ("Votre liste est vide"),
                         szAppName, MB_ICONERROR) ;
		                 InvalidateRect (hwnd, NULL ,TRUE);
		            }
		     

		if (SUPPRIMER==1)
		           {
		           designe=designer_point(pt,X,Y);
				if(designe!=NULL)
					{
		           supprime (&pt, designe);

				   InvalidateRect (hwnd, NULL ,TRUE);
					}
			     else
                   MessageBox (NULL, TEXT ("Votre liste est vide"),
                         szAppName, MB_ICONERROR) ;
		           InvalidateRect (hwnd, NULL ,TRUE);
		          }
			 
	      	if (AJOUTER==1)
			{
				
                			//......
                
			  designe=designer_point(pt,X,Y);
			    if(designe!=NULL)
					{

					
                      				ajouter(designe, X, Y);
                        				InvalidateRect (hwnd, NULL ,TRUE);   
					AJOUT=1; 
					}
                                                else
					 MessageBox (NULL, TEXT ("Votre liste est vide"),
                         szAppName, MB_ICONERROR) ;
		             InvalidateRect (hwnd, NULL ,TRUE);
		 return 0;
		  } 		  
		  case WM_MOUSEMOVE:
			 if(MODIF==1)
			       {
					(*designe).x=LOWORD(lParam);
					(*designe).y=HIWORD(lParam);
					InvalidateRect (hwnd, NULL ,TRUE);
				   }	  

				else if(AJOUT==1)
				  {
				  (*designe).x=LOWORD(lParam);
				  (*designe).y=HIWORD(lParam);
				  InvalidateRect (hwnd, NULL ,TRUE);
			      }
             
		  return 0;
		  case WM_RBUTTONDOWN :
				
		    	        X=  LOWORD (lParam);
				Y=   HIWORD(lParam) ;
				vertex_attr=designer_point(pt,X,Y);
DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG1), hwnd,             (DLGPROC)AttrProc);
				InvalidateRect(hwnd, NULL, TRUE);

		  return 0;
			
		  case WM_LBUTTONUP:
			    
			  if(MODIF==1) MODIF=0;
			  if(AJOUT==1) AJOUT=0;
			  
		  return 0;
		   case WM_CLOSE:
	           	DestroyWindow(hwnd);
		
			case WM_DESTROY:
			  PostQuitMessage(0);
			return 0;
		      
		  case WM_PAINT:
          
		   hdc = BeginPaint (hwnd, &ps) ;
 
                  	   if(pt!=NULL) 
		     {
		     MoveToEx (hdc,(int)(*pt).x,(int)(*pt).y, NULL) ;
		     index=pt->suivant;
                                   while (index!=NULL) 
		        {
			  	
			    LineTo(hdc, (int)(*index).x, (int)(*index).y);
			    index=index->suivant;
		          
		        }
                 // Affichage de la courbe de BÃ©zier
		   if (BEZIER==1)
		      calc_bezier(pt,hdc);



                 // Affichage de la courbe B-spline
		   if (BSPLINE==1)
			   
				calc_b_spline(pt,hdc);  
			   
				// Afficher les deux courbes en mÃªme temps
		   if (BS==1)
			  {
			  
			      calc_bezier(pt,hdc);
			      calc_b_spline(pt,hdc); //,d
				  

			   }
			   
								
		      
		   }
		 EndPaint (hwnd, &ps) ;

        return 0 ;
		  
		  
     }
         
    return DefWindowProc (hwnd, message, wParam, lParam) ;
}
