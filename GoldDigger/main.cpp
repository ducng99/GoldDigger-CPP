// COMP710 GP 2D Framework 2020

#include <ctime>

// Library includes:
#include <SDL.h>

// Local includes:
#include "game.h"
#include "logmanager.h"
#include "sound.h"
#include "hud.h"

#ifdef _DEBUG	
#include "debugger.h"
#endif // _DEBUG

#include <cstdlib>
#include <crtdbg.h>

int main(int argc, char* argv[])
{
	srand(static_cast<unsigned int>(time(0)));
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	SoundManager::GetInstance().Initialise();

	Game& gameInstance = Game::GetInstance();
	if (!gameInstance.Initialise())
	{
		LogManager::GetInstance().Log("Game initialise failed!");

		return (1);
	}

	if (argc > 1 && strcmp(argv[1], "--nosplash") == 0)
	{
		Game::GetInstance().SetGameState(GameStates::MAIN_MENU);
	}

#ifdef _DEBUG
	Debugger::GetInstance().Initialize(&gameInstance);
#endif // _DEBUG

	while (gameInstance.DoGameLoop())
	{
#ifdef _DEBUG
		Debugger::GetInstance().Draw();
#endif // _DEBUG
	}

#ifdef _DEBUG
	Debugger::DestroyInstance();
#endif
	HudManager::DestroyInstance();
	Game::DestroyInstance();
	SoundManager::DestroyInstance();
	LogManager::DestroyInstance();

	return (0);
}
