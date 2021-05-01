#include <iostream>
#include <stdio.h>
#include "Matrix.h"

#define DEBUG


void ComplMatrix( double** pMatrixD, double** pMatrix, int nDim ); //macierz dopelnien
void Complement( double** pMatrixO, double** pMatrixI, int nRow, int nCol, int nDim );


int CreateMatrix( double*** pMatrix, int nDim ) // zwraca 0 lub 1, 0-alokacja siê nie powiod³a 1-tak
// ka¿dy wiersz macierzy musi byæ wyzerowany – memset()
{
	*pMatrix = (double**)malloc( sizeof(double)*nDim );
    if (!*pMatrix) return 0;
    memset(*pMatrix, 0, sizeof(double) * nDim);


        double** ppMatrix = *pMatrix;//pomocniczy wskaznik zeby nie zmieniac pMatrix
        for (int i = 0; i < nDim; i++)
        {
            *ppMatrix = (double*)malloc(sizeof(double) * nDim);
            if (!*ppMatrix) return 0;
            memset(*ppMatrix++, 0, sizeof(double) * nDim);

        }
        return 1;
        
}

void DeleteMatrix( double*** pMatrix, int nDim )
{
    free(**pMatrix);
		free( *pMatrix );//zwolnienie pamieci dla tab wskaznikow

		*pMatrix = NULL;
	
}
void PrintMatrix( double** pMatrix, int nDim )
{
	 

	for ( int i = 0; i < nDim; i++ )
	{
		double* ppMatrix = *pMatrix++; // o 1 w prawo(kolumny)
		for ( int j = 0; j < nDim; j++ )
		{
			printf( "%lf ", *ppMatrix++ ); // o 1 w dol(wiersze)
		}
		printf("\n");
	}
}

void TransMatrix(double** pMatrix, int nDim) //transponowanie macierzy
{

    double** p = pMatrix;
    for (int i = 0; i < nDim - 1; i++)
    {
        double* v = *p++ + i + 1;
        for (int j = i + 1; j < nDim; j++, v++)
        {
            double temp = *v;
            *v = pMatrix[j][i];
            pMatrix[j][i] = temp;
        }

    }

    /*for ( int i = 0; i < nDim - 1; i++ )
    {
        for ( int j = i + 1; j < nDim; j++ )
        {
            double temp = pMatrix[i][j];
            pMatrix[i][j] = pMatrix[j][i];
            pMatrix[j][i] = temp;
        }
    }*/
}



void InverseMatrix( double** pInv, double** pMatrix, int nDim, double det )// macierz odwrotna , na we obliczony wczesniej w main() det
{
 	//ppInv- macierz odwrotna
 	ComplMatrix( pInv, pMatrix, nDim );// tworzy macierz doplenien

 #ifdef DEBUG

 	printf("\n Macierz Dopelnien:\n");
	PrintMatrix( pInv, nDim );
 	printf("\n");

 #endif


 	TransMatrix( pInv, nDim ); // transponuje te macierz

 #ifdef DEBUG

     
 	printf( "\n Transponowanie:\n" );
 	PrintMatrix( pInv, nDim );
 	printf("\n");

 #endif
    
 	for ( int i = 0; i < nDim; i++ )
 	{
 		double* ppInv = *pInv++;
 		for ( int j = 0; j < nDim; j++ )
 		{
 			*ppInv++ /= det; // dzieli kazdy element z pInv przez det i ponownie wrzuca do pInv
 		}
 	}

 }


double Det( double** pMatrix, int nDim ) //rozwiniecie wzgl 0-go wiersza // dla 2x2 uzyc znanego wzoru - mozna zaindeksowac//// wyznacznik rekurencyjnie
{
 	if( nDim == 1 ) return **pMatrix;
 	if ( nDim == 2 ) return **pMatrix * pMatrix[1][1] - pMatrix[1][0] * pMatrix[0][1];

 	double det = 0; // wartosc wyznacznika

 	double** dMatrix = NULL; // macierz do wpisywania macierzy po skresleniu wiersza i kolumny czyli jak w 6x6 skreslam to ta macierz bedzie 5x5
 	if ( !CreateMatrix( &dMatrix, nDim - 1 ) )
 	{
 		perror( "ERROR: Allocation error - Det" );
 		return 0;
 	}

 	int change = 1;  // 1 do potegi wiersz + kolumna /// bedzie przestawiane z 1 na -1	
 	double* ppMatrix = *pMatrix; // pomocniczy do poruszania sie po kolumnach
    for ( int i = 0; i < nDim; i++, change = -change )
    {
       
        Complement( dMatrix, pMatrix, 0, i, nDim );
        det += *ppMatrix++ * change * Det( dMatrix, nDim - 1 );


#ifdef DEBUG

        printf( "\n Complement, Macierz po wycieciu: \n" );
        PrintMatrix( dMatrix, nDim - 1 );
        printf("\n");

#endif
    }
	
 	DeleteMatrix( &dMatrix, nDim - 1 );//czyszczenie macierzy
 	return det;
}

void LayoutEqu( double** pInv, double* pB, double* pRes, int nDim )// rozwiazuje ukl rownan
{
	//p-Inv - macierz odwrotna
	//pB - wyrazy wolne
	//pRes - tablica wynikowa

	for ( int i = 0; i < nDim; i++ )
	{
		double* ppInv = *pInv++; // wsk do odwroconej aby zejsc wiersz w dol
		double* ppB = pB; // do powrotu na poczatek wyrazow wolnych

		for ( int j = 0; j < nDim; j++ )
			*pRes += *ppInv++ * *ppB++; // mnozenie odwroconej przez wolne i wstawiaj do pRes

		pRes++;
	}
}
void Complement( double** pMatrixO, double** pMatrixI, int nRow, int nCol, int nDim ) // oblicza macierz dopelnien pMatrixD na podstawie tablic wejsc pMatrix
{
 	for ( int i = 0; i < nDim; i++ )
 	{
 		double* ppMatrixI = *pMatrixI++; // pomocniczy aby miec mozliwosc zejscia 
 		if ( i == nRow ) // jesli skreslany wiersz == i to continue 
 			continue;

 		double* ppMatrixO = *pMatrixO++; // pomocniczy aby miec mozliwosc zejscia

 		for ( int j = 0; j < nDim; j++, ppMatrixI++ )
 		{
 			if ( j == nCol ) // gdy skreslana kolumna == j to continue 
 				continue;
 			*ppMatrixO++ = *ppMatrixI;
 			
 		}
 	}

}

 void ComplMatrix( double** pMatrixD, double** pMatrix, int nDim )//macierz dopelnien
{
    

 	//double** ppMatrix = pMatrix;

 	double** DpMatrix = NULL; // macierz sluzaca aby wpisac w nia po skresleniu wiersza i kolumny przez funkcje Complement
 	if ( !CreateMatrix( &DpMatrix, nDim - 1 ) )
 	{
 		perror("ERROR: Allocation error - ComplMatrix");
 		return;
 	}

 	for ( int i = 0; i < nDim; i++ )
 	{
 		//*ppMatrix++;
 		int change = (i % 2) ? -1 : 1; // do zmieniania z dodatniej na ujemna
 		double* ppMatrixD = *pMatrixD++;

 		for ( int j = 0; j < nDim; j++ )
 		{
 			Complement( DpMatrix, pMatrix, i, j, nDim );
 			*ppMatrixD++ = change * Det( DpMatrix, nDim - 1 );
 			change = -change;
 		}
 	}
 	DeleteMatrix( &DpMatrix, nDim - 1) ;
}
