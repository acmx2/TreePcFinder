#pragma once

#include "Point3d.h"
#include "DataIndexT.h"
#include "PointIndexer.h"
#include <vector>
#include <cfloat>


using namespace Tpc::Geom;
using namespace std;

namespace Tpc
{
	namespace Base
	{
		struct PointCell
		{
			Point3d center;
			PointCell()
			{
			}
		};
		struct DataCell : public PointCell
		{
			int weight;
			bool isSelected;
			DataCell()
				: weight(0)
				, isSelected(false)
			{
			}
		};

		typedef DataIndexT<DataCell,PointIndexerXy> PointCloudIndexXy;
		/// not used: 	typedef DataIndexT<DataCell,PointIndexer3d> PointCloudIndex3d;  
	}
}