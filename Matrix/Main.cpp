
/*  TEMAT PROJEKTU ==================================*/
/* Rozwiazanie ukl rownan Ax=b => x=A^-1 * b  */
/*===================================================*/

#include"Matrix.h"
#include<iostream>
#include <stdio.h>

#define DEBUG

void ReadData( FILE* plik, double** pMatrix, double* pB, int nDim );
void printTab(double* pB, int nDim);

int main( int argc, char* argv[] )
{
     if ( argc != 2 )
     {
         printf("Usage: %s <plik wejsciowy>", argv[0]);
         return 1;
     }

    // otworzyc plik do odczytu   -- argv[1]
    FILE* plik = fopen( argv[1], "r"); // wlaczenie pliku na odczyt
    if ( !plik )
    {
        printf( "ERROR: Read error: %s", argv[1] );
        return -1;
    }

    // odczytac rozmiar nDim
    int nDim = 0; // zmienna do wczytania rozmiaru

    fscanf( plik, "%d", &nDim );

    if ( nDim < 1 )
    {
        printf( "Incorrect matrix size." );
        return 0;
    }

    // wykreowac macierz ukladu nDim x nDim
    // wykreowac wektor wyrazow wolnych 
    double** pMatrix = NULL;
    double* pB = (double*)malloc(sizeof(double) * nDim);//tab na wyrazy wolne
    if ( !CreateMatrix( &pMatrix, nDim ) || !pB )
    {
        perror( "ERROR: Allocation error - Data Matrix/ Constant Terms" );
        return 0;
    }

    memset( pB, 0, sizeof(double)*nDim );

    // wczytac dane ( ReadData() )
    ReadData( plik, pMatrix, pB, nDim );

    fclose(plik);
    plik = NULL;

#ifdef DEBUG

    printf("WYDRUK KONTROLNY, WARUNKOWA KOMPILACJA\n");
    printf("--------------------------------------");

    // rozmiar macierzy
    printf("\n Rozmiar: \n");
    printf("%d\n", nDim);

    // kreowanie macierzy
    printf("\n Macierz poczatkowa:\n");
    PrintMatrix(pMatrix, nDim);
    printf("\n");

    //kreowanie wektora wyrazow wolnych
    printf("\n Wyrazy wolne: \n");
    printTab(pB, nDim);
    printf("\n");

#endif
    //Macierz wynnikowa
    double* Result = (double*)malloc( sizeof(double)*nDim );
    if ( !Result )
    {
        perror( "ERROR: Allocation error - Result Matrix" );
        return 0;
    }
    memset( Result, 0, sizeof(double)*nDim );
    double* dResult = Result;


    //obliczenie wyznacznika
    double det = Det( pMatrix, nDim );
    if (fabs(det) <= 1e-14)
    {
        printf( "ERROR: Det cannot be 0" );
        return 0;
    }
    //Macierz odwrotna
    double** pInv = NULL;
    if ( !CreateMatrix( &pInv, nDim ) )
    {
        perror( "ERROR, Allocation error - pInv" );
        return 0;
    }

    //odwracanie macierzy
    InverseMatrix( pInv, pMatrix, nDim, det );
    PrintMatrix( pInv, nDim );

#ifdef DEBUG

    // obl wyznacznik
    printf( "\n Wyznacznik: \n" );
    printf( "%.lf\n", det );

    // obrocic macierz
    printf( "Macierz odwrotna: \n" );
    PrintMatrix( pInv, nDim );
    printf("\n");

#endif
    // rozwiazywanie ukladu rownan
    LayoutEqu( pInv, pB, Result, nDim ); 

    printf( "Result: \n" );
    printTab(Result, nDim);
    printf("\n");

    DeleteMatrix( &pMatrix, nDim );
  

    DeleteMatrix( &pInv, nDim );
    

    //Result -= nDim;
    

    free( dResult ); 
    free( pB );   

    pB = NULL;
    Result = NULL;
  
   

    return 0;
}

void ReadData( FILE* plik, double** pMatrix, double* pB, int nDim ) // czyta plik
{
    for ( int i = 0; i < nDim; i++ )
    {
        double* DpMatrix = *pMatrix++;
        for ( int j = 0; j < nDim; j++ )
        {
            fscanf( plik, "%lf", DpMatrix++ );
        }
        fscanf( plik, "%lf", pB++ );
    }
}
void printTab(double* pB, int nDim)
{
   
    for ( int i = 0; i < nDim; i++ )
        printf("%lf ", *pB++);

}