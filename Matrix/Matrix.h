#ifndef MATRIX_H
#define MATRIX_H



int CreateMatrix(double*** pMatrix, int nDim);//tablica 2d
void DeleteMatrix(double*** pMatrix, int nDim);//zwalnia pamiec

void PrintMatrix(double** pMatrix, int nDim); //drukuje macierz

void TransMatrix(double** pMatrix, int nDim);

void InverseMatrix( double** pInv, double** pMatrix, int nSize, double det );  //double Det(double** pMatrix, int nDim); //rozwiniecie wzgl 0-go wiersza // dla 2x2 uzyc znanego wzoru - mozna zaindeksowac//// wyznacznik rekurencyjnie 

double Det( double** pMatrix, int nSize ); 

void LayoutEqu(double** pInv, double* pB, double* pRes, int nDim);// rozwiazuje ukl rownan





#endif

