#include "stdafx.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "Matrix.h"

//#include "ap.h"
//#include "inv.h"
//#include "det.h"
//#include "apvt.h"
using namespace std;

//inverse of the matrix by Gauss(related method).
/* prototype the invert functions that follow exec source */
void Gauss(double ** a, double ** b, int n);
void Crout(double ** a, double ** b, int n);
int rgaussi(double ** a, double ** b, int n);

/* below are additional C routines supplied by translator */
void memflt()
{
	fputs("Memory allocation error\n", stderr);
	exit(EXIT_FAILURE);
}



Matrix::Matrix()
{
	g = NULL;
	r = c = 0;
}

Matrix::Matrix(int row, int column)
{
	r = row;
	c = column;
	try 
	{
		g = new double[r*c];
		for(int i = 0; i < (r*c); i++)
			g[i] = 0;
	} 
	catch (bad_alloc xa) {
		cout << "Allocation Failure\n";
		exit(EXIT_FAILURE);
	}
}

Matrix::Matrix(double *pMat, int row, int column)
{
	r = row;
	c = column;
	try 
	{
		g = new double[r * c];
		for(int i = 0; i < (r * c); i++)
			g[i] = pMat[i];
	} 
	catch (bad_alloc xa) {
		cout << "Allocation Failure\n";
		exit(EXIT_FAILURE);
	}
}

Matrix::Matrix( Matrix& mat ):g(NULL)
{
	*this = mat;
	/*
	r = mat.r;
	c = mat.c;
	try 
	{
		g = new double[r * c];
		for( int i = 0; i < (r * c); i++) 
			g[i] = mat.g[i];
	} 
	catch (bad_alloc xa) {
		cout << "Allocation Failure\n";
		exit(EXIT_FAILURE);
	}*/
}

Matrix::~Matrix()
{
	if( g ){
		delete [] g; 
		g = NULL;
	}
}

void Matrix::SetMatrix(double *pMat, int row, int column)
{
	if(g) delete [] g;

	r = row;
	c = column;

	try 
	{
		g = new double[r * c];
		for( int i = 0; i < (r * c); i++) 
			g[i] = pMat[i];
	} 
	catch (bad_alloc xa) {
		cout << "Allocation Failure\n";
		exit(EXIT_FAILURE);
	}	
}

double &Matrix::put( int row, int column)
{
	return g[(row*c+column)];
}

double Matrix::get(int row, int column)
{
	return g[(row*c+column)];
}
void Matrix::DisplayMatrix()
{
	for(int i = 0; i < r; i++)
	{
		for(int j = 0; j < c; j++)
			cout << setw(4) << g[(i*c+j)] << " ";
		cout << "\n";
	}
}

double Matrix::MatrixDet()
{
	double det = 0.0;
	int flag;
	if(r == c)
	{
		for(int j = 0; j < c ; j++)
		{
			if(j%2 == 0) flag = 1;
			else flag = -1;

			if(r== 1)det = g[j];
			else
				det =  det + flag * g[j] * ((this->minor(0,j)).MatrixDet());
		}
		return det;
	}
	else
	{
		cout<<endl  
			<<" entered matrix is not a square matrix"<<endl
			<<" detrminant of the matrix can not be found";
		return -1;
	}

	//ap::real_2d_array  a;
	//a.setcontent(0, (r - 1), 0, (c - 1), g);
	//det = rmatrixdet(a, r);

	return det;
		
}
Matrix Matrix::minor(int row, int column)
{
	int i,j,l,k;
	Matrix Mtr((r-1), (c-1));
	if( r == c)
	{
		l = 0;
		for(i=0 ;i < r; i++)
		{
			if(i == row) continue;
			else
			{
				k = 0;
				for(j = 0; j < r; j++)
				{
					if(j == column) continue;
					else Mtr.put(l,k) = g[i*c+j];							
					k++;
				}
			
			}
			l++;
		}
	}
	else 
		cout << "The input matrix is not square.\n";
	
	return Mtr;
}

