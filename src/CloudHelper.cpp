#include "StdAfx.h"
#include "CloudHelper.h"
#include "Point3d.h"
#include "PointCloud.h"
#include "Exception.h"
#include "PointCloudBuilder.h"
#include "Cylinder.h"
#include <vector>
#include <set>
#include <algorithm>
#include <math.h>
#include <cfloat>

using namespace Tpc::Sys;
using namespace Tpc::Geom;
using namespace Tpc::Base;
using namespace Tpc::Processing;
using namespace std;


void Tpc::Processing::CloudHelper::recalculateCloudBox( PointCloud& cloud )
{
	cloud.m_data.boundingBox.setToEmpty();

	for(PointCloud::ConstIterator i =cloud.getConstIterator(); !i.isOutOfRangeOf(cloud); i.moveNext())
	{
		const PointCloud::Point& cpt =i.getCurrentPoint();

		cloud.m_data.boundingBox.append(cpt.pt);
	}		
}

void Tpc::Processing::CloudHelper::copySelectedPointsTo( const PointCloud& srcCloud, PointCloud& targetCloud )
{
	int nSel =srcCloud.calculateSelectedPointsCount();
	TPC_ASSERT(nSel >=0);

	if (nSel ==0)
	{
		return;
	}

	targetCloud.m_storage.reserve(targetCloud.m_storage.size() +nSel);

	PointCloud::StorageConstIteratorType selIntervalBeginIter =srcCloud.m_storage.begin();
	PointCloud::StorageConstIteratorType selIntervalEndIter =selIntervalBeginIter;
	while(PointCloud::findNextSelectedInterval(selIntervalBeginIter,selIntervalEndIter,srcCloud.m_storage.end(),true))
	{
		///copying the selected interval to the end of target
		targetCloud.m_storage.insert(targetCloud.m_storage.end(),selIntervalBeginIter,selIntervalEndIter);

		///moving the begin iterator to the end iterator position to continue seek of the selected intervals
		selIntervalBeginIter =selIntervalEndIter;
	}
}

void Tpc::Processing::CloudHelper::removeSelectedPoints( PointCloud& cloud )
{
	int nSel =cloud.calculateSelectedPointsCount();
	TPC_ASSERT(nSel >=0);

	if (nSel ==0)
	{
		return;
	}

	int nNewSize =(cloud.getPointCount() -nSel);
	if (nNewSize ==0)
	{
		cloud.clear();
		return;
	}

	PointCloud::StorageType newStorage;
	newStorage.reserve(nNewSize);
	PointCloud::StorageIteratorType newStorageEndIter =newStorage.end();

	PointCloud::StorageConstIteratorType nselIntervalBeginIter =cloud.m_storage.begin();
	PointCloud::StorageConstIteratorType nselIntervalEndIter =nselIntervalBeginIter;
	while(PointCloud::findNextSelectedInterval(nselIntervalBeginIter,nselIntervalEndIter,cloud.m_storage.end(),false))
	{
		///copying the not-selected interval to the end of target
		newStorage.insert(newStorage.end(),nselIntervalBeginIter,nselIntervalEndIter);

		///moving the begin iterator to the end iterator position to continue seek of the selected intervals
		nselIntervalBeginIter =nselIntervalEndIter;
	}

	///swapping the new storage with the old
	cloud.m_storage.swap(newStorage);
}

void Tpc::Processing::CloudHelper::selectHorizontalSlice( PointCloud& cloud, float zMin, float zMax )
{
	for(PointCloud::PointDataIterator i =cloud.getPointDataIterator(); !i.isOutOfRangeOf(cloud); i.moveNext())
	{
		const PointCloud::Point& cpt =i.getCurrentPoint();
		if ((cpt.pt.z <zMin) || (cpt.pt.z >zMax))
		{
			continue;
		}

		i.getCurrentPointData().isSelected =true;
	}		
}

void Tpc::Processing::CloudHelper::recalculateCenterAndSigma( PointCloud& cloud )
{
	if (cloud.getPointCount() ==0)
	{
		cloud.m_data.center.setToOrigin();
		cloud.m_data.sigma =0.0;
		cloud.m_data.sigmaXy =0.0;
		return;
	}

	Point3d startPoint =cloud.getConstIterator().getCurrentPoint().pt;
	double sumX =0.0;
	double sumY =0.0;
	double sumZ =0.0;
	for(PointCloud::ConstIterator i =cloud.getConstIterator(); !i.isOutOfRangeOf(cloud); i.moveNext())
	{
		const PointCloud::Point& cpt =i.getCurrentPoint();	
		sumX +=(cpt.pt.x -startPoint.x);
		sumY +=(cpt.pt.y  -startPoint.y);
		sumZ +=(cpt.pt.z  -startPoint.z);
	}		
	sumX /=((double)cloud.getPointCount());
	sumY /=((double)cloud.getPointCount());
	sumZ /=((double)cloud.getPointCount());
	cloud.m_data.center =(startPoint +Point3d(sumX,sumY,sumZ));

	double deltaSqSum =0.0;
	double deltaSqSumXy =0.0;
	for(PointCloud::ConstIterator i =cloud.getConstIterator(); !i.isOutOfRangeOf(cloud); i.moveNext())
	{
		const PointCloud::Point& cpt =i.getCurrentPoint();
		
		Point3d delta =(cloud.m_data.center -cpt.pt);

		const float deltaSq =delta.lengthSquare();
		deltaSqSum +=deltaSq;

		const float deltaSqXy =delta.getXyProjection().lengthSquare();
		deltaSqSumXy +=deltaSqXy;
	}		
	cloud.m_data.sigma =sqrtf(deltaSqSum/((double)cloud.getPointCount()));
	cloud.m_data.sigmaXy =sqrtf(deltaSqSumXy/((double)cloud.getPointCount()));
}

