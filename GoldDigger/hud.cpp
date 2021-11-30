#include "hud.h"
#include "backbuffer.h"
#include "sprite.h"
#include "game.h"
#include "HudButton.h"
#include "ButtonTypes.h"

#include <string>
#include <cassert>

std::map<ButtonTypes, GameStates> HudManager::m_buttonAction;
Sprite* HudManager::m_pBackground = 0;
Sprite* HudManager::m_pResultPanel = 0;
Sprite* HudManager::m_pInstructionSprite = 0;
HudManager* HudManager::sm_pInstance = 0;

HudManager::HudManager()
: m_bShowInstructions(false)
{

}

HudManager::~HudManager()
{
	m_pBackBuffer = 0;

	delete m_pBackground;
	m_pBackground = 0;
	delete m_pResultPanel;
	m_pResultPanel = 0;
	delete m_pInstructionSprite;
	m_pInstructionSprite = 0;

	std::map<Buttons, HudButton*>::iterator it = m_pButtons.begin();

	while (it != m_pButtons.end())
	{
		delete it->second;
		it->second = 0;

		it++;
	}

	m_pButtons.clear();
}

HudManager & HudManager::GetInstance()
{
	if (sm_pInstance == 0)
	{
		sm_pInstance = new HudManager();
	}

	assert(sm_pInstance);
	return *sm_pInstance;
}

void HudManager::DestroyInstance()
{
	delete sm_pInstance;
	sm_pInstance = 0;
}

void HudManager::InitialiseSprites(BackBuffer* backBuffer)
{
	m_pBackground = backBuffer->CreateSprite("assets\\imgs\\hud\\background_mainMenu.png");
	m_pResultPanel = backBuffer->CreateSprite("assets\\imgs\\hud\\resultPanel.png");
	m_pInstructionSprite = backBuffer->CreateSprite("assets\\imgs\\hud\\instructions.png");

	m_buttonAction[ButtonTypes::PLAY] = GameStates::IN_GAME;
	m_buttonAction[ButtonTypes::RETURN] = GameStates::RESULT_SCREEN;
	m_buttonAction[ButtonTypes::CLOSE] = GameStates::END_GAME;
}

void HudManager::Initialise(BackBuffer * backBuffer)
{
	m_pBackBuffer = backBuffer;

	m_pButtons[Buttons::MainMenuPlayButton] = new HudButton();
	m_pButtons[Buttons::MainMenuPlayButton]->Initialise(backBuffer->CreateSprite("assets\\imgs\\hud\\playButton.png"), ButtonTypes::PLAY);
	m_pButtons[Buttons::MainMenuPlayButton]->SetPosition(static_cast<int>(Game::GetInstance().GetWindowWidth() / 2.f - m_pButtons[Buttons::MainMenuPlayButton]->GetWidth() / 2.f), static_cast<int>(Game::GetInstance().GetWindowHeight() / 2.f - m_pButtons[Buttons::MainMenuPlayButton]->GetHeight() / 2.f));
	
	m_pButtons[Buttons::MainMenuCloseButton] = new HudButton();
	m_pButtons[Buttons::MainMenuCloseButton]->Initialise(backBuffer->CreateSprite("assets\\imgs\\hud\\closeButton.png"), ButtonTypes::CLOSE);
	m_pButtons[Buttons::MainMenuCloseButton]->SetPosition(0, 0);

	m_pButtons[Buttons::IngameEndButton] = new HudButton();
	m_pButtons[Buttons::IngameEndButton]->Initialise(backBuffer->CreateSprite("assets\\imgs\\hud\\closeButton.png"), ButtonTypes::RETURN);
	m_pButtons[Buttons::IngameEndButton]->SetPosition(0, 0);

	m_pButtons[Buttons::ResultPlayButton] = new HudButton();
	m_pButtons[Buttons::ResultPlayButton]->Initialise(backBuffer->CreateSprite("assets\\imgs\\hud\\playButton.png"), ButtonTypes::PLAY);
	m_pButtons[Buttons::ResultPlayButton]->SetPosition(static_cast<int>(Game::GetInstance().GetWindowWidth() / 2.f - m_pButtons[Buttons::ResultPlayButton]->GetWidth() / 2.f), static_cast<int>(Game::GetInstance().GetWindowHeight() / 2.f + 200));

	m_pButtons[Buttons::ResultCloseButton] = new HudButton();
	m_pButtons[Buttons::ResultCloseButton]->Initialise(backBuffer->CreateSprite("assets\\imgs\\hud\\closeButton.png"), ButtonTypes::CLOSE);
	m_pButtons[Buttons::ResultCloseButton]->SetPosition(0, 0);

	m_pButtons[Buttons::HelpButton] = new HudButton();
	m_pButtons[Buttons::HelpButton]->Initialise(backBuffer->CreateSprite("assets\\imgs\\hud\\helpButton.png"), ButtonTypes::HELP);
	m_pButtons[Buttons::HelpButton]->SetPosition(Game::GetInstance().GetWindowWidth() - m_pButtons[Buttons::HelpButton]->GetWidth(), 0);
}