double Matrix::cofactor(int row, int column)
{
	int w = (row + column);
	int flag;
	
	if(w%2) flag = -1;
	else flag = 1;
	
	Matrix Mtr = this->minor(row, column);
	
	double d = flag * Mtr.MatrixDet();
	
	return d;
}

Matrix Matrix::CofactorMat()
{
	Matrix Mtr(r,c);
	for(int i = 0; i < r; i++)
		for(int j = 0; j < c; j++)
			Mtr.put(i,j) = this->cofactor(i,j);
	return Mtr;
}

Matrix Matrix::TransposeMat()
{
	Matrix Mtr(c,r);
	for(int i = 0; i < r; i++)
		for(int j = 0; j < c; j++)
			Mtr.put(j,i) = this->get(i,j);
	return Mtr;
}

Matrix Matrix::InverseMat()
{
	Matrix Mtr(r,c);

	//ap::real_2d_array  a;
	//a.setcontent(0, (r - 1), 0, (c - 1), g);
	//bool result = rmatrixinverse(a, r);
	//for(int i = 0; i < r; i++)
	//{
	//	ap::raw_vector<double> rvec = a.getrow(i, 0, c-1);
	//	double *prow = rvec.GetData();
	//
	//	for(int j = 0; j < c; j++)
	//		Mtr.put(j,i) = prow[j];
	//}

	//double **ppdMat = new double*[r];

	//for(int i = 0; i < r; i++)
	//	ppdMat[i] = new double[r];		

	//for(int i = 0; i < r; i++)
	//	for(int j = 0; j < r; j++)
	//		ppdMat[i][j] = this->get(i, j);

	//double **ppdInvMat = new double*[r];

	//for(int i = 0; i < r; i++)
	//	ppdInvMat[i] = new double[r];		

	//int err = rgaussi(ppdMat, ppdInvMat, r);
	//if(err)
	//	return Mtr;

	//Matrix matTemp(r,r);
	//
	//for(int i = 0; i < r; i++)
	//	for(int j = 0; j < r; j++)
	//		matTemp.put(i, j) = ppdInvMat[i][j];

	//Mtr = matTemp.TransposeMat();

	//double det = this->MatrixDet();
	//// cofactor of the matrix
	//Matrix cofmat = this->CofactorMat();
	//// transpose of the cofactor matrix
	//Matrix trmat = cofmat.TransposeMat();

	C5MatrixInversion(r, g, Mtr.g);

	//check for singularity of the matrix
	//if(det)
	//	for(int i = 0; i < r; i++)
	//		for(int j = 0; j < c; j++)
	//			Mtr.put(j,i) = trmat(i,j)/ det;
	//else
	//	cout<<" entered matrix is singular matrix"<<endl
	//		<<" inverse of the matrix not exist"
	//		<<endl;
	return Mtr;
}

Matrix Matrix::operator *(Matrix &mat)
{
	Matrix Mat(r,mat.c);
	if(c == mat.r)
	{
		for(int i = 0; i < r; i++)
			for(int j = 0; j < mat.c; j++)
			{
				Mat.put(i,j) = 0;
				for(int k = 0; k < c; k++)
					Mat.put(i,j) = Mat.get(i, j) + (this->get(i, k)* mat.get(k, j));
			}		
	}
	else
		cout<<"multiplication is not possible";
	return Mat;
}
Matrix &Matrix::operator =(Matrix &mat)
{
	r = mat.r;
	c = mat.c;

	try 
	{
		if( g ) delete [] g;
		g = new double[r * c];
		for( int i = 0; i < (r * c); i++) 
			g[i] = mat.g[i];
	} 
	catch (bad_alloc xa) {
		cout << "Allocation Failure\n";
		exit(EXIT_FAILURE);
	}
	
	return *this;
}

double &Matrix::operator ()(int row, int column)
{
	return g[(row*c+column)];
}

