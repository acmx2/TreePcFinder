#pragma once

#include "Point3d.h"
#include "PointCloudIndex.h"


using namespace Tpc::Sys;
using namespace Tpc::Geom;

namespace Tpc
{
	namespace Io
	{
		class TxtReader;
	}

	using namespace Tpc::Io;

	namespace Base
	{
		class PointCloud;

		class PointCloudBuilder
		{
		private:
			PointCloudBuilder(void); /// no ctr
			~PointCloudBuilder(void); /// no dtor

		public:
			static void buildCloudFromArray(PointCloud&, const Point3dVector&);
			static void buildCloudFromTxtReader(PointCloud&,  TxtReader&) throw(IoException);
			static void buildIndexXy(PointCloud&,PointCloudIndexXy&,float,bool bForSelectedOnly =false);
			static void buildCloudFromIndexXy(PointCloudIndexXy&,PointCloud&,bool bFromSelectedOnly =false);
		};
	}
}
