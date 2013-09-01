#include "StdAfx.h"
#include "Settings.h"


using namespace Tpc::Sys;


float Tpc::Sys::Settings::getTolerance()
{
	return 1.0E-05;
}

float Tpc::Sys::Settings::getTrunkDiameterSearchSliceHeight()
{
	return 0.5;
}

float Tpc::Sys::Settings::getTreeCenterEstimationGridStep()
{
	return 1.5;
}

float Tpc::Sys::Settings::getTreeTrunkBarRadius()
{
	return 3.5;
}

float Tpc::Sys::Settings::getTreeTrunkBarDiameterSearchCutoff()
{
	return 2.0*Settings::getTreeTrunkBarRadius()*0.64; ///0.64 is due to it doesn't make sense to assume real diameter is greater than 64% of maximum limit
}

float Tpc::Sys::Settings::getTrunkDiameterSearchDispersionPercantage()
{
	return 0.15;
}

float Tpc::Sys::Settings::getTrunkDiameterSearchWeightFromMaxPeakCutoffPercantage()
{
	return 0.6;
}

float Tpc::Sys::Settings::getDriplineSearchClusteringThresholdDistance()
{
	return 3.0;
}

float Tpc::Sys::Settings::getDriplineSearchMinCellWeightPercentage()
{
	return 0.1;
}

float Tpc::Sys::Settings::getRootSearchTrunkVicinityMultiplier()
{
	return 2.0;
}

float Tpc::Sys::Settings::getDriplineSearchTolerance()
{
	return 0.5;  ///should be at least 2.5 less than getDriplineSearchClusteringThresholdDistance
}
