#pragma once

#include "Point3d.h"
#include <vector>
#include <map>


namespace Tpc
{
	namespace Base
	{
		///TM is a std::map<int,TD> or an unsorted_map. Shouldn't be multimap.
		///TD is a data structure
		///TX is a PointIndexer
		template <typename TD, typename TX, typename TM = std::map<int,TD> >
		class DataIndexT
		{
		private:
			enum __dummyEnum0{__dummyElement0 =sizeof(TM)}; ///sizeof ensures that TM is already fully declared
			enum __dummyEnum1{__dummyElement1 =sizeof(TD)}; ///sizeof ensures that TD is already fully declared
			enum __dummyEnum2{__dummyElement2 =sizeof(TX)}; ///sizeof ensures that TX is already fully declared

		protected:
//			typedef map<int,TD> StorageType;
			typedef TM StorageType;
			typedef typename StorageType::const_iterator StorageConstIteratorType;
			typedef typename StorageType::iterator StorageIteratorType;

		private:
			TX m_indexer;
			StorageType m_storage;

		public:
			template <class TI, class TR> 
			class IteratorT
			{
				typedef Tpc::Base::DataIndexT<TD,TX,TM> OwnerType;
				friend class OwnerType;

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

				inline int getCurrentKey() const
				{
					return m_iter->first;
				}
				inline TR& getCurrentData() const
				{
					return m_iter->second;
				}
				inline void moveNext()
				{
					m_iter++;			
				}
				bool isOutOfRangeOf(const OwnerType& owner) const
				{
					return (m_iter ==owner.m_storage.end());
				}
			};

			typedef IteratorT<StorageConstIteratorType,const TD> ConstIterator;
			typedef IteratorT<StorageIteratorType,TD> Iterator;

		public:
			DataIndexT()
			{
			}
			explicit DataIndexT(const TX& ix)
				: m_indexer(ix)
			{
			}
			~DataIndexT(void)
			{
			}

			const TX& getIndexer() const
			{
				return m_indexer;
			}
			void setIndexer(const TX& ix)
			{
				m_indexer =ix;
			}

			void clear()
			{
				m_storage.clear();
			}

			int getCount() const
			{
				return static_cast<int>(m_storage.size());
			}			
			ConstIterator getConstIterator() const
			{
				return ConstIterator(m_storage.begin());
			}
			Iterator getIterator()
			{
				return Iterator(m_storage.begin());
			}

			ConstIterator findByKeyConst(int key) const
			{
#ifdef TPC_DEBUG
				m_indexer.calculateKeyCenterPoint(key);
#endif
				StorageConstIteratorType iter =m_storage.find(key);
				return ConstIterator(iter);
			}
			Iterator findByKey(int key)
			{
#ifdef TPC_DEBUG
				m_indexer.calculateKeyCenterPoint(key);
#endif
				StorageIteratorType iter =m_storage.find(key);
				return Iterator(iter);
			}
			ConstIterator find(const Tpc::Geom::Point3d& pt) const
			{
				int key =m_indexer.calculatePointKey(pt);
				return findByKeyConst(key);
			}
			Iterator find(const Tpc::Geom::Point3d& pt)
			{
				int key =m_indexer.calculatePointKey(pt);
				return findByKey(key);
			}
			bool contains(int key) const
			{
				return !(findByKeyConst(key).isOutOfRangeOf(*this));
			}
			bool contains(const Tpc::Geom::Point3d& pt) const
			{
				return !(find(pt).isOutOfRangeOf(*this));
			}
			std::vector<int> filterExistingKeys(const std::vector<int>& v) const
			{
				vector<int> retV;
				for (vector<int>::const_iterator i =v.begin(); i !=v.end(); i++)
				{
					if (contains(*i))
					{
						retV.push_back(*i);
					}
				}
				return retV;
			}
			Iterator insertingFind(const Tpc::Geom::Point3d& pt)
			{
				int key =m_indexer.calculatePointKey(pt);
				std::pair<StorageIteratorType,bool> retPair =m_storage.insert(StorageType::value_type(key,TD()));
				StorageIteratorType iter =retPair.first;
				return Iterator(iter);
			}
			void erase(const Iterator i)
			{
				m_storage.erase(i.m_iter);
			}
		};
	}
}
