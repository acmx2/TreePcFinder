#pragma once

#include "Point3d.h"
#include <vector>


using namespace std;

namespace Tpc
{
	namespace Geom
	{
		class Cylinder
		{
		private:
			Point3d m_center;
			float m_radius;
			float m_zMin;
			float m_zMax;

		public:
			Cylinder(void);
			Cylinder(const Point3d&, float, float,float);
			~Cylinder(void);
			const Point3d& getCenter() const;
			float getRadius() const;
			float getZMin() const;
			float getZMax() const;

			bool contains(const Point3d&) const;
			bool containsXy(const Point3d&) const;
		};

		typedef vector<Cylinder> CylinderVector;
	}
}
