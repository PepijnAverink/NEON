#include "./gameplay/neon_game.h"

namespace Neon
{
	NeonGame::NeonGame(const std::string& _gameName, const VersionID _versionID)
		: Game(_gameName, _versionID)
	{ }

	bool NeonGame::Initialize()
	{
		return true;
	}

	bool NeonGame::Terminate()
	{
		return true;
	}

	void NeonGame::Update(const float _dt)
	{

	}

	void NeonGame::Render()
	{

	}
}