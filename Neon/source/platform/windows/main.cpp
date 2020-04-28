#include "./core/engine.h"
#include "./gameplay/neon_game.h"

using namespace Neon;
int main(char* argc, char** argv)
{
	// Create the engine
	Core::Engine::Create();

	// Create & run game
	Neon::NeonGame* game = new NeonGame("NeonGame", 0);
	Core::Engine::GetInstance()->RunGame(game);

	// Destroy the engine
	Core::Engine::Destroy();
	return 0;
}