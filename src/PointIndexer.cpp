#include "StdAfx.h"
#include "PointIndexer.h"
#include "Exception.h"
#include <vector>


using namespace Tpc::Geom;
using namespace Tpc::Sys;
using namespace std;


///=================PointIndexerXy====================

const int Tpc::Geom::PointIndexerXy::kDimSizeBits =15;


Tpc::Geom::PointIndexerXy::PointIndexerXy()
	: m_stepX(0)
	, m_stepY(0)
{
	m_bounds.setToEmpty();
}

Tpc::Geom::PointIndexerXy::PointIndexerXy( const Box& box, float sx, float sy )
: m_bounds(box)
, m_stepX(sx)
, m_stepY(sy)
{
	TPC_ASSERT(!m_bounds.isEmpty());
	TPC_ASSERT(m_stepX >TPC_TOL);
	TPC_ASSERT(m_stepY >TPC_TOL);

	///inflating box by moving its max point - to get all cells centers inside the box
	Point3d maxPt =m_bounds.getMaxPoint();
	const Point3d minPt =m_bounds.getMinPoint();
	const float dx =getMaxXKeyPart()*sx -0.1*sx;
	const float dy =getMaxYKeyPart()*sy -0.1*sy;
	if ((maxPt.x -minPt.x) <dx)
	{
		maxPt.x =(minPt.x +dx);
	}
	if ((maxPt.y -minPt.y) <dy)
	{
		maxPt.y =(minPt.y +dy);
	}
	m_bounds =Box(minPt,maxPt);
}

Tpc::Geom::PointIndexerXy::~PointIndexerXy( void )
{
}

int Tpc::Geom::PointIndexerXy::composeKey( int kx, int ky )
{
	int retKey =kx;
	retKey<<=kDimSizeBits;
	retKey +=ky;

	return retKey;
}

void Tpc::Geom::PointIndexerXy::decomposeKey( int key, int& outKx, int& outKy )
{
	outKx =0;
	outKy =0;

	unsigned int kx =static_cast<unsigned int>(key);
	unsigned int ky =static_cast<unsigned int>(key);

	kx =(kx>>kDimSizeBits);
	ky -=(kx<<kDimSizeBits);

	outKx =static_cast<int>(kx);
	outKy =static_cast<int>(ky);
}

int Tpc::Geom::PointIndexerXy::getMinXKeyPart() const
{
	return getXKeyPart(m_bounds.getMinPoint());
}

int Tpc::Geom::PointIndexerXy::getMaxXKeyPart() const
{
	return getXKeyPart(m_bounds.getMaxPoint());
}

int Tpc::Geom::PointIndexerXy::getMinYKeyPart() const
{
	return getYKeyPart(m_bounds.getMinPoint());
}

int Tpc::Geom::PointIndexerXy::getMaxYKeyPart() const
{
	return getYKeyPart(m_bounds.getMaxPoint());
}

int Tpc::Geom::PointIndexerXy::getXKeyPart( const Point3d& pt ) const
{
	int kx =(int)((pt.x-m_bounds.getMinPoint().x)/m_stepX);
	TPC_ASSERT(kx >=0);
	return kx;
}

int Tpc::Geom::PointIndexerXy::getYKeyPart( const Point3d& pt ) const
{
	const int ky =(int)((pt.y-m_bounds.getMinPoint().y)/m_stepY);
	TPC_ASSERT(ky >=0);
	return ky;
}

int Tpc::Geom::PointIndexerXy::calculatePointKey( const Point3d& pt ) const
{
	if (!m_bounds.containsXy(pt))
	{
		throw Exception(L"Point is out of range.");
	}

	const int kx =getXKeyPart(pt);
	const int ky =getYKeyPart(pt);

	return composeKey(kx,ky);
}

vector<int> Tpc::Geom::PointIndexerXy::calculatePointVicinityKeys( const Point3d& pt, float vicinityRadius ) const
{	
	if (!m_bounds.containsXy(pt))
	{
		throw Exception(L"Point is out of range.");
	}

	Point3d rightPt =(pt +Point3d(vicinityRadius,0,0));
	Point3d leftPt =(pt -Point3d(vicinityRadius,0,0));
	Point3d topPt =(pt +Point3d(0,vicinityRadius,0));
	Point3d bottomPt =(pt -Point3d(0,vicinityRadius,0));

	const int kxRight =(m_bounds.containsXy(rightPt)) ? getXKeyPart(rightPt) : getMaxXKeyPart();
	const int kxLeft =(m_bounds.containsXy(leftPt)) ? getXKeyPart(leftPt) : getMinXKeyPart();
	const int kyTop =(m_bounds.containsXy(topPt)) ? getYKeyPart(topPt) : getMaxYKeyPart();
	const int kyBottom =(m_bounds.containsXy(bottomPt)) ? getYKeyPart(bottomPt) : getMinYKeyPart();

	vector<int> retVec;	
	for(int kx =kxLeft; kx <=kxRight; kx++)
	{
		for(int ky =kyBottom; ky <=kyTop; ky++)
		{
			int key =composeKey(kx,ky);
			retVec.push_back(key);
		}
	}

	return retVec;
}


vector<int> Tpc::Geom::PointIndexerXy::calculateKeyVicinityKeys( int key, float vicinityRadius ) const
{
	Point3d keyPt =calculateKeyCenterPoint(key).getXyProjection();
	
	vector<int> retVec;	
	vector<int> rawVicinityKeys =calculatePointVicinityKeys(keyPt,vicinityRadius);
	retVec.reserve(rawVicinityKeys.size());

	for(vector<int>::const_iterator i =rawVicinityKeys.begin(); i !=rawVicinityKeys.end(); i++)
	{
		Point3d iPt =calculateKeyCenterPoint(*i).getXyProjection();

		if ((keyPt -iPt).length() >vicinityRadius)
		{
			continue;
		}

		retVec.push_back(*i);
	}

	return retVec;
}


const Point3d Tpc::Geom::PointIndexerXy::calculateKeyCenterPoint( int key ) const
{
	int kx =-1;
	int ky =-1;
	decomposeKey(key,kx,ky);
	TPC_ASSERT((kx>= getMinXKeyPart()) && (kx<= getMaxXKeyPart()));
	TPC_ASSERT((ky>= getMinYKeyPart()) && (ky<= getMaxYKeyPart()));

	double dx =(double)m_stepX*(double)kx;
	dx +=(double)m_stepX*0.5;
	double dy =(double)m_stepY*(double)ky;
	dy +=(double)m_stepY*0.5;

	float x =m_bounds.getMinPoint().x +(float)dx;
	float y =m_bounds.getMinPoint().y +(float)dy;

	x =(x >m_bounds.getMaxPoint().x) ? m_bounds.getMaxPoint().x : x;
	y =(y >m_bounds.getMaxPoint().y) ? m_bounds.getMaxPoint().y : y;

	return Point3d(x,y,0);
}

bool Tpc::Geom::PointIndexerXy::operator==( const PointIndexerXy& indexer ) const
{
	if (m_bounds !=indexer.m_bounds)
	{
		return false;
	}

	return ((m_stepX==indexer.m_stepX) &&(m_stepY==indexer.m_stepY));
}

bool Tpc::Geom::PointIndexerXy::operator!=( const PointIndexerXy& indexer ) const
{
	return !(operator ==(indexer));
}