#include "StdAfx.h"
#include "UiCommands.h"
#include "Exception.h"
#include "PointCloudBuilder.h"
#include "PointCloud.h"
#include "TxtReader.h"
#include "TreeTool.h"
#include "Settings.h"
#include "DrawHelper.h"
#include <string>

using namespace Tpc::Base;
using namespace Tpc::Geom;
using namespace Tpc::Sys;
using namespace Tpc::Io;
using namespace Tpc::Ui;
using namespace Tpc::Processing;
using namespace std;



void Tpc::Ui::Commands::understandTree(const char* pFileName)
{
	try
	{
		TPC_ASSERT(pFileName);

		TxtReader reader;
		reader.attachTo(pFileName);

		PointCloud cloud;
		PointCloudBuilder::buildCloudFromTxtReader(cloud,reader);

#ifdef _ACRXAPP
		DrawHelper::addCloudToDb(cloud,1);	
#endif

		TreeTool tool;
		tool.init(&cloud);

		tool.calculateProperties();

		printTreeProperties(tool.getTreeHeight(),tool.getDriplineDiameter(),tool.getTrunkDiameter());
	}
	catch (...)
	{
		ExceptionHelper::catchAllExceptions();
	}
}

void Tpc::Ui::Commands::printTreeProperties( float height, float driplineDiam, float trunkDiam )
{
#ifdef _ACRXAPP
	acutPrintf(L"\nTree parameters are calculated: \nHeight: %f \nDripline diameter: %f \nTrunk diameter: %f \nDone.",height,driplineDiam,trunkDiam);
#else
	std::cout << "\nTree parameters are calculated: \nHeight: " <<  height <<"\nDripline diameter: " << driplineDiam << "\nTrunk diameter: " << trunkDiam << "\nDone.";
#endif
}
