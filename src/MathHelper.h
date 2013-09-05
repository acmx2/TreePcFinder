#pragma once

#include <vector>


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
			static std::vector<float> truncateFunction( const std::vector<float>&, float );
			static float calculatePlateauValue( const std::vector<float>&, float , float , float );
			static int calculateBestPositionForValue( const std::vector<float>&, float ); ///calculates best pos =inverse_func(val)
		};
	}
}
