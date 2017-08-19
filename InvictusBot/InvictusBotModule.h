#pragma once

#include "MiningModule.h"
#include <BWAPI.h>

using namespace std;

namespace InvictusBot
{

	class InvictusBotModule : public BWAPI::AIModule
	{

	public:

		void	onStart();
		void	onFrame();
		void	onEnd(bool isWinner);
		void	onUnitDestroy(BWAPI::Unit unit);
		void	onUnitMorph(BWAPI::Unit unit);
		void	onSendText(std::string text);
		void	onUnitCreate(BWAPI::Unit unit);
		void	onUnitComplete(BWAPI::Unit unit);
		void	onUnitShow(BWAPI::Unit unit);
		void	onUnitHide(BWAPI::Unit unit);
		void	onUnitRenegade(BWAPI::Unit unit);
	};

}