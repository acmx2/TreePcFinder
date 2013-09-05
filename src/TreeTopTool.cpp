#include "StdAfx.h"
#include "TreeTopTool.h"
#include "Exception.h"
#include "CloudHelper.h"
#include "PointCloud.h"
#include "Settings.h"
#include "PointCloudBuilder.h"
#include "DrawHelper.h"
#include "MathHelper.h"
#include <math.h>
#include <cfloat>


using namespace Tpc::Sys;
using namespace Tpc::Geom;
using namespace Tpc::Base;
using namespace Tpc::Processing;
using namespace Tpc::Ui;
using namespace std;


Tpc::Processing::TreeTopTool::TreeTopTool( void )
: m_driplineDiameter(-1.0)
, m_apexZ(-1.0)
{
}

Tpc::Processing::TreeTopTool::~TreeTopTool( void )
{
}

float Tpc::Processing::TreeTopTool::getDriplineDiameter() const
{
	return m_driplineDiameter;
}

float Tpc::Processing::TreeTopTool::getApexZ() const
{
	return m_apexZ;
}

void Tpc::Processing::TreeTopTool::calculateProperties()
{
	///=========obtaining top diameter and center=================== 
	Point3d topCenter;
	{
		calculateDriplineDiameterAndCenter(m_driplineDiameter,topCenter);
	}

	m_apexZ =calculateTopZ(m_driplineDiameter/2.0,topCenter);
}

void Tpc::Processing::TreeTopTool::calculateDriplineDiameterAndCenter( float& outDriplineDiameter, Point3d& outDriplineCenter )
{
	///======finding out the biggest cluster
	PointCloudVector clusters;
	PointCloud* pBiggestCluster =0;
	{
		CloudHelper::splitIntoRepresentativeClustersXy(	getWorkingCloud()
																								,clusters
																								,Settings::getDriplineSearchTolerance()
																								,Settings::getDriplineSearchClusteringThresholdDistance()
																								,Settings::getDriplineSearchMinCellWeightPercentage());

		for(int i =0; i< clusters.size(); i++)
		{
			PointCloud* pTmpCloud =clusters[i];
			if (!pBiggestCluster)
			{
				pBiggestCluster =pTmpCloud;
				continue;
			}

			if (pTmpCloud->getPointCount() >pBiggestCluster->getPointCount())
			{
				pBiggestCluster =pTmpCloud;
			}
		}
		if (!pBiggestCluster)
		{
			throw Exception(L"Clustering error.");
		}
	}

	///=========obtaining diameter and center for the biggest cluster==== 
	CloudHelper::recalculateCloudBox(*pBiggestCluster);
	CloudHelper::recalculateDiameterXy(*pBiggestCluster);

	const float planeZ =getWorkingCloud().getCloudData().boundingBox.getCenterPoint().z;

	outDriplineDiameter =pBiggestCluster->getCloudData().diameterXy;
	const Point3d& topCenterXy =pBiggestCluster->getCloudData().diameterCenterXy;
	outDriplineCenter =Point3d(topCenterXy.x,topCenterXy.y,planeZ);

#ifdef TPC_DEBUG
//	debugDrawCluster(*pBiggestCluster,planeZ);
	debugDrawDriplineCircle(outDriplineCenter,m_driplineDiameter);
#endif
}

float Tpc::Processing::TreeTopTool::calculateTopZ( float topRadius, const Point3d& topCenter ) const
{
	const Point3d topCenterXy =topCenter.getXyProjection();

	float retZ =(-FLT_MAX);
	for(PointCloud::ConstIterator i =getWorkingCloud().getConstIterator(); !i.isOutOfRangeOf(getWorkingCloud()); i.moveNext())
	{
		const Point3d& tmpPt =i.getCurrentPoint().pt;
		if((topCenterXy -tmpPt.getXyProjection()).length() >topRadius)
		{
			continue;
		}

		if (tmpPt.z >retZ)
		{
			retZ =tmpPt.z;
		}
	}		

	return retZ;
}

#ifdef TPC_DEBUG
void Tpc::Processing::TreeTopTool::debugDrawCluster( const PointCloud& cluster, float z )
{
	DrawHelper::addZCloudToDb(const_cast<PointCloud&>(cluster),1,1,z);
}

void Tpc::Processing::TreeTopTool::debugDrawDriplineCircle( const Point3d& cPt, float diam )
{
	DrawHelper::addCircleToDb(cPt,diam/2.0);
}
#endif