#define ODXPRO_INVALID_INT -2147483647

int C5MatrixInversion(int n, const double *am, double *inv_am)
{
//    double dm[MAXDIM][MAXDIM];
//    double dv[MAXDIM];
 //   double ev[MAXDIM];
	double **dm;
    double *dv;
    double *ev;
    double d;
    double e;
    int i;
    int j;
    int k;
    int l;
 //   int ifv[MAXDIM];
//    int igv[MAXDIM];
 //   char hv[MAXDIM];
	int *ifv;
    int *igv;
    char *hv;

	dm = (double**)malloc(sizeof(double*)*n);
	hv = (char*)malloc(sizeof(char)*(n+1));
	dv = (double*)malloc(sizeof(double)*n);
	ev = (double*)malloc(sizeof(double)*n);
	ifv = (int*)malloc(sizeof(int)*n);
	igv = (int*)malloc(sizeof(int)*n);

    for (i = 0; i < n; i++)
    {
        hv[i] = 0;
		dm[i] = (double*)malloc(sizeof(double)*n);
        for (j = 0; j < n; j++)
        {
            dm[i][j] = am[i * n + j];
        }
    }

    for (k = 0; k < n; k++)
    {
        l = ODXPRO_INVALID_INT;
        d = 0.0;
        for (j = 0; j < n; j++)
        {
            if (!hv[j])
            {
                e = dm[k][j] < 0.0 ? -dm[k][j]:dm[k][j];
                if (e > d)
                {
                    d = e;
                    l = j;
                }
            }
        }

        if (l == ODXPRO_INVALID_INT)
            return (5);     /* Singular matrix   */

        d = 1.0 / dm[k][l];
        for (i = 0; i < n; i++)
        {
            dv[i] = d * dm[i][l];
            ev[i] = dm[k][i];
            dm[i][l] = 0.0;
            dm[k][i] = 0.0;
        }
        dv[k] = d;
        ev[l] = -1.0;
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
            {
                dm[i][j] -= dv[i] * ev[j];
                dm[k][l] = d;
                ifv[k] = l;
                igv[l] = k;
            }
        }
        hv[l] = 1;
    }
    for (i = 0; i < n; i++)
    {
        k = ifv[i];
        for (j = 0; j < n; j++)
        {
            l = igv[j];
            inv_am[k * n + l] = dm[i][j];
        }
    }

	for(i=0;i<n;i++)
		free((void*)dm[i]);

	free((void*)dm);
	free((void*)dv);
	free((void*)ev);
	free((void*)ifv);
	free((void*)igv);
	free((void*)hv);

    return (0);             /* successful execution */
}

/*
* --------------------------------------
* Invert matrix a -b by Gauss method
* --------------------------------------
*/
void Gauss(double ** a, double ** b, int n)
{
	double d,
		temp = 0,
		c;
	int i,
		j,
		k,
		m,
		nn,
		*ipvt;

	if ((ipvt = (int *) malloc(n * sizeof(int))) == NULL)
		memflt();

	nn = n;
	for (i = 0; i < nn; i++)
		ipvt[i] = i;

	for (k = 0; k < nn; k++) {
		temp = 0.;
		m = k;
		for (i = k; i < nn; i++) {
			d = a[k][i];
			if (fabs(d)> temp) {
				temp = fabs(d);
				m = i;
			}
		}
		if (m != k) {
			j = ipvt[k];
			ipvt[k] = ipvt[m];
			ipvt[m] = j;
			for (j = 0; j < nn; j++) {
				temp = a[j][k];
				a[j][k] = a[j][m];
				a[j][m] = temp;
			}
		}
		d = 1 / a[k][k];
		for (j = 0; j < k; j++) {
			c = a[j][k] * d;
			for (i = 0; i < nn; i++)
				a[j][i] -= a[k][i] * c;
			a[j][k] = c;
		}
		for (j = k + 1; j < nn; j++) {
			c = a[j][k] * d;
			for (i = 0; i < nn; i++)
				a[j][i] -= a[k][i] * c;
			a[j][k] = c;
		}
		for (i = 0; i < nn; i++)
			a[k][i] = -a[k][i] * d;
		a[k][k] = d;
	}

	for (i = 0; i < nn; i++)
		memcpy(b[ipvt[i]], a[i], sizeof(double) * nn);

	free(ipvt);
}


