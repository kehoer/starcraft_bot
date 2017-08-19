#pragma once
#include "Common.h"

	class ProductionManager {
		static ProductionManager & Instance();
		void buildMoreWorkers(BWAPI::Unitset depots, std::map<BWAPI::Unit, int> workerMap, int workerCount);
	};
