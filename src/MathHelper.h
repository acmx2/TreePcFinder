#pragma once


#include <vector>

using namespace std;

namespace Tpc
{
	namespace Base
	{
		class MathHelper
		{
		private:
			MathHelper(void); ///no ctor
			~MathHelper(void);  ///no dtor

		public:
			static vector<float> truncateFunction( const vector<float>&, float );
			static float calculatePlateauValue( const vector<float>&, float , float , float );
			static int calculateBestPositionForValue( const vector<float>&, float ); ///calculates best pos =inverse_func(val)
		};
	}
}
