#pragma once
#include "Common.h"

//Useful for calculating how/where to build buildings
//class MinersMoveData { 
//public:
//
//	int mineralsNeeded;
//	int gasNeeded;
//	BWAPI::Position position;
//
//	MinersMoveData(int m, int g, BWAPI::Position p)
//		: mineralsNeeded(m)
//		, gasNeeded(g)
//		, position(p)
//	{
//
//	}
//
//	MinersMoveData() {}
//};

class MiningModule
{

public:

	enum WorkerJob { Minerals, Gas, Move, Idle, New};

private:
	const float OPTIMAL_PATCH_SATURATION = 2.5;
	const int GOOD_PATCH_SATURATION = 2;
	const int LOW_PATCH_SATURATION = 1;
	const int MAX_GAS_MINERS = 3;
	const int MINERAL_RADIUS_SIZE = 400;

	BWAPI::Unitset workers;
	BWAPI::Unitset depots;

	// worker -> jobs
	std::map<BWAPI::Unit, enum WorkerJob>  workerJobMap;
	// worker -> command center?
	std::map<BWAPI::Unit, BWAPI::Unit>     workerDepotMap;
	// worker -> gas
	std::map<BWAPI::Unit, BWAPI::Unit>     workerRefineryMap;
	// worker -> position
	//std::map<BWAPI::Unit, MinersMoveData>  workerMoveMap;

	//how many workers are at each CC
	std::map<BWAPI::Unit, int> depotWorkerCount;
	// how many workers are mining?
	std::map<BWAPI::Unit, int> refineryWorkerCount;
	// how many workers are on each mineral patch that we know of?
	std::map<BWAPI::Unit, int>  workersOnMineralPatch;
	// worker -> mineral patch
	std::map<BWAPI::Unit, BWAPI::Unit> workerMineralAssignment;

	//Depots to other maps

	void clearPreviousJob(BWAPI::Unit unit);

public:

	MiningModule();

	void                    workerSplit(BWAPI::Unitset units);
	void					workerDestroyed(BWAPI::Unit unit);
	void					addDepot(BWAPI::Unit unit);
	void					removeDepot(BWAPI::Unit unit);
	// overloaded function for assigning workers to different maps
	void					addWorker(BWAPI::Unit unit);
	void					addWorker(BWAPI::Unit unit, WorkerJob job, BWAPI::Unit jobUnit);
	void					addWorker(BWAPI::Unit unit, WorkerJob job, BWAPI::UnitType jobUnitType);
	// overloaded function for assigning worker jobs
	void					setWorkerJob(BWAPI::Unit unit, WorkerJob job, BWAPI::Unit jobUnit);
	//void					setWorkerJob(BWAPI::Unit unit, WorkerJob job, WorkerMoveData wmd);
	void					setWorkerJob(BWAPI::Unit unit, WorkerJob job, BWAPI::UnitType jobUnitType);

	// why are these const?
	int						getNumWorkers() const;
	int						getNumMineralWorkers() const;
	int						getNumGasWorkers() const;
	int						getNumIdleWorkers() const;
	// return currently assigned worker job
	char					getJobCode(BWAPI::Unit unit);

	void					getMineralWorkers(std::set<BWAPI::Unit> & mw);
	void					getGasWorkers(std::set<BWAPI::Unit> & mw);
	void					getBuildingWorkers(std::set<BWAPI::Unit> & mw);
	void					getRepairWorkers(std::set<BWAPI::Unit> & mw);

	// keep track of how many workers a depot "should" have
	bool					depotIsFull(BWAPI::Unit depot);
	int						getNumMineralsNearDepot(BWAPI::Unit depot);
	int                     getNumMineralsNearDepot(set<pair<int, BWAPI::Unit>> mineralSet);

	int						getNumAssignedWorkers(BWAPI::Unit unit);

	enum WorkerJob			getWorkerJob(BWAPI::Unit unit);
	BWAPI::Unit   getWorkerResource(BWAPI::Unit unit);
	BWAPI::Unit   getWorkerDepot(BWAPI::Unit unit);
	BWAPI::UnitType			getWorkerBuildingType(BWAPI::Unit unit);
	//WorkerMoveData			getWorkerMoveData(BWAPI::Unit unit);

	set<pair<int, BWAPI::Unit>> createMineralSet(BWAPI::Unit depot);
	void                        addToMineralPatch(BWAPI::Unit unit, int num);
	void					    drawDepotDebugInfo();
	void                        buildMoreWorkers();
	bool                        mineralSetSort(pair<int, BWAPI::Unit> mineralDistanceOne, pair<int, BWAPI::Unit> mineralDistanceTwo);

	const BWAPI::Unitset & getWorkers() const { 
		return workers; 
	}

};