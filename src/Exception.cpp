#include "StdAfx.h"
#include "Exception.h"
#include <exception>
#include <new>


using namespace Tpc::Sys;
using namespace std;

Tpc::Sys::Exception::Exception( const wchar_t* pMsg /*=0*/ )
{
	if (pMsg)
	{
		m_msg =(pMsg);
	}
}

Tpc::Sys::Exception::~Exception( void )
{
}

const wstring& Tpc::Sys::Exception::getMessage() const
{
	return m_msg;
}

Tpc::Sys::IoException::IoException( const wchar_t* pMsg /*=0*/ )
{
	if (pMsg)
	{
		m_msg =(pMsg);
	}
	else
	{
		m_msg =L"IO Exception.";
	}
}

void Tpc::Sys::ExceptionHelper::catchAllExceptions()
{
	try
	{
		throw;
	}
	catch(std::bad_alloc&)
	{
		print(L"\n");
		print(L"Bad alloc caught.");
		print(L" Exit.\n");
	}
	catch(std::exception&)
	{
		print(L"\n");
		print(L"Std::exception caught.");
		print(L" Exit.\n");
	}
	catch (const Tpc::Sys::Exception& e)
	{
		print(L"\n");
		print(e.getMessage().c_str());
		print(L" Exit.\n");
	}
	catch (...) 
	{
		print(L"\nUNEXPECTED EXCEPTION. EXIT.\n");
	}
}

void Tpc::Sys::ExceptionHelper::print( const wchar_t* pMsg )
{
	const wchar_t* pToPrint = (pMsg) ? pMsg : L"";
#ifdef _ACRXAPP
	acutPrintf(pToPrint);
#else
	std::wcout << pToPrint;
#endif
}
