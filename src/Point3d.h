#pragma once

#include "Settings.h"
#include <vector>

using namespace Tpc::Sys;
using namespace std;

namespace Tpc
{
	namespace Geom
	{
		struct Point3d
		{
			float x;
			float y;
			float z;

			static const Point3d kOrigin;
			static const Point3d kPosInfinity;
			static const Point3d kNegInfinity;

			inline Point3d()
				: x(0)
				, y(0)
				, z(0)
			{
			}
			inline Point3d(float ptx, float pty, float ptz)
				: x(ptx)
				, y(pty)
				, z(ptz)
			{
			}
			inline void setToOrigin()
			{
				(*this) =kOrigin;
			}
			inline float dotProduct(const Point3d& pt) const
			{
				return x*pt.x +y*pt.y +x*pt.z;
			}
			inline float lengthSquare() const
			{
				return dotProduct(*this);
			}
			inline Point3d operator +(const Point3d& pt) const
			{
				return Point3d(x+pt.x, y+pt.y, z+pt.z);
			}
			inline Point3d operator -(const Point3d& pt) const
			{
				return Point3d(x-pt.x, y-pt.y, z-pt.z);
			}
			inline Point3d& operator +=(const Point3d& pt)
			{
				x+=pt.x;
				y+=pt.y;
				z+=pt.z;
				return (*this);
			}
			inline Point3d operator -=(const Point3d& pt)
			{
				x-=pt.x;
				y-=pt.y;
				z-=pt.z;
				return (*this);
			}
			inline Point3d operator -() const
			{
				return Point3d(-x, -y, -z);
			}
			friend Point3d operator *(float, const Point3d&);
			friend Point3d operator /(const Point3d& pt,float);

			Point3d crossProduct(const Point3d& pt) const;
			float length() const;

			Point3d getXyProjection() const;

			bool isEqualTo(const Point3d&, float tol =TPC_TOL) const;

			bool operator ==(const Point3d&) const;
		};

		inline Point3d operator *(float c, const Point3d& pt)
		{
			return Point3d(c*pt.x, c*pt.y, c*pt.z);
		}
		inline Point3d operator /(const Point3d& pt,float c)
		{
			return Point3d(pt.x/c, pt.y/c, pt.z/c);
		}

		typedef vector<Point3d> Point3dVector;
	}
}
