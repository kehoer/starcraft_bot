#include "MiningModule.h"
#include "Micro.h"

using namespace std;

// make sure to use BWAPI::self()->foo when doing shit with units

void MiningModule::workerDestroyed(BWAPI::Unit unit) {
	if (!unit) { return; }

	clearPreviousJob(unit);
	workers.erase(unit);
}

void MiningModule::addWorker(BWAPI::Unit unit) {
	if (!unit) { return; }

	workers.insert(unit);
	workerJobMap[unit] = New;
}

void MiningModule::addWorker(BWAPI::Unit unit, WorkerJob job, BWAPI::Unit jobUnit) {
	if (!unit || !jobUnit) { return; }

	// make sure the unit isn't already in the workers map
	assert(workers.find(unit) == workers.end());

	workers.insert(unit);
	setWorkerJob(unit, job, jobUnit);
}

void MiningModule::addWorker(BWAPI::Unit unit, enum WorkerJob job, BWAPI::UnitType jobUnitType) {
	if (!unit) { return; }

	assert(workers.find(unit) == workers.end());
	workers.insert(unit);
	setWorkerJob(unit, job, jobUnitType);
}

void MiningModule::addDepot(BWAPI::Unit unit) {
	if (!unit) { return; }

	assert(depots.find(unit) == depots.end());
	depots.insert(unit);
	depotWorkerCount[unit] = 0;
}

void MiningModule::removeDepot(BWAPI::Unit unit) {
	if (!unit) { return; }

	depots.erase(unit);
	depotWorkerCount.erase(unit);

	// re-balance workers in here
	for (auto & worker : workers)
	{
		// if a worker was working at this depot
		if (workerDepotMap[worker] == unit)
		{
			setWorkerJob(worker, Idle, nullptr);
		}
	}
}

void MiningModule::addToMineralPatch(BWAPI::Unit unit, int num) {
	if (workersOnMineralPatch.find(unit) == workersOnMineralPatch.end())
	{
		workersOnMineralPatch[unit] = num;
	}
	else
	{
		workersOnMineralPatch[unit] = workersOnMineralPatch[unit] + num;
	}
}

//jobUnit can be building, depot, nullptr, unit (refinery(
void MiningModule::setWorkerJob(BWAPI::Unit unit, enum WorkerJob job, BWAPI::Unit jobUnit) {
	if (!unit) { return; }

	clearPreviousJob(unit);
	workerJobMap[unit] = job;

	if (job == Minerals)
	{
		// increase the number of workers assigned to this nexus
		depotWorkerCount[jobUnit] += 1;

		// set the mineral the worker is working on
		workerDepotMap[unit] = jobUnit;

		// right click the mineral to start mining
	}
	else if (job == Gas)
	{
		// increase the count of workers assigned to this refinery
		refineryWorkerCount[jobUnit] += 1;

		// set the refinery the worker is working on
		workerRefineryMap[unit] = jobUnit;

		// right click the refinery to start harvesting
		Micro::SmartRightClick(unit, jobUnit);
	}
}

void MiningModule::setWorkerJob(BWAPI::Unit unit, enum WorkerJob job, BWAPI::UnitType jobUnitType)
{
	if (!unit) { return; }

	clearPreviousJob(unit);
	workerJobMap[unit] = job;
}

//void MiningModule::setWorkerJob(BWAPI::Unit unit, enum WorkerJob job, WorkerMoveData wmd)
//{
//	if (!unit) { return; }
//
//	clearPreviousJob(unit);
//	workerJobMap[unit] = job;
//
//	if (job == Move)
//	{
//		workerMoveMap[unit] = wmd;
//	}
//
//	if (workerJobMap[unit] != Move)
//	{
//		BWAPI::Broodwar->printf("Something went horribly wrong");
//	}
//}


