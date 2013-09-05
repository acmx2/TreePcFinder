#pragma once

#include "PointCloud.h"
#include "PointCloudIndex.h"


namespace Tpc
{
	namespace Geom
	{
		class Cylinder;
	}

	namespace Processing
	{
		using Tpc::Base::PointCloud;
		using Tpc::Base::PointCloudVector;
		using Tpc::Base::PointCloudIndexXy;
		using Tpc::Geom::Cylinder;

		class CloudHelper
		{
		private:
			CloudHelper(void);  ///no ctor
			~CloudHelper(void);  ///no dtor

			static void unselectAll(PointCloudIndexXy&);
			static void invertSelection(PointCloudIndexXy&);
			static void copySelectedCellsTo(const PointCloudIndexXy&,PointCloudIndexXy&);
			static void removeSelectedCells(PointCloudIndexXy&);
			static void selectACluster(PointCloudIndexXy&,float);

		public:
			static void recalculateCloudBox(PointCloud&);
			static void recalculateCenterAndSigma(PointCloud&);
			static void recalculateDiameterXy(PointCloud&);
			static void copySelectedPointsTo(const PointCloud&,PointCloud&);
			static void removeSelectedPoints(PointCloud&);
			static void selectHorizontalSlice(PointCloud&,float,float);
			static void selectCylinder(PointCloud&,const Cylinder&);
			static void splitIntoRepresentativeClustersXy(PointCloud&, PointCloudVector&, float, float, float, bool bForSelectedOnly =false);
			static void splitIntoHorizontalSlices(PointCloud&,PointCloudVector&,float);
		};
	}
}