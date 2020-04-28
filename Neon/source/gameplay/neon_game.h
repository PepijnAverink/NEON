#pragma once
#include "./core/game.h"

namespace Neon
{
	class NeonGame : public Core::Game
	{
	public:
		NeonGame() = default;
		NeonGame(const std::string& _gameName, const VersionID _versionID);

		virtual bool Initialize() override;
		virtual bool Terminate() override;

		virtual void Update(const float _dt) override;
		virtual void Render() override;
	};
}