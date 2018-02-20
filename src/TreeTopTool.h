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
		class TreeTopTool : public CloudTool
		{
		private:
			float m_driplineDiameter;
			float m_apexZ;

#ifdef TPC_DEBUG
			static void debugDrawCluster(const Tpc::Base::PointCloud&, float);
			static void debugDrawDriplineCircle(const Tpc::Geom::Point3d&,float);
#endif

			void calculateDriplineDiameterAndCenter(float&,Tpc::Geom::Point3d&);
			float calculateTopZ(float,const Tpc::Geom::Point3d&) const;

		public:
			TreeTopTool(void);
			virtual ~TreeTopTool(void);

			float getDriplineDiameter() const;
			float getApexZ() const;

			void calculateProperties();
		};
	}
}
