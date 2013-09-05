#include "StdAfx.h"
#include "CloudTool.h"
#include "Exception.h"


using namespace Tpc::Sys;
using namespace Tpc::Base;
using namespace Tpc::Processing;


Tpc::Processing::CloudTool::CloudTool( void )
: m_pWorkingCloud(0)
{
}

Tpc::Processing::CloudTool::~CloudTool( void )
{
}

void Tpc::Processing::CloudTool::init( PointCloud* pCloud )
{
	m_pWorkingCloud =pCloud;
}

bool Tpc::Processing::CloudTool::isInitialized() const
{
	return (m_pWorkingCloud);
}

const PointCloud& Tpc::Processing::CloudTool::getWorkingCloud() const
{
	TPC_ASSERT(isInitialized());
	return (*m_pWorkingCloud);
}

PointCloud& Tpc::Processing::CloudTool::getWorkingCloud()
{
	TPC_ASSERT(isInitialized());
	return (*m_pWorkingCloud);
}
