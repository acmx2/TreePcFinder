#pragma once

namespace Tpc
{
	namespace Base
	{
		class PointCloud;
	}

	namespace Processing
	{
		///Working cloud is provided on init. The tool doesn't own it.
		class CloudTool
		{
		protected:
			Tpc::Base::PointCloud* m_pWorkingCloud;

		public:
			CloudTool(void);
			virtual ~CloudTool(void);

			virtual void init(Tpc::Base::PointCloud*);
			virtual bool isInitialized() const;

			const Tpc::Base::PointCloud& getWorkingCloud() const;
			Tpc::Base::PointCloud& getWorkingCloud();
		};
	}
}
