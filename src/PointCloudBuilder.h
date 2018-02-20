#pragma once

#include "Point3d.h"
#include "PointCloudIndex.h"


namespace Tpc
{
	namespace Io
	{
		class TxtReader;
	}

	namespace Base
	{
		class PointCloud;

		class PointCloudBuilder
		{
		private:
			PointCloudBuilder(void); /// no ctr
			~PointCloudBuilder(void); /// no dtor

		public:
			static void buildCloudFromArray(PointCloud&, const Tpc::Geom::Point3dVector&);
			static void buildCloudFromTxtReader(PointCloud&,  Tpc::Io::TxtReader&) throw(Tpc::Sys::IoException);
			static void buildIndexXy(PointCloud&,PointCloudIndexXy&,float,bool bForSelectedOnly =false);
			static void buildCloudFromIndexXy(PointCloudIndexXy&,PointCloud&,bool bFromSelectedOnly =false);
		};
	}
}
