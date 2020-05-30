#pragma once
#include "./core/game.h"
#include "./utilities/version_ID.h"

#include <string>


namespace Neon
{
	namespace Graphics { class GraphicsContext; }
	namespace Core
	{
		class Window;
		class Engine final
		{
		public:
			static void Create();
			static void Destroy();

			static Engine* GetInstance();

			int  RunGame(Game* _game);
			void RequestQuit();

		private:
			Engine();
			Engine(const std::string& _engineName, const VersionID _versionID);

			bool Initialize();
			bool Terminate();

			std::string m_EngineName;
			VersionID   m_EngineVersion;

			Window*     m_Window;

			Game*       m_Game;
			bool        m_Running;
		};
	}
}