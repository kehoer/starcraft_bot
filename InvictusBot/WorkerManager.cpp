#include "WorkerManager.h"

WorkerManager::WorkerManager()
{
	previousClosestWorker = nullptr;
}

WorkerManager & WorkerManager::Instance()
{
	static WorkerManager instance;
	return instance;
}

void updateWorkers() {

}