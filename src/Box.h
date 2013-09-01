#pragma once

#include "Point3d.h"

namespace Tpc
{
	namespace Geom
	{
		class Box
		{
		private:
			Point3d m_minPt;
			Point3d m_maxPt;

		public:
			Box(void);
			Box(const Point3d&,const Point3d&);
			~Box(void);

			void setToEmpty();

			bool isEmpty() const;

			bool operator ==(const Box&) const;
			bool operator !=(const Box&) const;

			const Point3d& getMaxPoint() const;
			const Point3d& getMinPoint() const;
			const Point3d getCenterPoint() const;
			float getDiameter() const;
			float getHeight() const;
			float getWidthX() const;
			float getWidthY() const;

			bool contains(const Point3d&) const;
			bool containsXy(const Point3d&) const;

			void append(const Point3d&);
		};
	}
}