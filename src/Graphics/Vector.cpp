
#include "StdAfx.h"
#include "Vector.h"
#include <math.h>
#include <iostream>
#include <fstream>
//#include <windows.h>
//#include <afxwin.h>

using namespace std;

Vector::Vector(double p, double q, double r)
{
	x = p;
	y = q;
	z = r;
}
double Vector::getx()
{
	return x;
}
double Vector::gety()
{
	return y;
}
double Vector::getz()
{
	return z;
}

Vector Vector::operator +(Vector ob)
{
	return Vector( x + ob.x, y + ob.y, z + ob.z );
}

Vector Vector::operator -(Vector ob)
{
	return Vector( x - ob.x, y - ob.y, z - ob.z);
}

Vector Vector::operator *(Vector ob)
{
	return Vector( ( y*ob.z - z*ob.y), -( x*ob.z - z*ob.x ), ( x*ob.y - y*ob.x )); 
}

Vector Vector::operator *( double k )
{
	return Vector( k*x, k*y, k*z );
}

double Vector::operator ,(Vector ob)
{
	double dotpr;

	dotpr = x*ob.x + y*ob.y + z*ob.z;
	
	return dotpr;
}

double Vector::VectorDet()
{
	double det;
	
	det = sqr(x) + sqr(y) + sqr(z);
	
	det = sqrt(det);

	return det;
}

Vector Vector::UnitVector()
{
	if( VectorDet() )
		return Vector( x/VectorDet(), y/VectorDet(), z/VectorDet());
	return Vector( 0, 0, 0 );
}

void Vector::VectorRep()
{
	cout<<endl
		<< x << "i+" << y << "j+" << z << "k"
		<<endl;
}
	
/*
main()
{
	Vector v1(1,2,3),v2(2,3,4),v3;

	v3 = v1+v2;

	v3.VectorRep();

	v3 = v2*v3;

	v3.VectorRep();

	double dotpr;
	double det;
	
	dotpr = v3.VectorDotPr(v1,v2);

	det = v3.VectorDet(v1);

	cout<<endl
		<<" the dot product of the v1 and v2 vectors is: "
		<<dotpr
		<<endl
		<<" the determinant of the vector"
		<<det
		<<endl;

	v3 = v3.UnitVector(v1);

	v3.VectorRep();

	CDC dc;
	POINT point;
	point.x = v3.getx();
	point.y = v3.gety();
	dc.LineTo(point);

}
*/
Vector::~Vector(void)
{
}

//vector routines implementations
Vector VectorRotation( Vector vPostion, Vector vNormal, Vector vAbtPostion, double dAng )
{
	double t = ( vPostion,vNormal ) - ( vAbtPostion,vNormal );

	Vector vCentre = vAbtPostion + vNormal*t;

	Vector u = vPostion - vCentre;				

	Vector v = vNormal*u;

	double c = cos( dAng );
	double s = sin( dAng );

	Vector r = ( u * c ) + ( v * s );

	return vCentre + r;
}