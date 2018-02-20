#pragma once

#include "Exception.h"
#include <fstream>
#include <string>


namespace Tpc
{
	namespace Io
	{
		class TxtReader
		{
		private:
			std::ifstream m_fileStream;

		public:
			TxtReader();
			~TxtReader(void);

			void attachTo(const char*) throw (Tpc::Sys::IoException);
			bool isAttached() const;
			void detach();

			bool readLineAndMoveNext(std::string&) throw (Tpc::Sys::IoException);			
		};
	}
}
