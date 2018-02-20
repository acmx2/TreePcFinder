#pragma once

#include "Point3d.h"
#include "DataIndexT.h"
#include "PointIndexer.h"


namespace Tpc
{
	namespace Base
	{
		struct PointCell
		{
			Tpc::Geom::Point3d center;
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

		typedef DataIndexT<DataCell,Tpc::Geom::PointIndexerXy> PointCloudIndexXy;
		/// can be used: 	typedef DataIndexT<DataCell,PointIndexer3d> PointCloudIndex3d;  
	}
}
