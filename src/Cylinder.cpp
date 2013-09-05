#include "StdAfx.h"
#include "Cylinder.h"
#include "Exception.h"


using namespace Tpc::Geom;

Tpc::Geom::Cylinder::Cylinder( void ) 
: m_radius(0)
{
}

Tpc::Geom::Cylinder::Cylinder( const Point3d& c, float zMin, float zMax, float r ) 
: m_center(c)
, m_zMin(zMin)
, m_zMax(zMax)
, m_radius(r)
{
	TPC_ASSERT(m_zMin<=zMax);
}

Tpc::Geom::Cylinder::~Cylinder( void )
{
}

const Point3d& Tpc::Geom::Cylinder::getCenter() const
{
	return m_center;
}

float Tpc::Geom::Cylinder::getRadius() const
{
	return m_radius;
}

float Tpc::Geom::Cylinder::getZMin() const
{
	return m_zMin;
}

float Tpc::Geom::Cylinder::getZMax() const
{
	return m_zMax;
}

bool Tpc::Geom::Cylinder::contains( const Point3d& pt ) const
{
	if (!containsXy(pt))
	{
		return false;
	}

	if (pt.z >m_zMax)
	{
		return false;
	}
	if (pt.z <m_zMin)
	{
		return false;
	}

	return true;
}

bool Tpc::Geom::Cylinder::containsXy( const Point3d& pt ) const
{
	return ((m_center -pt).getXyProjection().length() <=m_radius);
}
