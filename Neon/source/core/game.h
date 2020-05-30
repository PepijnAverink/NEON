#pragma once
#include "./utilities/version_ID.h"

#include <string>

namespace Neon
{
	namespace Core
	{
		class Window;
		class Game
		{
		public:
			Game() = default;
			Game(const std::string& _gameName, const VersionID _versionID)
				: m_GameName(_gameName)
				, m_GameVersion(_versionID)
			{}

			virtual ~Game() {}

			virtual bool Initialize(Window* _window) = 0;
			virtual bool Terminate()  = 0;

			virtual void Update(const float _dt) = 0;
			virtual void Render() = 0;

		private:
			std::string m_GameName;
			VersionID   m_GameVersion;
		};
	}
}