/*
* --------------------------------------
* Invert matrix a -b by Crout method
* --------------------------------------
*/
void Crout(double ** a, double ** b, int n)
{
	int i,
		j; /* Current row & column */
	int maxlok; /* Location of maximum pivot */
	int *index; /* Partial pivot record */
	double *temp = 0,
		the_max;
	double tmp,
		*ptr;
	double *matr = 0;
	int k,
		ind,
		ind2;

	if ((index = (int *) malloc(n * sizeof(int))) == NULL ||
		(temp = (double *) malloc(n * sizeof(double))) == NULL ||
		(matr = (double *) malloc(n * n * sizeof(double))) == NULL)
		memflt();

	/* Initialize everything */

	for (i = 0; i < n; i++)
		index[i] = i;

	/* Shuffle matrix */
	for (j = 0; j < n; j++) {
		for (i = 0; i < j; i++)
			b[j][i] = a[j][i];
		for (i = j; i < n; i++)
			b[j][i] = a[i - j][n - j - 1];
	}

	/* LU decomposition; reciprocals of diagonal elements in L matrix */
	for (j = 0; j < n; j++) {
		/* Get current column of L matrix */
		for (i = j; i < n; i++) {
			tmp = 0;
			ind = n - i - 1;
			for (k = 0; k < j; k++)
				tmp += b[ind][ind + k] * b[j][k];
			b[ind][ind + j] -= tmp;
		}
		maxlok = 0;
		the_max = fabs(b[0][j]);
		for (i = 1; i < n - j; i++)
			if (fabs(b[i][j + i]) >= the_max) {
				the_max = fabs(b[i][j + i]);
				maxlok = i;
			}
			/* det = det*b(j+maxlok-1,maxlok) */
			b[maxlok][j + maxlok] = 1 / b[maxlok][j + maxlok];

			/* Swap biggest element to current pivot position */
			if (maxlok + 1 != n - j) {
				ind = n - maxlok - 1;
				ind2 = index[j];
				index[j] = index[ind];
				index[ind] = ind2;
				for (k = n - maxlok; k < n; k++) {
					tmp = b[k][j];
					b[k][j] = b[k][ind];
					b[k][ind] = tmp;
				}
				memcpy(temp, &(b[maxlok][maxlok]), sizeof(double) * (n -
					maxlok));
				ptr = &(b[n - j - 1][n - j - 1]);
				memmove(&(b[maxlok][maxlok]), ptr, sizeof(double) * (j + 1));
				for (k = j + 1; k < n - maxlok; k++)
					b[maxlok][maxlok + k] = b[k][j];
				memcpy(ptr, temp, (j + 1) * sizeof(double));
				for (k = j + 1; k < n - maxlok; k++)
					b[k][j] = temp[k];
			}
			/* Get current row of U matrix */
			ind = n - j - 1;
			for (i = j + 1; i < n; i++) {
				tmp = 0.;
				for (k = 0; k < j; k++)
					tmp += b[i][k] * b[ind][ind + k];
				b[i][j] = b[ind][n - 1] * (b[i][j] - tmp);
			}
	} /* END DO LU_outer */

	/* Invert L matrix */
	for (j = 0; j < n - 1; j++) {
		temp[0] = b[n - j - 1][n - 1];
		for (i = j + 1; i < n; i++) {
			ind = n - i - 1;
			tmp = 0.;
			for (k = 0; k < i - j; k++)
				tmp += temp[k] * b[ind][ind + j + k];
			b[ind][ind + j] = -tmp * b[ind][n - 1];
			temp[i - j] = b[ind][ind + j];
		}
	}

	/* Reshuffle matrix */
	for (i = 0; i < (n + 1) / 3; i++) {
		memcpy(temp, &(b[i][2 * (i + 1) - 1]), sizeof(double) * (n + 2 -
			3 * (i +1)));
		for (j = 2 * (i + 1) - 1; j < n - i; j++)
			b[i][j] = b[n - j - 1][n - j + i - 1];
		ind = n - i - 1;
		for (j = i; j < n + 1 - 2 * (i + 1); j++)
			b[j][i + j] = b[n - i - j - 1][ind];
		for (k = 0; k < n + 2 - 3 * (i + 1); k++)
			b[i + 1 + k][ind] = temp[k];
	}

	/* Invert U matrix */
	for (i = 0; i < n - 1; i++) {
		for (j = i + 1; j < n; j++) {
			tmp = 0.;
			for (k = 0; k < j - i - 1; k++)
				tmp += temp[k] * b[j][i + 1 + k];
			b[j][i] = -b[j][i] - tmp;
			temp[j - i - 1] = b[j][i];
		}
	}

	/* Multiply inverses in reverse order */
	for (i = 0; i < n - 1; i++) {
		for (k = 0; k < n - i - 1; k++)
			temp[k] = b[i + 1 + k][i];
		for (j = 0; j <= i; j++) {
			tmp = 0.;
			for (k = 0; k < n - i - 1; k++)
				tmp += temp[k] * b[j][i + 1 + k];
			b[j][i] += tmp;
		}
		for (j = i + 1; j < n; j++) {
			tmp = 0.;
			for (k = j; k < n; k++)
				tmp += temp[k - i - 1] * b[j][k];
			b[j][i] = tmp;
		}
	}

	/* Straighten out the columns of the result */
	for (i = 0; i < n; i++)
		memcpy(matr + n * i, b[i], sizeof(double) * n);
	for (i = 0; i < n; i++)
		memcpy(b[index[i]], matr + n * i, sizeof(double) * n);

	free(index);
	free(temp);
	free(matr);
}