void HudManager::DrawMainMenu()
{
	m_pBackBuffer->DrawSprite(*m_pBackground);

	m_pButtons[Buttons::MainMenuPlayButton]->Draw(m_pBackBuffer);
	m_pButtons[Buttons::MainMenuCloseButton]->Draw(m_pBackBuffer);
	m_pButtons[Buttons::HelpButton]->Draw(m_pBackBuffer);
}

void HudManager::DrawIngame()
{
	m_pBackBuffer->DrawSprite(*m_pBackground);

	m_pButtons[Buttons::IngameEndButton]->Draw(m_pBackBuffer);
	m_pButtons[Buttons::HelpButton]->Draw(m_pBackBuffer);
}

void HudManager::DrawResult(Player* player)
{
	m_pBackBuffer->DrawSprite(*m_pBackground);

	m_pResultPanel->SetX(static_cast<int>(Game::GetInstance().GetWindowWidth() / 2.f - m_pResultPanel->GetWidth() / 2.f));
	m_pResultPanel->SetY(static_cast<int>(180));
	m_pBackBuffer->DrawSprite(*m_pResultPanel);

	m_pBackBuffer->SetTextColour(0xE, 0xE, 0xE);
	m_pBackBuffer->DrawText(static_cast<int>(Game::GetInstance().GetWindowWidth() / 2.f - 200), 250, ("Name: " + string(player->GetName())).c_str());
	m_pBackBuffer->DrawText(static_cast<int>(Game::GetInstance().GetWindowWidth() / 2.f - 200), 290, ("Score: " + to_string(player->GetScore())).c_str());
	m_pBackBuffer->DrawText(static_cast<int>(Game::GetInstance().GetWindowWidth() / 2.f - 200), 330, ("Life: " + to_string(player->GetLife())).c_str());

	m_pButtons[Buttons::ResultPlayButton]->Draw(m_pBackBuffer);
	m_pButtons[Buttons::ResultCloseButton]->Draw(m_pBackBuffer);
	m_pButtons[Buttons::HelpButton]->Draw(m_pBackBuffer);
}

void HudManager::DrawInstructions()
{
	if (m_bShowInstructions)
	{
		m_pBackBuffer->DrawSprite(*m_pInstructionSprite);
	}
}

void HudManager::ButtonClick(int x, int y)
{
	bool clicked = false;

	std::map<Buttons, HudButton*>::iterator it = m_pButtons.begin();

	while (it != m_pButtons.end() && !clicked)
	{
		bool buttonShowing = false;

		switch (Game::GetInstance().GetGameState())
		{
		case GameStates::MAIN_MENU:
			if (it->first == Buttons::MainMenuCloseButton || it->first == Buttons::MainMenuPlayButton)
			{
				buttonShowing = true;
			}
			break;
		case GameStates::IN_GAME:
			if (it->first == Buttons::IngameEndButton)
			{
				buttonShowing = true;
			}
			break;
		case GameStates::RESULT_SCREEN:
			if (it->first == Buttons::ResultPlayButton || it->first == Buttons::ResultCloseButton)
			{
				buttonShowing = true;
			}
		}

		int buttonX = it->second->GetX();
		int buttonY = it->second->GetY();

		if (buttonShowing)
		{

			if (x >= buttonX && x <= buttonX + it->second->GetWidth() && y >= buttonY && y <= buttonY + it->second->GetHeight())
			{
				if (m_buttonAction[it->second->GetType()] == GameStates::IN_GAME)
				{
					Game::GetInstance().SetupNewGame();
				}

				Game::GetInstance().SetGameState(m_buttonAction[it->second->GetType()]);

				clicked = true;
			}
		}

		if (it->first == Buttons::HelpButton && x >= buttonX && x <= buttonX + it->second->GetWidth() && y >= buttonY && y <= buttonY + it->second->GetHeight())
		{
			m_bShowInstructions = !m_bShowInstructions;
		}

		it++;
	}
}