void Tpc::Processing::CloudHelper::recalculateDiameterXy( PointCloud& cloud )
{
	///easy optimization is needed here: to select the cloud boundary points via IndexXy and to iterate through them only 
	cloud.m_data.diameterCenterXy.setToOrigin();
	cloud.m_data.diameterXy =0.0;
	if (cloud.getPointCount() ==0)
	{
		return;
	}
	cloud.m_data.diameterCenterXy =cloud.getConstIterator().getCurrentPoint().pt.getXyProjection();

	for(PointCloud::ConstIterator i =cloud.getConstIterator(); !i.isOutOfRangeOf(cloud); i.moveNext())
	{
		Point3d iPtXy =i.getCurrentPoint().pt.getXyProjection();
		for(PointCloud::ConstIterator j =i; !j.isOutOfRangeOf(cloud); j.moveNext())
		{
			Point3d jPtXy =j.getCurrentPoint().pt.getXyProjection();
			double d =(jPtXy -iPtXy).length();
			if (d >cloud.m_data.diameterXy)
			{
				cloud.m_data.diameterXy =d;
				cloud.m_data.diameterCenterXy =((iPtXy +jPtXy)/2.0);
			}
		}		
	}		
}

void Tpc::Processing::CloudHelper::selectCylinder( PointCloud& cloud, const Cylinder& cylinder)
{
	for(PointCloud::PointDataIterator cIter =cloud.getPointDataIterator(); !cIter.isOutOfRangeOf(cloud); cIter.moveNext())
	{
		if (cylinder.contains(cIter.getCurrentPoint().pt))
		{
			cIter.getCurrentPointData().isSelected =true;
		}
	}
}

void Tpc::Processing::CloudHelper::splitIntoRepresentativeClustersXy(PointCloud& srcCloud, PointCloudVector& outClusters, float representativeGridStep, float clusterCutoffDistance, float cellWeightNotLessPercentage, bool bForSelectedOnly /*=false*/)
{
	outClusters.clear();

	recalculateCloudBox(srcCloud);

	PointCloudIndexXy indexXy;
	PointCloudBuilder::buildIndexXy(srcCloud,indexXy,representativeGridStep,bForSelectedOnly);  ///grid cell size is cutoffDistance/2.5 - to avoid improper clustering

	///calculating mean weight and lower cutoff weight, erasing too light cells
	{
		int weightSum =0;
		for(PointCloudIndexXy::ConstIterator i =indexXy.getConstIterator(); !i.isOutOfRangeOf(indexXy); i.moveNext())
		{
			const int w =i.getCurrentData().weight;
			weightSum +=w;
		}
		const int lowerCutoffWeight =(int)((((double)weightSum)/((double)indexXy.getCount()))*cellWeightNotLessPercentage);

		unselectAll(indexXy);
		for(PointCloudIndexXy::Iterator i =indexXy.getIterator(); !i.isOutOfRangeOf(indexXy); i.moveNext())
		{
			const int w =i.getCurrentData().weight;
			if (w <lowerCutoffWeight)
			{
				i.getCurrentData().isSelected =true;
			}
		}
		removeSelectedCells(indexXy);
	}

	int nCounter =0;
	while(indexXy.getCount() >0)
	{
		if ((++nCounter) >1000)
		{
			throw Exception(L"Too many clusters created.");
		}

		selectACluster(indexXy,clusterCutoffDistance);

		outClusters.reserveOnePlace();
		PointCloud* pNewCloud =new PointCloud;
		if (!pNewCloud)
		{
			throw Exception(L"Can't allocate new cloud.");
		}
		outClusters.append(pNewCloud);

		PointCloudBuilder::buildCloudFromIndexXy(indexXy,*pNewCloud,true);

		removeSelectedCells(indexXy);
	}

}

void Tpc::Processing::CloudHelper::unselectAll( PointCloudIndexXy& index )
{
	for(PointCloudIndexXy::Iterator i =index.getIterator(); !i.isOutOfRangeOf(index); i.moveNext())
	{
		i.getCurrentData().isSelected =false;
	}
}

