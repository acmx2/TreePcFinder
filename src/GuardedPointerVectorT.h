#pragma once

#include <vector>
#include <algorithm>

using namespace std;

namespace Tpc
{
	namespace Sys
	{
		template <class T>
		class GuardedPointerVectorT
		{
		private:
			enum __dummyEnum{__dummyElement =sizeof(T)}; ///sizeof ensures that T is already fully declared

			GuardedPointerVectorT(const GuardedPointerVectorT&); ///no copy ctor
			GuardedPointerVectorT& operator =(const GuardedPointerVectorT&); ///no assignment op

		public:
			typedef vector<T*> VectorType;

		protected:
			VectorType m_vector;

		public:
			GuardedPointerVectorT()
			{
			}
			~GuardedPointerVectorT()
			{
				clear();
			}
			void clear()
			{
				VectorType::iterator i =m_vector.begin();
				for( ; i !=m_vector.end(); i++)
				{
					T* tmp = (*i);
					if (tmp)
					{
						delete tmp;
					}
				}
				m_vector.clear();
			}
			int size() const
			{
				return static_cast<int>(m_vector.size());
			}
			void append(T* val)
			{
				if (std::find(m_vector.begin(),m_vector.end(),val) !=m_vector.end())
				{
					return;
				}
				m_vector.push_back(val);
			}
			void remove(T* val)
			{
				typename VectorType::iterator i =std::find(m_vector.begin(), m_vector.end(),val);
				if (i == m_vector.end())
				{
					return;
				}
				m_vector.erase(i);
			}
			void reserveOnePlace()
			{
				m_vector.reserve(m_vector.size() +1);
			}
			void reserve(int c)
			{
				m_vector.reserve(c);
			}
			typename VectorType::const_iterator getBegin() const
			{
				return m_vector.begin();
			}
			typename VectorType::const_iterator getEnd() const
			{
				return m_vector.end();
			}
			typename VectorType::iterator getBegin()
			{
				return m_vector.begin();
			}
			typename VectorType::iterator getEnd()
			{
				return m_vector.end();
			}
			T* operator[] (int i)
			{
				return m_vector[i];
			}
			const T* operator[] (int i) const
			{
				return m_vector[i];
			}
			T& getRefAt(int i)
			{
				TPC_ASSERT(m_vector[i]);
				return (*(m_vector[i]));
			}
			const T& getConstRefAt(int i) const
			{
				TPC_ASSERT(m_vector[i]);
				return (*(m_vector[i]));
			}
		};
	}
}
