#pragma once

#include "Exception.h"
#include <fstream>
#include <string>


using namespace Tpc::Sys;
using namespace std;

namespace Tpc
{
	namespace Io
	{
		class TxtReader
		{
		private:
			ifstream m_fileStream;

		public:
			TxtReader();
			~TxtReader(void);

			void attachTo(const char*) throw (IoException);
			bool isAttached() const;
			void detach();

			bool readLineAndMoveNext(string&) throw (IoException);			
		};
	}
}