void Tpc::Processing::CloudHelper::invertSelection( PointCloudIndexXy& index )
{
	for(PointCloudIndexXy::Iterator i =index.getIterator(); !i.isOutOfRangeOf(index); i.moveNext())
	{
		i.getCurrentData().isSelected =!(i.getCurrentData().isSelected);
	}
}

void Tpc::Processing::CloudHelper::copySelectedCellsTo( const PointCloudIndexXy& src, PointCloudIndexXy& target )
{
	if (src.getIndexer() !=target.getIndexer())
	{
		throw Exception(L"Can't copy cells to index with different indexer");
	}

	for(PointCloudIndexXy::ConstIterator i =src.getConstIterator(); !i.isOutOfRangeOf(src); i.moveNext())
	{
		if(i.getCurrentData().isSelected)
		{
			PointCloudIndexXy::Iterator insertedI =target.insertingFind(src.getIndexer().calculateKeyCenterPoint(i.getCurrentKey()));
			TPC_ASSERT(i.getCurrentKey() ==insertedI.getCurrentKey());

			insertedI.getCurrentData() =i.getCurrentData();
		}
	}

	unselectAll(target);
}

void Tpc::Processing::CloudHelper::removeSelectedCells( PointCloudIndexXy& index )
{
	PointCloudIndexXy::Iterator i =index.getIterator();
	while(!i.isOutOfRangeOf(index))
	{
		if (i.getCurrentData().isSelected)
		{
			///can erase this way and continue iterating:
			//C++98: Iterators, pointers and references referring to elements removed by the function are invalidated.
			//All other iterators, pointers and references keep their validity.

			PointCloudIndexXy::Iterator ei =i;
			i.moveNext();
			index.erase(ei); 
		}
		else
		{
			i.moveNext();
		}
	}
}

void Tpc::Processing::CloudHelper::selectACluster( PointCloudIndexXy& index, float clusterCutoffDistance )
{
	if (index.getCount() ==0)
	{
		return;
	}

	unselectAll(index);

	PointCloudIndexXy::Iterator iter =index.getIterator();
	iter.getCurrentData().isSelected =true;
	vector<int> initBoundaryKeys =index.filterExistingKeys(index.getIndexer().calculateKeyVicinityKeys(iter.getCurrentKey(),clusterCutoffDistance));
	set<int> selBoundaryKeys;
	selBoundaryKeys.insert(initBoundaryKeys.begin(),initBoundaryKeys.end());
	while(selBoundaryKeys.size() >0)
	{
		const int boundaryKey =(*(selBoundaryKeys.begin()));
		selBoundaryKeys.erase(selBoundaryKeys.begin());

		iter =index.findByKey(boundaryKey);
		if (iter.isOutOfRangeOf(index))
		{
			continue;
		}
		if (iter.getCurrentData().isSelected)
		{
			continue;
		}

		iter.getCurrentData().isSelected =true;

		vector<int> tmpBoundaryKeys =index.filterExistingKeys(index.getIndexer().calculateKeyVicinityKeys(iter.getCurrentKey(),clusterCutoffDistance));
		for(int j =0; j< tmpBoundaryKeys.size(); j++)
		{
			const int tkey =tmpBoundaryKeys[j];
			PointCloudIndexXy::ConstIterator jter =index.findByKeyConst(tkey);
			TPC_ASSERT(!jter.isOutOfRangeOf(index));
			if (jter.getCurrentData().isSelected)
			{
				continue;
			}

			if (selBoundaryKeys.find(tkey) !=selBoundaryKeys.end())
			{
				continue;
			}

			selBoundaryKeys.insert(tkey);
		}

	}
}

void Tpc::Processing::CloudHelper::splitIntoHorizontalSlices( PointCloud& cloud, PointCloudVector& outSlices, float sliceHeight )
{
	outSlices.clear();

	if (cloud.getPointCount() < 100)
	{
		throw Exception(L"Too few points in cloud.");
	}
	if (sliceHeight <0.05)
	{
		throw Exception(L"Too small slice height.");
	}

	recalculateCloudBox(cloud);
	const Box& box =cloud.getCloudData().boundingBox;

	float height =box.getHeight();
	if (10*height <sliceHeight)
	{
		throw Exception(L"Cloud is too flattened or of inappropriate scale.");
	}

	double sliceBegZ =box.getMinPoint().z;
	double sliceEndZ =(sliceBegZ +sliceHeight);
	int nCounter =0;
	do 
	{
		cloud.unselectAll();
		selectHorizontalSlice(cloud,sliceBegZ,sliceEndZ);

		outSlices.reserveOnePlace();
		PointCloud* pNewCloud =new PointCloud;
		if (!pNewCloud)
		{
			throw Exception(L"Can't allocate new cloud.");
		}
		outSlices.append(pNewCloud);

		copySelectedPointsTo(cloud,*pNewCloud);

		sliceBegZ =sliceEndZ;
		sliceEndZ +=sliceHeight;

		nCounter++;
	} 
	while (sliceBegZ <box.getMaxPoint().z);
}