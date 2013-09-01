#pragma once

#include "Point3d.h"
#include "Box.h"
#include "GuardedPointerVectorT.h"
#include <vector>

using namespace std;
using namespace Tpc::Geom;
using namespace Tpc::Sys;

namespace Tpc
{
	namespace Processing
	{
		class CloudHelper;
	}

	namespace Base
	{
		class PointCloudBuilder;

		class PointCloud
		{
			friend class Tpc::Base::PointCloudBuilder;
			friend class Tpc::Processing::CloudHelper;

		private:
			PointCloud(const PointCloud&); ///no copy ctr 
			PointCloud& operator =(const PointCloud&); ///no assignment op
	
		public:
			struct CloudData
			{
				Box boundingBox;
				Point3d center;
				float sigma;
				float sigmaXy;
				Point3d diameterCenterXy;
				float diameterXy;

				CloudData();

				void clear();
			};
			struct PointData
			{
#ifdef _ACRXAPP
				AcDbObjectId id;
#endif
				bool isSelected;
				float remoteness;

				PointData()
					: isSelected(false)
					, remoteness(0)
				{
				}
				void clear()
				{
#ifdef _ACRXAPP
					id =AcDbObjectId::kNull;
#endif
					isSelected =false;
					remoteness =0.0;
				}
			};
			struct Point
			{
				Point3d pt;
				PointData data;				
				inline explicit Point(const Point3d& x)
					: pt(x)
				{
				}
			};

		protected:
			typedef vector<Point> StorageType;
			typedef StorageType::const_iterator StorageConstIteratorType;
			typedef StorageType::iterator StorageIteratorType;

		protected:
			CloudData m_data;
			StorageType m_storage;

			void setSelectedFlagForAll(bool);

		public:
			template <class TI, class TP, class TD> 
			class IteratorT
			{
				friend class Tpc::Base::PointCloud;

			private:
				TI m_iter; 

			private:
				explicit inline IteratorT(const TI& iter)
					: m_iter(iter)
				{
				}

			public:
				inline ~IteratorT()
				{
				}

				inline TP& getCurrentPoint() const
				{
					return (*m_iter);
				}
				inline TD& getCurrentPointData() const
				{
					return m_iter->data;
				}
				inline bool isOutOfRangeOf(const PointCloud& cloud) const
				{
					return (m_iter ==cloud.m_storage.end());
				}
				inline void moveNext()
				{
					m_iter++;					
				}
			};

			typedef IteratorT<StorageConstIteratorType,const Point,const PointData> ConstIterator;
			typedef IteratorT<StorageIteratorType,Point,PointData> Iterator;
			typedef IteratorT<StorageIteratorType,const Point,PointData> PointDataIterator;

		protected:
			Iterator getIterator();

			static bool findNextSelectedInterval(PointCloud::StorageConstIteratorType&, PointCloud::StorageConstIteratorType&, const PointCloud::StorageConstIteratorType&, bool);
		
		public:
			PointCloud(void);
			~PointCloud(void);

			void clear();
			const CloudData& getCloudData() const;
			int getPointCount() const;

			int calculateSelectedPointsCount() const;

			ConstIterator getConstIterator() const;
			PointDataIterator getPointDataIterator();

			void selectAll();
			void unselectAll();
		};

		typedef GuardedPointerVectorT<PointCloud> PointCloudVector;
	}
}
