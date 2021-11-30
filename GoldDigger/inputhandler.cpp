// COMP710 GP 2D Framework 2020

// This include:
#include "inputhandler.h"

// Local includes:
#include "game.h"
#include "logmanager.h"
#include "player.h"
#include "hud.h"
#include "Debugger.h"

// Library includes:
#include <cassert>

InputHandler::InputHandler()
: m_pGameController(0)
{

}

InputHandler::~InputHandler()
{
	if (m_pGameController)
	{
		SDL_JoystickClose(m_pGameController);
		m_pGameController = 0;
	}
}

bool 
InputHandler::Initialise()
{
	int numControllesr = SDL_NumJoysticks();

	m_pGameController = SDL_JoystickOpen(0);

	if (!m_pGameController)
	{
		LogManager::GetInstance().Log("No controller detected!");
	}

	return (true);
}

void 
InputHandler::ProcessInput(Game& game)
{
	SDL_Event event;

	while (SDL_PollEvent(&event) != 0)
	{
#ifdef _DEBUG
		if (event.window.windowID == Debugger::GetInstance().GetWindowID())
		{
			Debugger::GetInstance().ProcessInput(event);
		}
		else
#endif // _DEBUG
		{
			if (event.type == SDL_QUIT)
			{
				game.Quit();
			}
			else if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					game.Quit();
				}
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
			{
				int mouseX = 0;
				int mouseY = 0;
				SDL_GetMouseState(&mouseX, &mouseY);

				HudManager::GetInstance().ButtonClick(mouseX, mouseY);
			}
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);
	bool moving = false;

	// Detect joystick axis and buttons pressed, not using SDL events because we need to get the value continuously, not only when axis changes value
	// We use 7000 offset to avoid joystick jitter.

	if (state[SDL_SCANCODE_DOWN] || SDL_JoystickGetButton(m_pGameController, 1))
	{
		moving = true;
		game.PlayerDig(Player::DigDirections::DOWN);
	}
	else if (state[SDL_SCANCODE_LEFT] || SDL_JoystickGetButton(m_pGameController, 3))
	{
		moving = true;
		game.PlayerDig(Player::DigDirections::LEFT);
	}
	else if (state[SDL_SCANCODE_RIGHT] || SDL_JoystickGetButton(m_pGameController, 2))
	{
		moving = true;
		game.PlayerDig(Player::DigDirections::RIGHT);
	}
	else
	{
		if (state[SDL_SCANCODE_S] || SDL_JoystickGetHat(m_pGameController, 0) == SDL_HAT_DOWN || SDL_JoystickGetAxis(m_pGameController, 1) > 7000)	// S key, D-pad down, joystick down
		{
			moving = true;
			game.MovePlayerDown();
		}

		if (state[SDL_SCANCODE_A] || SDL_JoystickGetHat(m_pGameController, 0) == SDL_HAT_LEFT || SDL_JoystickGetAxis(m_pGameController, 0) < -7000)	// A key, D-pad left, joystick left
		{
			moving = true;
			game.MovePlayerLeft();
		}

		if (state[SDL_SCANCODE_D] || SDL_JoystickGetHat(m_pGameController, 0) == SDL_HAT_RIGHT || SDL_JoystickGetAxis(m_pGameController, 0) > 7000)	// D key, D-pad right, joystick right
		{
			moving = true;
			game.MovePlayerRight();
		}
	}

	if (!moving)
	{
		game.GetPlayer()->Stand();
	}
}
