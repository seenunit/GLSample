#include "stdafx.h"
#include "SampleMath.h"
#include <iostream>

//using namespace std;

double nfact( int n )
{
	if( n >= 0 )
	{
		double fact = 1;
		for( int i = 1; i <= n; i++ )
			fact = fact *  i;
		return fact;
	}
	else return 0; 
}
double nCr( int n, int r )
{
	double ncr = 0;
	if( n >= r )
	{
		ncr = nfact( n )/(nfact( r ) * nfact( n - r ));
		return  ncr;
	}
	return 0;
}
