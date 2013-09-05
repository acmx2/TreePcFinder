#pragma once

#include <string>

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
			std::wstring m_msg;

		public:
			explicit Exception(const wchar_t* pMsg =0);
			virtual ~Exception(void);

			const std::wstring& getMessage() const;
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
