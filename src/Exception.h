#pragma once

#include <string>

using namespace std;


#ifndef TPC_ASSERT
#ifdef TPC_DEBUG
#define TPC_ASSERT(x) assert(x)
#else
#define TPC_ASSERT(x)
#endif
#endif

namespace Tpc
{
	namespace Sys
	{
		class Exception
		{
		protected:
			wstring m_msg;

		public:
			explicit Exception(const wchar_t* pMsg =0);
			virtual ~Exception(void);

			const wstring& getMessage() const;
		};

		class IoException : public Exception
		{
		public:
			explicit IoException(const wchar_t* pMsg =0);
		};

		class ExceptionHelper
		{
		private:
			ExceptionHelper();
			~ExceptionHelper();

			static void print(const wchar_t*);

		public:
			static void catchAllExceptions();
		};
	}
}
