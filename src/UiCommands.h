#pragma once


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
