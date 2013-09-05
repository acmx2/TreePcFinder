#include "StdAfx.h"
#include "Point3d.h"
#include "Exception.h"
#include <math.h>
#include <cfloat>

using namespace Tpc::Geom;
using namespace std;


const Tpc::Geom::Point3d Tpc::Geom::Point3d::kOrigin(0,0,0);
const Tpc::Geom::Point3d Tpc::Geom::Point3d::kPosInfinity(FLT_MAX,FLT_MAX,FLT_MAX);
const Tpc::Geom::Point3d Tpc::Geom::Point3d::kNegInfinity(-FLT_MAX,-FLT_MAX,-FLT_MAX);


Tpc::Geom::Point3d Tpc::Geom::Point3d::crossProduct( const Point3d& pt ) const
{
	return Point3d(y*pt.z -z*pt.y, pt.x*z -z*pt.x, x*pt.y -y*pt.x);
}

float Tpc::Geom::Point3d::length() const
{
	return sqrtf(lengthSquare());
}

bool Tpc::Geom::Point3d::isEqualTo( const Point3d& pt, float tol /*=TPC_TOL*/ ) const
{
	Point3d delta =((*this) -pt);
	float sq2 =lengthSquare();
	float tol2 =tol*tol;
	
	if (lengthSquare() <=tol2)
	{
		return true;
	}
	return false;
}

Tpc::Geom::Point3d Tpc::Geom::Point3d::getXyProjection() const
{
	return Point3d(x,y,0);
}

bool Tpc::Geom::Point3d::operator==( const Point3d& pt ) const
{
	return ((x==pt.x) &&(y==pt.y) &&(z==pt.z));
}
