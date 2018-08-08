#pragma once


class Vector
{
public:
	double x,y,z;

	Vector() {}

	Vector(double p, double q, double r);
	
	double getx();
	
	double gety();
	
	double getz();
	
	Vector operator +(Vector ob);

	Vector operator -(Vector ob);

	Vector operator *(Vector ob);

	Vector operator *( double k );

	double operator ,(Vector ob);

	double VectorDet();

	Vector UnitVector();

	void VectorRep();
public:
	~Vector(void);
};

//Vector routines
Vector VectorRotation( Vector vPostion, Vector vNormal, Vector vAbtPostion, double dAng );