void MiningModule::clearPreviousJob(BWAPI::Unit unit)
{
	if (!unit) { return; }

	WorkerJob previousJob = getWorkerJob(unit);

	if (previousJob == Minerals)
	{
		depotWorkerCount[workerDepotMap[unit]] -= 1;

		workerDepotMap.erase(unit);

		// remove a worker from this unit's assigned mineral patch
		addToMineralPatch(workerMineralAssignment[unit], -1);

		// erase the association from the map
		workerMineralAssignment.erase(unit);
	}
	else if (previousJob == Gas)
	{
		refineryWorkerCount[workerRefineryMap[unit]] -= 1;
		workerRefineryMap.erase(unit);
	}
	workerJobMap.erase(unit);
}

int MiningModule::getNumWorkers() const
{
	return workers.size();
}

int MiningModule::getNumMineralWorkers() const
{
	size_t num = 0;
	for (auto & unit : workers)
	{
		if (workerJobMap.at(unit) == MiningModule::Minerals)
		{
			num++;
		}
	}
	return num;
}

int MiningModule::getNumGasWorkers() const
{
	size_t num = 0;
	for (auto & unit : workers)
	{
		if (workerJobMap.at(unit) == MiningModule::Gas)
		{
			num++;
		}
	}
	return num;
}

int MiningModule::getNumIdleWorkers() const
{
	size_t num = 0;
	for (auto & unit : workers)
	{
		if (workerJobMap.at(unit) == MiningModule::Idle)
		{
			num++;
		}
	}
	return num;
}


enum MiningModule::WorkerJob MiningModule::getWorkerJob(BWAPI::Unit unit)
{
	if (!unit) { return New; }

	std::map<BWAPI::Unit, enum WorkerJob>::iterator it = workerJobMap.find(unit);

	if (it != workerJobMap.end())
	{
		return it->second;
	}

	return New;
}

bool MiningModule::depotIsFull(BWAPI::Unit depot)
{
	if (!depot) { return false; }

	int assignedWorkers = getNumAssignedWorkers(depot);
	int mineralsNearDepot = getNumMineralsNearDepot(depot);

	if (assignedWorkers >= mineralsNearDepot * 3)
	{
		return true;
	}
	else
	{
		return false;
	}
}

set<pair<int, BWAPI::Unit>> MiningModule::createMineralSet(BWAPI::Unit depot) {
	set<pair<int, BWAPI::Unit>> mineralsNearDepot;
	int radius = 400;

	for (auto & unit : BWAPI::Broodwar->getAllUnits())
	{
		if ((unit->getType() == BWAPI::UnitTypes::Resource_Mineral_Field) && unit->getDistance(depot) < radius)
		{
			mineralsNearDepot.insert(pair<int, BWAPI::Unit>(unit->getDistance(depot), unit));
		}
	}

	// if we didn't find any, use the whole map
	if (mineralsNearDepot.empty())
	{
		for (auto & unit : BWAPI::Broodwar->getAllUnits())
		{
			if ((unit->getType() == BWAPI::UnitTypes::Resource_Mineral_Field))
			{
				mineralsNearDepot.insert(pair<int, BWAPI::Unit>(unit->getDistance(depot), unit));
			}
		}
	}

	return mineralsNearDepot;
}

int MiningModule::getNumMineralsNearDepot(BWAPI::Unit depot)
{
	if (!depot) { return 0; }

	int mineralsNearDepot = 0;

	for (auto & unit : BWAPI::Broodwar->getAllUnits())
	{
		if ((unit->getType() == BWAPI::UnitTypes::Resource_Mineral_Field) && unit->getDistance(depot) < MINERAL_RADIUS_SIZE)
		{
			mineralsNearDepot++;
		}
	}

	return mineralsNearDepot;
}

const int getNumMineralsNearDepot(const set<pair<int, BWAPI::Unit>> mineralSet) {
	return mineralSet.size();
}

