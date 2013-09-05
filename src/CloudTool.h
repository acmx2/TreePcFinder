#pragma once

namespace Tpc
{
	namespace Base
	{
		class PointCloud;
	}

	namespace Processing
	{
		using Tpc::Base::PointCloud;

		///Working cloud is passed in on init. The tool doesn't own it.
		class CloudTool
		{
		protected:
			PointCloud* m_pWorkingCloud;

		public:
			CloudTool(void);
			virtual ~CloudTool(void);

			virtual void init(PointCloud*);
			virtual bool isInitialized() const;

			const PointCloud& getWorkingCloud() const;
			PointCloud& getWorkingCloud();
		};
	}
}
