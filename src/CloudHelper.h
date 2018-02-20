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
		class CloudHelper
		{
		private:
			CloudHelper(void);  ///no ctor
			~CloudHelper(void);  ///no dtor

			static void unselectAll(Tpc::Base::PointCloudIndexXy&);
			static void invertSelection(Tpc::Base::PointCloudIndexXy&);
			static void copySelectedCellsTo(const Tpc::Base::PointCloudIndexXy&,Tpc::Base::PointCloudIndexXy&);
			static void removeSelectedCells(Tpc::Base::PointCloudIndexXy&);
			static void selectACluster(Tpc::Base::PointCloudIndexXy&,float);

		public:
			static void recalculateCloudBox(Tpc::Base::PointCloud&);
			static void recalculateCenterAndSigma(Tpc::Base::PointCloud&);
			static void recalculateDiameterXy(Tpc::Base::PointCloud&);
			static void copySelectedPointsTo(const Tpc::Base::PointCloud&,Tpc::Base::PointCloud&);
			static void removeSelectedPoints(Tpc::Base::PointCloud&);
			static void selectHorizontalSlice(Tpc::Base::PointCloud&,float,float);
			static void selectCylinder(Tpc::Base::PointCloud&,const Tpc::Geom::Cylinder&);
			static void splitIntoRepresentativeClustersXy(Tpc::Base::PointCloud&, Tpc::Base::PointCloudVector&, float, float, float, bool bForSelectedOnly =false);
			static void splitIntoHorizontalSlices(Tpc::Base::PointCloud&,Tpc::Base::PointCloudVector&,float);
		};
	}
}
