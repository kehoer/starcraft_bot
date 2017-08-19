#include "Common.h"
#include "InvictusBotModule.h"


void InvictusBotModule::onStart() {
	BWAPI::Broodwar->printf("Game started!");
	
 }

 void InvictusBotModule::onFrame(){

 }

 void InvictusBotModule::onEnd(bool isWinner) {
	 std::cout << "Game Ended";
 }

 void InvictusBotModule::onUnitDestroy(BWAPI::Unit unit) {
 }

 void InvictusBotModule::onUnitMorph(BWAPI::Unit unit) {
 }

 void InvictusBotModule::onSendText(std::string text) {

 }

 void InvictusBotModule::onUnitCreate(BWAPI::Unit unit) {
 }

 void InvictusBotModule::onUnitComplete(BWAPI::Unit unit) {
	 cout << unit->getID + "complete!";
 }

 void InvictusBotModule::onUnitShow(BWAPI::Unit unit){
 }

 void InvictusBotModule::onUnitHide(BWAPI::Unit unit){
 }

 void InvictusBotModule::onUnitRenegade(BWAPI::Unit unit){
 }

