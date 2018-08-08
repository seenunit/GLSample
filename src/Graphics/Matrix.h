
#pragma once

class Matrix
{
	double *g;
	int r,c;
public:
	
	Matrix();
	Matrix(int row, int column);
	Matrix(double *pMat, int row, int column);
	Matrix( Matrix &mat);
	~Matrix();
public:
	double &put(int row, int column);
	double get(int row, int column);
	
	void DisplayMatrix(); // display the matrix in console
	
	//double* SetMatData(int *p,int *q);
	void SetMatrix(double *pMat, int row, int column );
	Matrix minor(int r, int c);
	double MatrixDet();
	double cofactor(int r, int c);
	Matrix CofactorMat();
	Matrix TransposeMat();
	Matrix InverseMat();
	
	Matrix operator *( Matrix &mat);
	Matrix &operator =( Matrix &mat);
	double &operator ()(int row, int column);
	
};


int C5MatrixInversion(int n, const double *am, double *inv_am);