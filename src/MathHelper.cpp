#include "StdAfx.h"
#include "MathHelper.h"
#include "Exception.h"
#include <vector>
#include <algorithm>
#include <math.h>


using namespace Tpc::Base;
using namespace std;


namespace
{
	struct __tF__WeghtIdx
	{
		int idx;
		int weight;
		float val;
		__tF__WeghtIdx(int i, int w, float v) 
			: idx(i)
			, weight(w)
			, val(v) 
		{
		}
		bool operator <(const __tF__WeghtIdx& x) const 
		{
			return weight<x.weight;
		}
	};
}	

///implements FOREL approach
///func is non-negative
float Tpc::Base::MathHelper::calculatePlateauValue( const vector<float>& func, float cutoff, float intervalSizePercentage, float weightNotLessFromMaxPeakPercentage )
{
	if (func.size() <3)
	{
		return -1;
	}

	vector<float> workingFunc =func;

	///make the neighboring equal values more weighty
	vector<float> additionalValues;
	for(int i =0; i< (workingFunc.size() -1); i++)
	{
		if (workingFunc[i] >=cutoff || workingFunc[i+1] >=cutoff)  ///don't add weights for cutoff values
		{
			continue;
		}

		float intervalSize =workingFunc[i]*intervalSizePercentage;
		float intervalSize2 =intervalSize*0.65;
		float intervalSize3 =intervalSize*0.33;
		float deltaWithNext =(workingFunc[i+1] -workingFunc[i]);
		if (fabsf(deltaWithNext)< intervalSize3)
		{
			additionalValues.push_back(workingFunc[i] +deltaWithNext*0.25);
			additionalValues.push_back(workingFunc[i] +deltaWithNext*0.5);
			additionalValues.push_back(workingFunc[i] +deltaWithNext*0.75);
			continue;
		}
		if (fabsf(deltaWithNext)< intervalSize2)
		{
			additionalValues.push_back(workingFunc[i] +deltaWithNext*0.33);
			additionalValues.push_back(workingFunc[i] +deltaWithNext*0.67);
			continue;
		}
		if (fabsf(deltaWithNext)< intervalSize)
		{
			additionalValues.push_back(workingFunc[i] +deltaWithNext*0.5);
			continue;
		}
	}
	workingFunc.insert(workingFunc.end(),additionalValues.begin(),additionalValues.end());

	///cut off all values above cutoff
	{
		vector<float> refinedFunc2;
		for(int i =0; i< workingFunc.size(); i++)
		{
			if (workingFunc[i] >=cutoff)
			{
				continue;
			}
			refinedFunc2.push_back(workingFunc[i]);
		}
		workingFunc =refinedFunc2;
	}

	if (workingFunc.size() ==0)
	{
		return -1;
	}

	///run FOREL: calculate interval entry numbers assuming each value as lower boundary of interval and take max
	float bestVal =0.0;
	{
		vector<__tF__WeghtIdx> weights;
		for(int i =0; i< workingFunc.size(); i++)
		{
			float lowerBoundary =workingFunc[i];
			float upperBoundary =lowerBoundary +lowerBoundary*intervalSizePercentage;

			int w =0;
			double vSum =0.0;
			for(int j =0; j< workingFunc.size(); j++)
			{
				float v =workingFunc[j];
				if (v >=lowerBoundary && v <=upperBoundary)
				{
					w++;
					vSum +=v;
				}
			}
			float averageVal =(float)(vSum/(double)w);

			weights.push_back(__tF__WeghtIdx(i,w,averageVal));
		}
		std::sort(weights.begin(),weights.end());

		int maxWeight =weights.back().weight;
		int allowedWeight =(int)(weightNotLessFromMaxPeakPercentage*(float)maxWeight) +1;
		bestVal =weights.back().val;
		for(vector<__tF__WeghtIdx>::const_reverse_iterator ri =weights.rbegin(); ri !=weights.rend() && ri->weight>=allowedWeight; ri++)
		{
			if (ri->val <bestVal)
			{
				bestVal =ri->val;
			}
		}
	}

	return bestVal;
}

vector<float> Tpc::Base::MathHelper::truncateFunction( const vector<float>& func, float cutoff )
{
	if (func.size() <3)
	{
		return func;
	}

	vector<float> refinedFunc;
	int nCutoffCount =0;
	for(int i =0; i< func.size(); i++) ///truncate function when a lot of cutoffs is met
	{
		if (func[i] >=cutoff)
		{
			nCutoffCount++;
			if (i >0 && func[i-1] >=cutoff) ///make subsequent cutoffs weighty
			{
				nCutoffCount++;
			}
			if (nCutoffCount >=10)
			{
				break;
			}
		}
		refinedFunc.push_back(func[i]);
		if (nCutoffCount >0)
		{
			nCutoffCount--; ///to wait until next cutoffs peak
		}
	}

	return refinedFunc;
}

int Tpc::Base::MathHelper::calculateBestPositionForValue( const vector<float>& func, float fitVal )
{
	if (func.size() ==0)
	{
		return -1;
	}

	int bestI =0;
	float bestDelta =fabs(func[bestI] -fitVal);
	for(int i =1; i <func.size(); i++)
	{
		const float d =fabs(func[i] -fitVal);
		if (d <bestDelta)
		{
			bestDelta =d;
			bestI =i;
		}
	}
	return bestI;
}
