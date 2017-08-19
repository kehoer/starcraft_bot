#pragma once
#include "MiningModule.h"
#include "Common.h"

class WorkerManager {
	MiningModule miningModule;
	BWAPI::Unit previousClosestWorker;

	void updateWorkers();

	static WorkerManager &  Instance();
};