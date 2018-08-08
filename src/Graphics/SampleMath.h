#ifndef __SAMPLE_MATH_FOR__
#define __SAMPLE_MATH_FOR__

#pragma once

#include <cmath>

extern double nfact( int n );

extern double nCr( int n, int r );

template <class X>
X sqr(X d)
{
	X sqr = d*d;
	return sqr;
}

template <class Y>
Y cube(Y d)
{
	Y cube = d*d*d;
	return cube;
}

template <class Y>
Y npower(Y d, int p)
{
	Y npower = 1; 
	for( int i = 0; i < p; i++ )
		npower = npower*d;
	return npower;
}


#endif //__SAMPLE_MATH_FOR__