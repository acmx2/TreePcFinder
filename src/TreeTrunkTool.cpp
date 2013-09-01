#include "StdAfx.h"
#include "TreeTrunkTool.h"
#include "CloudHelper.h"
#include "Point3d.h"
#include "PointCloud.h"
#include "Exception.h"
#include "Settings.h"
#include "PointCloudBuilder.h"
#include "DrawHelper.h"
#include "MathHelper.h"
#include <math.h>


using namespace Tpc::Sys;
using namespace Tpc::Geom;
using namespace Tpc::Base;
using namespace Tpc::Processing;
using namespace Tpc::Ui;
using namespace std;


Tpc::Processing::TreeTrunkTool::TreeTrunkTool()
: m_diameter(0.0)
{
}

Tpc::Processing::TreeTrunkTool::~TreeTrunkTool()
{
}

void Tpc::Processing::TreeTrunkTool::calculateProperties()
{
	///====splitting into slices====================
	PointCloudVector slices;
	CloudHelper::splitIntoHorizontalSlices(getWorkingCloud(),slices,Settings::getTrunkDiameterSearchSliceHeight());
	if (slices.size() <5)
	{
		throw Exception(L"Too low tree or improper scale.");
	}

	///==============calculating slice diameters===========
	vector<float> diameters;
	for (int i =0; i< slices.size(); i++)
	{
		CloudHelper::recalculateCloudBox(slices.getRefAt(i));
		CloudHelper::recalculateDiameterXy(slices.getRefAt(i));

		const PointCloud::CloudData& cdat =slices.getConstRefAt(i).getCloudData();
		diameters.push_back(cdat.diameterXy);

#ifdef TPC_DEBUG
		debugPrintSliceInfo(i, slices.getConstRefAt(i));
#endif
	}

	///=============estimating trunk diameter==========
	diameters =MathHelper::truncateFunction(	diameters
																					,Settings::getTreeTrunkBarDiameterSearchCutoff());
	m_diameter =MathHelper::calculatePlateauValue(	diameters
																								,Settings::getTreeTrunkBarDiameterSearchCutoff()
																								,Settings::getTrunkDiameterSearchDispersionPercantage()
																								,Settings::getTrunkDiameterSearchWeightFromMaxPeakCutoffPercantage());

	///============estimating trunk middle================
	int bestSliceI =MathHelper::calculateBestPositionForValue(diameters, m_diameter);
	TPC_ASSERT(bestSliceI >=0);
	Point3d midXy =slices.getConstRefAt(bestSliceI).getCloudData().diameterCenterXy;
	float midZ =slices.getConstRefAt(bestSliceI).getCloudData().boundingBox.getCenterPoint().z;
	m_middle =Point3d(midXy.x,midXy.y,midZ);
}

float Tpc::Processing::TreeTrunkTool::getDiameter() const
{
	return m_diameter;
}

const Point3d& Tpc::Processing::TreeTrunkTool::getMiddle() const
{
	return m_middle;
}

#ifdef TPC_DEBUG
void Tpc::Processing::TreeTrunkTool::debugPrintSliceInfo( int i, const PointCloud& slice ) const
{
	const PointCloud::CloudData& cdat =slice.getCloudData();
	acutPrintf(L"\nNum: %d\t, Points: %d\t, Diam: %f\t", i,slice.getPointCount(),cdat.diameterXy);
}
#endif

