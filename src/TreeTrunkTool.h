#pragma once


#include "CloudTool.h"
#include "Point3d.h"


namespace Tpc
{
	namespace Processing
	{
		class TreeTrunkTool : public CloudTool
		{
		private:
			float m_diameter;
			Tpc::Geom::Point3d m_middle;

#ifdef TPC_DEBUG
			void debugPrintSliceInfo(int,const Tpc::Base::PointCloud&) const;
#endif

		public:
			TreeTrunkTool();
			virtual ~TreeTrunkTool();

			float getDiameter() const;
			const Tpc::Geom::Point3d& getMiddle() const;

			void calculateProperties();
		};
	}
}
