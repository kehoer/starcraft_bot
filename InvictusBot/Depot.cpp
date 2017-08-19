#include "Depot.h"

Depot::Depot(int zoneOfControlRadius) {
	this->zoneOfControlRadius = zoneOfControlRadius;
}

Resource::Resource(BWAPI::Unit resource, int distanceToDepot) {
	this->resource = resource;
	this->distanceToDepot = distanceToDepot;
}

BWAPI::Unit Depot::getWorker() {
	BWAPI::Unit closestWorker;
	// look for idle worker first
	closestWorker = this->depot->getClosestUnit(BWAPI::Filter::IsWorker && BWAPI::Filter::IsOwned && BWAPI::Filter::IsIdle);
	
	//if there is no idle worker get someone mining
	if (closestWorker == nullptr) {
		closestWorker = this->depot->getClosestUnit(BWAPI::Filter::IsWorker && BWAPI::Filter::IsOwned);
	}

	return closestWorker;
}

void Depot::removeMineral(Resource* mineral) {
	this->minerals.erase(mineral);
}

void Depot::buildWorker() {
	this->depot->train(this->depot->getType().getRace().getWorker());
}

void Depot::createMineralSet() {
	for (BWAPI::Unit unit : this->depot->getUnitsInRadius(this->zoneOfControlRadius, BWAPI::Filter::IsMineralField)) {
		this->minerals.insert(new Resource(unit, unit->getDistance(this->depot)));
	}
}