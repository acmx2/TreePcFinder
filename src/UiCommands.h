#pragma once

#include "Exception.h"
#include "Point3d.h"


using namespace Tpc::Sys;
using namespace Tpc::Geom;

namespace Tpc
{
	namespace Ui
	{
		class Commands
		{
		private:
			Commands(void);
			~Commands(void);

			static void printTreeProperties(float,float,float);

		public:
			static void understandTree(const char*);
		};
	}
}
