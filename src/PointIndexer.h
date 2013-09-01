#pragma once

#include "Box.h"
#include <vector>


using namespace std;

namespace Tpc
{
	namespace Geom
	{
		class PointIndexerXy
		{
		private:
			Box	m_bounds;
			float m_stepX;
			float m_stepY;

			static const int kDimSizeBits;

			static int composeKey(int,int);
			static void decomposeKey(int,int&,int&);
			
			int getMinXKeyPart() const;
			int getMaxXKeyPart() const;
			int getMinYKeyPart() const;
			int getMaxYKeyPart() const;

			int getXKeyPart(const Point3d&) const;
			int getYKeyPart(const Point3d&) const;

		public:
			PointIndexerXy();
			PointIndexerXy(const Box&, float, float);
			~PointIndexerXy(void);

			bool operator ==(const PointIndexerXy&) const;
			bool operator !=(const PointIndexerXy&) const;

			int calculatePointKey(const Point3d&) const;
			const Point3d calculateKeyCenterPoint(int) const;
			vector<int> calculatePointVicinityKeys(const Point3d&, float) const;
			vector<int> calculateKeyVicinityKeys(int, float) const;
		};

		// for the future: class PointIndexer3d;
	}
}
