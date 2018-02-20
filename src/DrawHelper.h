#pragma once

#include "Exception.h"
#include "Point3d.h"
#include "PointCloud.h"


namespace Tpc
{
	namespace Ui
	{
#ifdef _ACRXAPP
		class DrawHelper
		{
		private:
			DrawHelper(void);
			~DrawHelper(void);

		public:
			static AcDbObjectId addPointToDb(const Tpc::Geom::Point3d&,int c =0) throw(Tpc::Sys::Exception);
			static AcDbObjectId addCircleToDb(const Tpc::Geom::Point3d&,float, int c =0) throw(Tpc::Sys::Exception);
			static AcDbObjectId addLineToDb(const Tpc::Geom::Point3d&,const Tpc::Geom::Point3d&) throw(Tpc::Sys::Exception);
			static void addCloudToDb(Tpc::Base::PointCloud&,int,int c =0) throw(Tpc::Sys::Exception);
			static void addZCloudToDb(Tpc::Base::PointCloud&,int,int,float) throw(Tpc::Sys::Exception);
		};
#endif
	}
}
