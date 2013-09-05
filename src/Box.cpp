#include "StdAfx.h"
#include "Box.h"
#include "Exception.h"


using namespace Tpc::Geom;

Tpc::Geom::Box::Box( void )
{
}

Tpc::Geom::Box::Box( const Point3d& minPt,const Point3d& maxPt)
	: m_minPt(minPt)
	, m_maxPt(maxPt)
{
}

Tpc::Geom::Box::~Box( void )
{
}

const Point3d& Tpc::Geom::Box::getMaxPoint() const
{
	return m_maxPt;
}

const Point3d& Tpc::Geom::Box::getMinPoint() const
{
	return m_minPt;
}

void Tpc::Geom::Box::append( const Point3d& pt )
{
	if (pt.x <m_minPt.x)
	{
		m_minPt.x =pt.x;
	}
	if (pt.y <m_minPt.y)
	{
		m_minPt.y =pt.y;
	}
	if (pt.z <m_minPt.z)
	{
		m_minPt.z =pt.z;
	}

	if (pt.x >m_maxPt.x)
	{
		m_maxPt.x =pt.x;
	}
	if (pt.y >m_maxPt.y)
	{
		m_maxPt.y =pt.y;
	}
	if (pt.z >m_maxPt.z)
	{
		m_maxPt.z =pt.z;
	}

}

void Tpc::Geom::Box::setToEmpty()
{
	m_minPt =Point3d::kPosInfinity;
	m_maxPt =Point3d::kNegInfinity;
}

bool Tpc::Geom::Box::isEmpty() const
{
	if (m_minPt.x >m_maxPt.x)
	{
		return true;
	}
	if (m_minPt.y >m_maxPt.y)
	{
		return true;
	}
	if (m_minPt.z >m_maxPt.z)
	{
		return true;
	}

	return false;
}

float Tpc::Geom::Box::getDiameter() const
{
	TPC_ASSERT(!isEmpty());
	 
	return (getMaxPoint() -getMinPoint()).length();
}

float Tpc::Geom::Box::getHeight() const
{
	TPC_ASSERT(!isEmpty());
	return (getMaxPoint().z -getMinPoint().z);
}

float Tpc::Geom::Box::getWidthX() const
{
	TPC_ASSERT(!isEmpty());
	return (getMaxPoint().x -getMinPoint().x);
}

float Tpc::Geom::Box::getWidthY() const
{
	return (getMaxPoint().y -getMinPoint().y);
}


bool Tpc::Geom::Box::contains( const Point3d& pt ) const
{
	if (!containsXy(pt))
	{
		return false;
	}
	if (pt.z <getMinPoint().z)
	{
		return false;
	}
	if (pt.z >getMaxPoint().z)
	{
		return false;
	}

	return true;
}

bool Tpc::Geom::Box::containsXy( const Point3d& pt ) const
{
	if (pt.x <getMinPoint().x)
	{
		return false;
	}
	if (pt.y <getMinPoint().y)
	{
		return false;
	}
	if (pt.x >getMaxPoint().x)
	{
		return false;
	}
	if (pt.y >getMaxPoint().y)
	{
		return false;
	}

	return true;
}

bool Tpc::Geom::Box::operator==( const Box& x ) const
{
	return ((m_minPt ==x.m_minPt) && (m_maxPt ==x.m_maxPt));
}

bool Tpc::Geom::Box::operator!=( const Box& x ) const
{
	return !(operator ==(x));
}

const Point3d Tpc::Geom::Box::getCenterPoint() const
{
	TPC_ASSERT(!isEmpty());
	return ((getMinPoint() +getMaxPoint())/2.0);
}
