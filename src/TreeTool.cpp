#include "StdAfx.h"
#include "TreeTool.h"
#include "CloudHelper.h"
#include "PointCloud.h"
#include "Exception.h"
#include "PointCloudBuilder.h"
#include "TreeTrunkTool.h"
#include "DrawHelper.h"
#include "Cylinder.h"
#include "TreeTopTool.h"
#include <cfloat>


using namespace Tpc::Sys;
using namespace Tpc::Geom;
using namespace Tpc::Base;
using namespace Tpc::Processing;
using namespace Tpc::Ui;
using namespace std;




Tpc::Processing::TreeTool::TreeTool()
	: m_treeHeight(-1.0),
	m_trunkDiameter(-1.0),
	m_driplineDiameter(-1.0)
{
}

Tpc::Processing::TreeTool::~TreeTool()
{
}

float Tpc::Processing::TreeTool::getTreeHeight() const
{
	return m_treeHeight;
}

float Tpc::Processing::TreeTool::getTrunkDiameter() const
{
	return m_trunkDiameter;
}

float Tpc::Processing::TreeTool::getDriplineDiameter() const
{
	return m_driplineDiameter;
}

Point3d Tpc::Processing::TreeTool::estimateTreeCenterXy()
{
	CloudHelper::recalculateCloudBox(getWorkingCloud());

	PointCloudIndexXy weightsXy;
	PointCloudBuilder::buildIndexXy(getWorkingCloud(),weightsXy,Settings::getTreeCenterEstimationGridStep());
	
	PointCloudIndexXy::ConstIterator bestWeightI =weightsXy.getConstIterator();
	for(PointCloudIndexXy::ConstIterator i =bestWeightI; !i.isOutOfRangeOf(weightsXy); i.moveNext())
	{
		if (i.getCurrentData().weight >bestWeightI.getCurrentData().weight)
		{
			bestWeightI =i;
		}
	}

	if(bestWeightI.isOutOfRangeOf(weightsXy))
	{
		throw Exception(L"Can't estimate tree center.");
	}

	return bestWeightI.getCurrentData().center;
}

void Tpc::Processing::TreeTool::calculateProperties()
{
	if(getWorkingCloud().getPointCount() <50)
	{
		throw Exception(L"Too few points in cloud.");
	}
	///==============Initial step: calculate the working cloud bounding box and remove selection if is.===============
	{
		CloudHelper::recalculateCloudBox(getWorkingCloud());
		getWorkingCloud().unselectAll();
	}
	///==================Making an estimation of tree vertical centerline. The centerline, by hypothesis, is at point of max cloud density.======
	Point3d treeCenterXy; ///'Xy' suffix means everywhere that point is used as 2d, on XY plane.
	{
		treeCenterXy =estimateTreeCenterXy();

#ifdef TPC_DEBUG
	//	debugDrawCenterline(treeCenterXy);
#endif
	}
	///=================Next, feeding an 'around the centerline cylindric bar' into the TreeTrunk tool and getting the exact trunk diameter and midpoint.====
	Point3d trunkMidpoint;
	{
		///====Selecting the bar cylinder on the working cloud.======= 
		{
			const Cylinder trunkBarCylinder(	treeCenterXy,
																	getWorkingCloud().getCloudData().boundingBox.getMinPoint().z,
																	getWorkingCloud().getCloudData().boundingBox.getMaxPoint().z,
																	Settings::getTreeTrunkBarRadius());
			getWorkingCloud().unselectAll();
			CloudHelper::selectCylinder(getWorkingCloud(),trunkBarCylinder);
		}

		///======Copying the selection to trunkBar cloud and obtaining the diameter and midpoint.============
		PointCloud trunkBar;
		CloudHelper::copySelectedPointsTo(getWorkingCloud(),trunkBar);

		TreeTrunkTool trunkTool;
		trunkTool.init(&trunkBar);

		trunkTool.calculateProperties();
		m_trunkDiameter =trunkTool.getDiameter();
		trunkMidpoint =trunkTool.getMiddle();

#ifdef TPC_DEBUG
		debugDrawTrunkSection(trunkMidpoint,m_trunkDiameter);
#endif
	}

	///=================Next, feeding the cloud top part into the TreeTop tool and getting the dripline diameter and apex Z.====
	float apexZ =0.0;
	{
		///====Selecting the 'above the middle' part.======= 
		{
			getWorkingCloud().unselectAll();
			CloudHelper::selectHorizontalSlice(getWorkingCloud(),trunkMidpoint.z,FLT_MAX);
		}

		///======Copying the selection to topBar cloud and obtaining the diameter and apex.============
		PointCloud topBar;
		CloudHelper::copySelectedPointsTo(getWorkingCloud(),topBar);

		TreeTopTool topTool;
		topTool.init(&topBar);

		topTool.calculateProperties();
		m_driplineDiameter =topTool.getDriplineDiameter();
		apexZ =topTool.getApexZ();
	}

	///=================Next, determining root Z as the lowest point in the trunk vicinity and thus obtaining the three height.====
	{
		float rootZ =calculateRootZ(trunkMidpoint, (m_trunkDiameter/2.0)*Settings::getRootSearchTrunkVicinityMultiplier());
		m_treeHeight =(apexZ -rootZ);
		TPC_ASSERT(m_treeHeight >0.0);

#ifdef TPC_DEBUG
		debugDrawVerticalDimLine(trunkMidpoint,rootZ,apexZ);
#endif
	}
}

float Tpc::Processing::TreeTool::calculateRootZ( const Point3d& _trunkCenterXy, float trunkVicinityRadius ) const
{
	const Point3d trunkCenterXy =_trunkCenterXy.getXyProjection();

	float retZ =FLT_MAX;
	for(PointCloud::ConstIterator i =getWorkingCloud().getConstIterator(); !i.isOutOfRangeOf(getWorkingCloud()); i.moveNext())
	{
		const Point3d& tmpPt =i.getCurrentPoint().pt;
		if((trunkCenterXy -tmpPt.getXyProjection()).length() >trunkVicinityRadius)
		{
			continue;
		}

		if (tmpPt.z <retZ)
		{
///			DrawHelper::addPointToDb(tmpPt, 3);
			retZ =tmpPt.z;
		}
	}		

	return retZ;
}

#ifdef TPC_DEBUG
void Tpc::Processing::TreeTool::debugDrawCenterline( const Point3d& treeCenterXy ) const
{
	Point3d cptMin =treeCenterXy;
	Point3d cptMax =cptMin;
	cptMin.z =getWorkingCloud().getCloudData().boundingBox.getMinPoint().z;
	cptMax.z =getWorkingCloud().getCloudData().boundingBox.getMaxPoint().z;
	DrawHelper::addLineToDb(cptMin,cptMax);
}

void Tpc::Processing::TreeTool::debugDrawVerticalDimLine( const Point3d& trunkCenterXy, float minZ, float maxZ ) const
{
	Point3d cptMin =trunkCenterXy;
	Point3d cptMax =cptMin;
	cptMin.z =minZ;
	cptMax.z =maxZ;
	DrawHelper::addLineToDb(cptMin,cptMax);
}

void Tpc::Processing::TreeTool::debugDrawTrunkSection( const Point3d& midPt, float diam ) const
{
	DrawHelper::addCircleToDb(midPt,diam/2.0);
}
#endif