BWAPI::Unit MiningModule::getWorkerResource(BWAPI::Unit unit) {
	if (!unit) { return nullptr; }

	// create the iterator
	std::map<BWAPI::Unit, BWAPI::Unit>::iterator it;

	// if the worker is mining, set the iterator to the mineral map
	if (getWorkerJob(unit) == Minerals)
	{
		it = workerMineralMap.find(unit);
		if (it != workerMineralMap.end())
		{
			return it->second;
		}
	}
	else if (getWorkerJob(unit) == Gas)
	{
		it = workerRefineryMap.find(unit);
		if (it != workerRefineryMap.end())
		{
			return it->second;
		}
	}

	return nullptr;
}

BWAPI::Unit MiningModule::getWorkerDepot(BWAPI::Unit unit)
{
	if (!unit) { return nullptr; }

	std::map<BWAPI::Unit, BWAPI::Unit>::iterator it = workerDepotMap.find(unit);

	if (it != workerDepotMap.end())
	{
		return it->second;
	}

	return nullptr;
}

//BWAPI::UnitType	MiningModule::getWorkerBuildingType(BWAPI::Unit unit)
//{
//	if (!unit) { return BWAPI::UnitTypes::None; }
//
//	std::map<BWAPI::Unit, BWAPI::UnitType>::iterator it = workerBuildingTypeMap.find(unit);
//
//	if (it != workerBuildingTypeMap.end())
//	{
//		return it->second;
//	}
//
//	return BWAPI::UnitTypes::None;
//}

//WorkerMoveData MiningModule::getWorkerMoveData(BWAPI::Unit unit)
//{
//	std::map<BWAPI::Unit, WorkerMoveData>::iterator it = workerMoveMap.find(unit);
//
//	assert(it != workerMoveMap.end());
//
//	return (it->second);
//}

int MiningModule::getNumAssignedWorkers(BWAPI::Unit unit)
{
	if (!unit) { return 0; }

	std::map<BWAPI::Unit, int>::iterator it;

	// if the worker is mining, set the iterator to the mineral map
	if (unit->getType().isResourceDepot())
	{
		it = depotWorkerCount.find(unit);

		// if there is an entry, return it
		if (it != depotWorkerCount.end())
		{
			return it->second;
		}
	}
	else if (unit->getType().isRefinery())
	{
		it = refineryWorkerCount.find(unit);

		// if there is an entry, return it
		if (it != refineryWorkerCount.end())
		{
			return it->second;
		}
		// otherwise, we are only calling this on completed refineries, so set it
		else
		{
			refineryWorkerCount[unit] = 0;
		}
	}

	// when all else fails, return 0
	return 0;
}

char MiningModule::getJobCode(BWAPI::Unit unit)
{
	if (!unit) { return 'X'; }

	MiningModule::WorkerJob j = getWorkerJob(unit);

	if (j == MiningModule::Gas) return 'G';
	if (j == MiningModule::Idle) return 'I';
	if (j == MiningModule::Minerals) return 'M';
	if (j == MiningModule::Move) return 'O';
	if (j == MiningModule::New) return 'N';
	return 'X';
}

void MiningModule::drawDepotDebugInfo()
{
	for (auto & depot : depots)
	{
		int x = depot->getPosition().x - 64;
		int y = depot->getPosition().y - 32;

		set<pair<int, BWAPI::Unit>> minerals = createMineralSet(depot);

		for (auto & mineral : minerals)
		{
			int x = mineral.second->getPosition().x;
			int y = mineral.second->getPosition().y;
			break;

		//	if (workersOnMineralPatch.find(mineral) != workersOnMineralPatch.end())
		//	{
				//if (Config::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawBoxMap(x-2, y-1, x+75, y+14, BWAPI::Colors::Black, true);
				//if (Config::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(x, y, "\x04 Workers: %d", workersOnMineralPatch[mineral]);
		//	}
		}
	}
}

bool MiningModule::mineralSetSort(pair<int, BWAPI::Unit> mineralDistanceOne, pair<int, BWAPI::Unit> mineralDistanceTwo) {
	return mineralDistanceOne.first > mineralDistanceTwo.first;
}

