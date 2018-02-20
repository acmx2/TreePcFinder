#pragma once


namespace Tpc
{
	namespace Sys
	{
		class Settings
		{
		private:
			Settings(void);  ///no ctor
			~Settings(void);  ///no dtor

		public:

			static float getTolerance();
			static float getTreeCenterEstimationGridStep();
			static float getTreeTrunkBarRadius();
			static float getTreeTrunkBarDiameterSearchCutoff();
			static float getTrunkDiameterSearchSliceHeight();
			static float getTrunkDiameterSearchDispersionPercantage();
			static float getTrunkDiameterSearchWeightFromMaxPeakCutoffPercantage();
			static float getDriplineSearchTolerance();
			static float getDriplineSearchClusteringThresholdDistance();
			static float getDriplineSearchMinCellWeightPercentage();
			static float getRootSearchTrunkVicinityMultiplier();
		};
	}
}

#ifndef TPC_TOL
#define TPC_TOL Tpc::Sys::Settings::getTolerance()
#endif