#pragma once

#include "CloudTool.h"


namespace Tpc
{
	namespace Geom
	{
		class Point3d;
	}

	namespace Processing
	{
		///Owns slices, doesn't own working cloud.
		class TreeTool : public CloudTool
		{
		private:
			float m_treeHeight;
			float m_trunkDiameter;
			float m_driplineDiameter;

			Point3d estimateTreeCenterXy();
			float calculateRootZ(const Tpc::Geom::Point3d&, float) const;

#ifdef TPC_DEBUG
			void debugDrawCenterline(const Tpc::Geom::Point3d&) const;
			void debugDrawTrunkSection(const Tpc::Geom::Point3d&,float) const;
			void debugDrawVerticalDimLine(const Tpc::Geom::Point3d&,float,float) const;
#endif

		public:
			TreeTool();
			virtual ~TreeTool();

			float getTreeHeight() const;
			float getTrunkDiameter() const;
			float getDriplineDiameter() const;

			void calculateProperties();
		};
	}
}
