#include "StdAfx.h"
#include "DrawHelper.h"


using namespace Tpc::Ui;
using namespace Tpc::Geom;
using namespace Tpc::Base;

#ifdef _ACRXAPP

#define TPC_THROW_NEOK(x) {Acad::ErrorStatus eRes =(x); if(eRes!=Acad::eOk){throw Tpc::Sys::Exception(L"Exception on bad ErrorStatus");};}

AcDbObjectId Tpc::Ui::DrawHelper::addPointToDb( const Point3d& pt,int c /*=0*/ ) throw(Exception)
{
	AcDbDatabase* pDb =acdbHostApplicationServices()->workingDatabase();

	AcDbObjectPointer<AcDbPoint> ptPtr;
	TPC_THROW_NEOK(ptPtr.create());
	ptPtr->setDatabaseDefaults(pDb);

	TPC_THROW_NEOK(ptPtr->setPosition(AcGePoint3d(pt.x,pt.y,pt.z)));
	TPC_THROW_NEOK(ptPtr->setColorIndex(c));

	{
		AcDbBlockTableRecordPointer pMS (ACDB_MODEL_SPACE, pDb, AcDb::kForWrite);
		TPC_THROW_NEOK(pMS->appendAcDbEntity (ptPtr.object ())) ;
	}

	return ptPtr->objectId();
}

AcDbObjectId Tpc::Ui::DrawHelper::addCircleToDb( const Point3d& pt, float r, int c /*=0*/ ) throw(Exception)
{
	AcDbDatabase* pDb =acdbHostApplicationServices()->workingDatabase();

	AcDbObjectPointer<AcDbCircle> cPtr;
	TPC_THROW_NEOK(cPtr.create());
	cPtr->setDatabaseDefaults(pDb);

	TPC_THROW_NEOK(cPtr->setCenter(AcGePoint3d(pt.x,pt.y,pt.z)));
	TPC_THROW_NEOK(cPtr->setRadius(r));
	TPC_THROW_NEOK(cPtr->setNormal(AcGeVector3d::kZAxis));
	TPC_THROW_NEOK(cPtr->setColorIndex(c));

	{
		AcDbBlockTableRecordPointer pMS (ACDB_MODEL_SPACE, pDb, AcDb::kForWrite);
		TPC_THROW_NEOK(pMS->appendAcDbEntity (cPtr.object ())) ;
	}

	return cPtr->objectId();
}

AcDbObjectId Tpc::Ui::DrawHelper::addLineToDb( const Point3d& p1,const Point3d& p2 ) throw(Exception)
{
	AcDbDatabase* pDb =acdbHostApplicationServices()->workingDatabase();

	AcDbObjectPointer<AcDbLine> cPtr;
	TPC_THROW_NEOK(cPtr.create());
	cPtr->setDatabaseDefaults(pDb);

	TPC_THROW_NEOK(cPtr->setStartPoint(AcGePoint3d(p1.x,p1.y,p1.z)));
	TPC_THROW_NEOK(cPtr->setEndPoint(AcGePoint3d(p2.x,p2.y,p2.z)));

	{
		AcDbBlockTableRecordPointer pMS (ACDB_MODEL_SPACE, pDb, AcDb::kForWrite);
		TPC_THROW_NEOK(pMS->appendAcDbEntity (cPtr.object ())) ;
	}

	return cPtr->objectId();
}

void Tpc::Ui::DrawHelper::addCloudToDb( PointCloud& cloud, int denom, int color /*=0*/ ) throw(Exception)
{
	int c =0;
	for(PointCloud::PointDataIterator i =cloud.getPointDataIterator(); !i.isOutOfRangeOf(cloud); i.moveNext(), c++)
	{
		if (c%denom !=0)
		{
			continue;
		}

		const PointCloud::Point& cpt =i.getCurrentPoint();
		PointCloud::PointData& cpdata =i.getCurrentPointData();

		cpdata.id =DrawHelper::addPointToDb(cpt.pt,color);
	}	
}

void Tpc::Ui::DrawHelper::addZCloudToDb( PointCloud& cloud, int denom, int color, float z ) throw(Exception)
{
	int c =0;
	for(PointCloud::PointDataIterator i =cloud.getPointDataIterator(); !i.isOutOfRangeOf(cloud); i.moveNext(), c++)
	{
		if (c%denom !=0)
		{
			continue;
		}

		const PointCloud::Point& cpt =i.getCurrentPoint();
		PointCloud::PointData& cpdata =i.getCurrentPointData();

		cpdata.id =DrawHelper::addPointToDb(Point3d(cpt.pt.x,cpt.pt.y,z),color);
	}	
}

#endif