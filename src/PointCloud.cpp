#include "StdAfx.h"
#include "PointCloud.h"
#include "Exception.h"
#include <cfloat>

using namespace Tpc::Base;
using namespace Tpc::Geom;
using namespace std;


Tpc::Base::PointCloud::CloudData::CloudData()
: sigma(-1.0)
, sigmaXy(-1.0)
, diameterXy(-1.0)
{
	boundingBox.setToEmpty();
}

void Tpc::Base::PointCloud::CloudData::clear()
{
	boundingBox.setToEmpty();
	center.setToOrigin();
	sigma =-1.0;
	sigmaXy =-1.0;
	diameterCenterXy.setToOrigin();
	diameterXy =-1.0;
}

Tpc::Base::PointCloud::PointCloud( void )
{
}

Tpc::Base::PointCloud::~PointCloud( void )
{
	clear();
}

void Tpc::Base::PointCloud::clear()
{
	m_data.clear();
	m_storage.clear();
}

Tpc::Base::PointCloud::ConstIterator Tpc::Base::PointCloud::getConstIterator() const
{
	return ConstIterator(m_storage.begin());
}

Tpc::Base::PointCloud::Iterator Tpc::Base::PointCloud::getIterator()
{
	return Iterator(m_storage.begin());
}

Tpc::Base::PointCloud::PointDataIterator Tpc::Base::PointCloud::getPointDataIterator()
{
	return PointDataIterator(m_storage.begin());
}

const Tpc::Base::PointCloud::CloudData& Tpc::Base::PointCloud::getCloudData() const
{
	return m_data;
}

int Tpc::Base::PointCloud::calculateSelectedPointsCount() const
{
	int nRet =0;
	for(ConstIterator i =getConstIterator(); !i.isOutOfRangeOf(*this); i.moveNext())
	{
		const PointCloud::Point& cpt =i.getCurrentPoint();

		if (cpt.data.isSelected)
		{
			nRet++;
		}
	}			

	return nRet;
}

void Tpc::Base::PointCloud::setSelectedFlagForAll( bool bSelected )
{
	for(PointDataIterator i =getPointDataIterator(); !i.isOutOfRangeOf(*this); i.moveNext())
	{
		PointCloud::PointData& cpdata =i.getCurrentPointData();

		cpdata.isSelected =bSelected;
	}		
}

void Tpc::Base::PointCloud::selectAll()
{
	setSelectedFlagForAll(true);
}

void Tpc::Base::PointCloud::unselectAll()
{
	setSelectedFlagForAll(false);
}

bool Tpc::Base::PointCloud::findNextSelectedInterval( PointCloud::StorageConstIteratorType& itrvBegIter, PointCloud::StorageConstIteratorType& itrvEndIter, const PointCloud::StorageConstIteratorType& storageEndIter, bool bSelFlag )
{
	while ((itrvBegIter !=storageEndIter) && (itrvBegIter->data.isSelected !=bSelFlag))
	{
		itrvBegIter++;
	}
	if (itrvBegIter ==storageEndIter)
	{
		return false;
	}

	itrvEndIter =itrvBegIter;
	while ((itrvEndIter !=storageEndIter) && (itrvEndIter->data.isSelected ==bSelFlag))
	{
		itrvEndIter++;
	}

	return true;
}

int Tpc::Base::PointCloud::getPointCount() const
{
	return static_cast<int>(m_storage.size());
}
