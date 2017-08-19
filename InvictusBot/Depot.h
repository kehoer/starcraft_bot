#pragma once
#include "Common.h"

class Resource {
public:
	BWAPI::Unit resource;
	int distanceToDepot;
	std::vector<BWAPI::Unit> assignedWorkers;
	Resource(BWAPI::Unit resource, int distanceToDepot);
};

class Depot {
private:
	BWAPI::Unit depot;
	int zoneOfControlRadius;

	enum workerJobs { Mining, Idle, Attacking, Building, Scouting };

	std::set<Resource*> minerals;
	std::set<Resource*> gas;
	

	void onFrame();
	void onUnitComplete();
	void createMineralSet();
	
public:
	Depot(int zoneOfControlRadius);

	std::vector<BWAPI::Unit> buildingVector;
	std::map<BWAPI::Unit, enum workerJobs> workerJobs;

	BWAPI::Unit getWorker();
	void removeMineral(Resource* mineral);
	void buildWorker();
};