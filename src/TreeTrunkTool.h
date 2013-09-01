#pragma once


#include "CloudTool.h"
#include "Point3d.h"


using namespace Tpc::Geom;


namespace Tpc
{
	namespace Processing
	{
		class TreeTrunkTool : public CloudTool
		{
		private:
			float m_diameter;
			Point3d m_middle;

#ifdef TPC_DEBUG
			void debugPrintSliceInfo(int,const PointCloud&) const;
#endif

		public:
			TreeTrunkTool();
			virtual ~TreeTrunkTool();

			float getDiameter() const;
			const Point3d& getMiddle() const;

			void calculateProperties();
		};
	}
}
