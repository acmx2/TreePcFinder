#include "StdAfx.h"
#include "PointCloudBuilder.h"
#include "PointCloud.h"
#include "TxtReader.h"
#include "Settings.h"
#include "Box.h"
#include "PointCloudIndex.h"
#include <string>
#include <sstream>

using namespace Tpc::Base;
using namespace Tpc::Geom;
using namespace Tpc::Sys;
using namespace Tpc::Io;
using namespace std;


void Tpc::Base::PointCloudBuilder::buildCloudFromArray( PointCloud& cloud, const Point3dVector& pts )
{
	cloud.clear();

	for (Point3dVector::const_iterator i =pts.begin(); i !=pts.end(); i++)
	{
		PointCloud::Point elem(*i);
		cloud.m_storage.push_back(elem);
	}
}

void Tpc::Base::PointCloudBuilder::buildCloudFromTxtReader( PointCloud& cloud, TxtReader& reader ) throw(IoException)
{
	cloud.clear();

	string sLine;
	while(reader.readLineAndMoveNext(sLine))
	{
		if (sLine.empty())
		{
			continue;
		}

		Point3d tmpPt;
		string tmpDelim;

		istringstream line(sLine);

		line >> tmpPt.x;
		line >> tmpDelim;
		if (tmpDelim != ",")
		{
			throw IoException(L"Bad file format");
		}

		line >> tmpPt.y;
		line >> tmpDelim;
		if (tmpDelim != ",")
		{
			throw IoException(L"Bad file format");
		}

		line >> tmpPt.z;

		PointCloud::Point elem(tmpPt);
		cloud.m_storage.push_back(elem);
	}
}

void Tpc::Base::PointCloudBuilder::buildIndexXy( PointCloud& cloud, PointCloudIndexXy& index, float step, bool bForSelectedOnly /*=false*/ )
{
	index.clear();

	PointIndexerXy indexerXy(cloud.getCloudData().boundingBox,step,step);
	index.setIndexer(indexerXy);
	for(PointCloud::ConstIterator i =cloud.getConstIterator(); !i.isOutOfRangeOf(cloud); i.moveNext())
	{
		if (bForSelectedOnly && !i.getCurrentPointData().isSelected)
		{
			continue;
		}

		const PointCloud::Point& cpt =i.getCurrentPoint();
		
		PointCloudIndexXy::Iterator wi =index.insertingFind(cpt.pt);
		wi.getCurrentData().weight++;
	}		
	for(PointCloudIndexXy::Iterator i =index.getIterator(); !i.isOutOfRangeOf(index); i.moveNext())
	{
		i.getCurrentData().center =index.getIndexer().calculateKeyCenterPoint(i.getCurrentKey());
	}	
}

void Tpc::Base::PointCloudBuilder::buildCloudFromIndexXy( PointCloudIndexXy& index, PointCloud& cloud,bool bFromSelectedOnly /*=false*/ )
{
	cloud.clear();

	if (index.getCount() ==0)
	{
		return;
	}

	for(PointCloudIndexXy::Iterator i =index.getIterator(); !i.isOutOfRangeOf(index); i.moveNext())
	{
		const DataCell& cellData =i.getCurrentData();
		if (bFromSelectedOnly && !cellData.isSelected)
		{
			continue;
		}

		const Point3d& cellCenter =cellData.center;

		PointCloud::Point elem(cellCenter);
		cloud.m_storage.push_back(elem);
	}	
}


/*
void Tpc::Base::PointCloudBuilder::buildPointIndexXy(PointCloud& cloud, PointCloudIndexXy& index, float step)
{
	CloudHelper::recalculateCloudBox(cloud);

	PointIndexerXy indexerXy(cloud.getCloudData().boundingBox,step,step);
	index.setIndexer(indexerXy);
	for(PointCloud::ConstIterator i =cloud.getConstIterator(); !i.isOutOfRangeOf(cloud); i.moveNext())
	{
		const PointCloud::Point& cpt =i.getCurrentPoint();

		PointCloudIndexXy::Iterator wi =index.insertingFind(cpt.pt);
		wi.getCurrentData().points.push_back(i);
	}	
}
*/