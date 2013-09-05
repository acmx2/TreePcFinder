#include "StdAfx.h"
#include "TxtReader.h"
#include "Exception.h"


using namespace Tpc::Sys;
using namespace Tpc::Io;
using namespace std;


Tpc::Io::TxtReader::TxtReader()
{
}

Tpc::Io::TxtReader::~TxtReader( void )
{
}

void Tpc::Io::TxtReader::attachTo( const char* pFileName ) throw (IoException)
{
	if (!pFileName)
	{
		throw IoException(L"Bad file name");
	}

	if (isAttached())
	{
		detach();
	}

	m_fileStream.open(pFileName);
	if (!isAttached())
	{
		throw Exception(L"Can't read file.");
	}
}

bool Tpc::Io::TxtReader::isAttached() const
{
	return m_fileStream.is_open();
}

bool Tpc::Io::TxtReader::readLineAndMoveNext( string& outLine ) throw (IoException)
{
	if (!isAttached())
	{
		return false;
	}
	if (!m_fileStream.good())
	{
		return false;
	}

	getline(m_fileStream,outLine);

	return true;
}

void Tpc::Io::TxtReader::detach()
{
	m_fileStream.close();
}

