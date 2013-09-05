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
		using Tpc::Geom::Point3d;

		class TreeTopTool : public CloudTool
		{
		private:
			float m_driplineDiameter;
			float m_apexZ;

#ifdef TPC_DEBUG
			static void debugDrawCluster(const PointCloud&, float);
			static void debugDrawDriplineCircle(const Point3d&,float);
#endif

			void calculateDriplineDiameterAndCenter(float&,Point3d&);
			float calculateTopZ(float,const Point3d&) const;

		public:
			TreeTopTool(void);
			virtual ~TreeTopTool(void);

			float getDriplineDiameter() const;
			float getApexZ() const;

			void calculateProperties();
		};
	}
}