/*
** This routine is due to buers@gmx.de (Dieter Buerssner)
** Destroys a, return 0: success, 1: zero pivot, 2: out of mem.
*/
int rgaussi(double ** a, double ** b, int n)
{
	int i,
		j,
		k,
		maxj,
		t;
	double maxel,
		pivinv,
		tmaxel,
		aji;
	double *tp,
		*ai,
		*aj;
	/* C99: int ipiv[n]; */
	int *ipiv = (int *)malloc(n * sizeof(int));

	if (ipiv == NULL)
		return 2;
	for (i = 0; i < n; i++)
		ipiv[i] = i;
	for (i = 0; i < n; i++) 
	{
		maxj = -1;
		maxel = 0.0;
		/* find pivot element */
		for (j = i; j < n; j++)
			if ((tmaxel = fabs(a[j][i]))> maxel) {
				maxj = j;
				maxel = tmaxel;
			}
			if (maxj < 0) {
				free(ipiv);
				return 1;
			}
			/* exchange rows */
			if (maxj != i) {
				/* just exchange pointers for a */
				tp = a[maxj];
				a[maxj] = a[i];
				a[i] = tp;
				t = ipiv[maxj];
				ipiv[maxj] = ipiv[i];
				ipiv[i] = t;
			}
			ai = a[i];
			pivinv = 1.0 / ai[i];
			ai[i] = 1.0;
			for (k = 0; k < n; k++)
				ai[k] *= pivinv;
			for (j = 0; j < n; j++) {
				if (j != i) {
					aj = a[j];
					aji = aj[i];
					aj[i] = 0.0;
					for (k = 0; k < n; k++)
						aj[k] -= aji * ai[k];
				}
			}
	}
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			b[i][ipiv[j]] = a[i][j];
	free(ipiv);
	return 0;
}