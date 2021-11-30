// COMP710 GP 2D Framework 2020

// This includes:
#include "game.h"

// Local includes:
#include "backbuffer.h"
#include "inputhandler.h"
#include "logmanager.h"
#include "sprite.h"
#include "GameObject.h"
#include "Dirt.h"
#include "Chest.h"
#include "Diamond.h"
#include "Gold.h"
#include "Heart.h"
#include "Mine.h"
#include "BackgroundRock.h"
#include "map.h"
#include "player.h"
#include "sound.h"
#include "hud.h"

// Library includes:
#include <cassert>
#include <SDL.h>
#include <cstdio>

#define GET_SPLASH_ALPHA(curTime, maxTime) static_cast<unsigned char>((255 / powf(maxTime / 2, 2)) * powf(curTime - maxTime / 2, 2)) % 256

// Static Members:
Game* Game::sm_pInstance = 0;

Game&
Game::GetInstance()
{
	if (sm_pInstance == 0)
	{ 
		sm_pInstance = new Game();
	}

	assert(sm_pInstance);

	return (*sm_pInstance);
}

void 
Game::DestroyInstance()
{
	delete sm_pInstance; 
	sm_pInstance = 0;
}

Game::Game()
: m_pBackBuffer(0)
, m_pInputHandler(0)
, m_looping(true)
, m_executionTime(0)
, m_elapsedSeconds(0)
, m_frameCount(0)
, m_FPS(0)
, m_numUpdates(0)
, m_lastTime(0)
, m_lag(0)
, m_numBlocksWidth(8)
, m_numBlocksHeight(12)
, m_windowWidth(640)
, m_windowHeight(960)
, m_blockHeight(80)
, m_blockWidth(80)
, m_pPlayer(0)
, m_pMap(0)
, m_splashTimer(0)
, m_gameState(GameStates::GAME_START)
{
	
}

Game::~Game()
{
	delete m_pInputHandler;
	m_pInputHandler = 0;

	delete m_pBackBuffer;
	m_pBackBuffer = 0;

	delete m_pMap;
	m_pMap = 0;

	delete m_pPlayer;
	m_pPlayer = 0;

	delete m_pAUTLogoSprite;
	m_pAUTLogoSprite = 0;

	delete m_pMWLogosSprite;
	m_pMWLogosSprite = 0;

	BackgroundRock::DestroySprites();
	Chest::DestroySprites();
	Diamond::DestroySprites();
	Dirt::DestroySprites();
	Gold::DestroySprites();
	Heart::DestroySprites();
	Mine::DestroySprites();
	Player::DestroySprites();
}

bool 
Game::Initialise()
{
	m_pBackBuffer = new BackBuffer();
	if (!m_pBackBuffer->Initialise(m_windowWidth, m_windowHeight))
	{
		LogManager::GetInstance().Log("BackBuffer Init Fail!");
		return (false);
	}

	m_pInputHandler = new InputHandler();
	if (!m_pInputHandler->Initialise())
	{
		LogManager::GetInstance().Log("InputHandler Init Fail!");
		return (false);
	}

	m_lastTime = SDL_GetPerformanceCounter();
	m_lag = 0.0f;

	m_pBackBuffer->SetClearColour(0, 0, 0);
	m_pBackBuffer->SetTextColour(0, 0, 0);

	m_pAUTLogoSprite = m_pBackBuffer->CreateSprite("assets\\imgs\\AUT-logo.jpg");
	m_pAUTLogoSprite->SetX(static_cast<int>(m_windowWidth / 2.f - m_pAUTLogoSprite->GetWidth() / 2.f));
	m_pAUTLogoSprite->SetY(static_cast<int>(m_windowHeight / 2.f - m_pAUTLogoSprite->GetHeight() / 2.f));

	m_pMWLogosSprite = m_pBackBuffer->CreateSprite("assets\\imgs\\mw.png");

	HudManager::InitialiseSprites(m_pBackBuffer);
	HudManager::GetInstance().Initialise(m_pBackBuffer);

	BackgroundRock::InitialiseSprites(m_pBackBuffer);
	Chest::InitialiseSprites(m_pBackBuffer);
	Diamond::InitialiseSprites(m_pBackBuffer);
	Dirt::InitialiseSprites(m_pBackBuffer);
	Gold::InitialiseSprites(m_pBackBuffer);
	Heart::InitialiseSprites(m_pBackBuffer);
	Mine::InitialiseSprites(m_pBackBuffer);
	Player::InitialiseSprites(m_pBackBuffer);

	m_pMap = new Map(m_numBlocksWidth, m_numBlocksHeight);
	assert(m_pMap);

	m_pPlayer = new Player("Player 1");
	m_pPlayer->SetPosition(floorf(m_numBlocksWidth / 2.f) * m_blockWidth, 3.f * m_blockHeight);

	SetupNewGame();

	return (true);
}

bool 
Game::DoGameLoop()
{
	const float stepSize = 1.0f / 60.0f;

	assert(m_pInputHandler);
	
	if (m_looping)
	{
		Uint64 current = SDL_GetPerformanceCounter();

		float deltaTime = (current - m_lastTime) / static_cast<float>(SDL_GetPerformanceFrequency());
		
		m_lastTime = current;

		m_executionTime += deltaTime;

		Process(deltaTime);
	
		m_lag += deltaTime;

		int innerLag = 0;

		while (m_lag >= stepSize)
		{
			Process(stepSize);
			m_pInputHandler->ProcessInput(*this);

			m_lag -= stepSize;

			++m_numUpdates;
			++innerLag;
		}

		// DEBUG STUB:
//		char buffer[64];
//		sprintf(buffer, "%f", deltaTime);
//		LogManager::GetInstance().Log(buffer);

		Draw(*m_pBackBuffer);
	}

	return (m_looping);
}

void 
Game::Process(float deltaTime)
{
	// Count total simulation time elapsed:
	m_elapsedSeconds += deltaTime;

	// Frame Counter:
	if (m_elapsedSeconds > 1)
	{
		m_elapsedSeconds -= 1;
		m_FPS = m_frameCount;
		m_frameCount = 0;
	}

	switch (GetGameState())
	{
	case GameStates::GAME_START:
		m_gameState = GameStates::AUT_SPLASH_SCREEN;
		break;
	case GameStates::AUT_SPLASH_SCREEN:
		if (m_splashTimer < SPLASH_SHOW_TIME)
		{
			m_splashTimer += deltaTime;
		}
		else
		{
			m_splashTimer = 0;
			m_gameState = GameStates::MIDDLEWARE_SPLASH_SCREEN;
		}
		break;
	case GameStates::MIDDLEWARE_SPLASH_SCREEN:
		if (m_splashTimer < SPLASH_SHOW_TIME)
		{
			m_splashTimer += deltaTime;
		}
		else
		{
			m_splashTimer = 0;
			m_gameState = GameStates::MAIN_MENU;
		}
		break;
	case GameStates::MAIN_MENU:
		//m_gameState = GameStates::IN_GAME;
		break;
	case GameStates::IN_GAME:
		ProcessGameplay(deltaTime);
		break;
	case GameStates::RESULT_SCREEN:

		break;
	case GameStates::END_GAME:
		this->Quit();
		break;
	}
}

void 
Game::Draw(BackBuffer& backBuffer)
{
	++m_frameCount;

	backBuffer.Clear();

	switch (m_gameState)
	{
	case GameStates::AUT_SPLASH_SCREEN:
		backBuffer.DrawSprite(*m_pAUTLogoSprite);												// Draw AUT logo
		backBuffer.SetDrawColour(0, 0, 0, GET_SPLASH_ALPHA(m_splashTimer, SPLASH_SHOW_TIME));	// Set cover black colour with varying alpha value (parabola equation)
		backBuffer.DrawRectangle(0, 0, m_windowWidth, m_windowHeight);							// Draw cover
		HudManager::GetInstance().DrawInstructions();
		break;
	case GameStates::MIDDLEWARE_SPLASH_SCREEN:
		backBuffer.DrawSprite(*m_pMWLogosSprite);	// Draw middlewares' logos
		backBuffer.SetDrawColour(0, 0, 0, GET_SPLASH_ALPHA(m_splashTimer, SPLASH_SHOW_TIME));
		backBuffer.DrawRectangle(0, 0, m_windowWidth, m_windowHeight);
		HudManager::GetInstance().DrawInstructions();
		break;
	case GameStates::MAIN_MENU:
		HudManager::GetInstance().DrawMainMenu();
		HudManager::GetInstance().DrawInstructions();
		break;
	case GameStates::IN_GAME:
		HudManager::GetInstance().DrawIngame();

		// https://stackoverflow.com/a/19830820 - fix switch scope issue by adding scopes
		{
			std::vector<std::vector<BackgroundRock*>> backgroundBlocks = m_pMap->GetBackgroundBlocks();

			for each (auto bgRockObjects in backgroundBlocks)
			{
				for (size_t height = 3; height < bgRockObjects.size(); ++height)
				{
					bgRockObjects[height]->Draw(backBuffer);
				}
			}
		}

		{
			std::vector<std::vector<Dirt*>> dirtBlocks = m_pMap->GetDirtBlocks();

			for each (auto dirtObjects in dirtBlocks)
			{
				for (size_t height = 3; height < dirtObjects.size(); ++height)
				{
					if (!dirtObjects[height]->IsDiscovered())
						dirtObjects[height]->Draw(backBuffer);
				}
			}
		}

		for (int width = 0; width < m_numBlocksWidth; ++width)
		{
			for (int height = 0; height < m_numBlocksHeight; ++height)
			{
				auto block = m_pMap->GetBlock(width, height);
				if (block->GetType() != Types::UNKNOWN && !block->IsDiscovered())
				{
					block->Draw(backBuffer);
				}
			}
		}

		m_pPlayer->Draw(backBuffer);

		char hud_score[64];
		sprintf_s(hud_score, "Player: %s - Score: %d - Life: %d", m_pPlayer->GetName(), m_pPlayer->GetScore(), m_pPlayer->GetLife());

		HudManager::GetInstance().DrawInstructions();
		backBuffer.DrawText(120, 0, hud_score);
		break;
	case GameStates::RESULT_SCREEN:
		HudManager::GetInstance().DrawResult(m_pPlayer);
		HudManager::GetInstance().DrawInstructions();
		break;
	default:
		break;
	}
	
	backBuffer.Present();
}

void Game::ProcessGameplay(float deltaTime)
{
	auto dirtBlocks = m_pMap->GetDirtBlocks();

	for (size_t width = 0; width < dirtBlocks.size(); ++width)
	{
		for (size_t height = 3; height < dirtBlocks[width].size(); ++height)
		{
			switch (dirtBlocks[width][height]->GetDirtPosition())
			{
			case Dirt::DirtPosition::TOP_LEFT_DIGGED:
				if (width < dirtBlocks.size() - 1 && !dirtBlocks[width + 1][height]->IsDiscovered())
				{
					dirtBlocks[width + 1][height]->Initialise(Dirt::DirtPosition::TOP_LEFT);
				}
				break;
			case Dirt::DirtPosition::TOP_DIGGED:
				if (width > 0 && !dirtBlocks[width - 1][height]->IsDiscovered())
				{
					dirtBlocks[width - 1][height]->Initialise(Dirt::DirtPosition::TOP_RIGHT);
				}

				if (width < dirtBlocks.size() - 1 && !dirtBlocks[width + 1][height]->IsDiscovered())
				{
					dirtBlocks[width + 1][height]->Initialise(Dirt::DirtPosition::TOP_LEFT);
				}
				break;
			case Dirt::DirtPosition::TOP_RIGHT_DIGGED:
				if (width > 0 && !dirtBlocks[width - 1][height]->IsDiscovered())
				{
					dirtBlocks[width - 1][height]->Initialise(Dirt::DirtPosition::TOP_RIGHT);
				}
			case Dirt::DirtPosition::MIDDLE_LEFT_DIGGED:
				if (width < dirtBlocks.size() - 1 && !dirtBlocks[width + 1][height]->IsDiscovered())
				{
					dirtBlocks[width + 1][height]->Initialise(Dirt::DirtPosition::MIDDLE_LEFT);
				}
				break;
			case Dirt::DirtPosition::MIDDLE_DIGGED:
				if (width > 0 && !dirtBlocks[width - 1][height]->IsDiscovered())
				{
					dirtBlocks[width - 1][height]->Initialise(Dirt::DirtPosition::MIDDLE_RIGHT);
				}

				if (width < dirtBlocks.size() - 1 && !dirtBlocks[width + 1][height]->IsDiscovered())
				{
					dirtBlocks[width + 1][height]->Initialise(Dirt::DirtPosition::MIDDLE_LEFT);
				}
				break;
			case Dirt::DirtPosition::MIDDLE_RIGHT_DIGGED:
				if (width > 0 && !dirtBlocks[width - 1][height]->IsDiscovered())
				{
					dirtBlocks[width - 1][height]->Initialise(Dirt::DirtPosition::MIDDLE_RIGHT);
				}
				break;
			default:
				break;
			}
		}
	}

	for (int width = 0; width < m_numBlocksWidth; ++width)
	{
		for (int height = 0; height < m_numBlocksHeight; ++height)
		{
			auto block = m_pMap->GetBlock(width, height);
			if (m_pPlayer->IsCollidingWith(*block) && !block->IsDiscovered())
			{
				switch (block->GetType())
				{
				case Types::CHEST:
					m_pPlayer->SetScore(m_pPlayer->GetScore() + block->GetValue());
					SetGameState(GameStates::RESULT_SCREEN);
					static_cast<Chest*>(block)->OpenChest();
					SoundManager::GetInstance().PlaySound(SoundManager::GetInstance().GetSound("assets\\sounds\\pickup.wav"));
					break;
				case Types::DIAMOND:
					m_pPlayer->SetScore(m_pPlayer->GetScore() + block->GetValue());
					SoundManager::GetInstance().PlaySound(SoundManager::GetInstance().GetSound("assets\\sounds\\pickup.wav"));
					break;
				case Types::GOLD:
					m_pPlayer->SetScore(m_pPlayer->GetScore() + block->GetValue());
					SoundManager::GetInstance().PlaySound(SoundManager::GetInstance().GetSound("assets\\sounds\\pickup.wav"));
					break;
				case Types::HEART:
					m_pPlayer->SetLife(m_pPlayer->GetLife() + block->GetValue());
					SoundManager::GetInstance().PlaySound(SoundManager::GetInstance().GetSound("assets\\sounds\\pickup.wav"));
					break;
				case Types::MINE:
					m_pPlayer->SetLife(m_pPlayer->GetLife() + block->GetValue());
					
					if (m_pPlayer->GetLife() == 0)
					{
						SetGameState(GameStates::RESULT_SCREEN);
					}

					SoundManager::GetInstance().PlaySound(SoundManager::GetInstance().GetSound("assets\\sounds\\explosion.wav"));
					break;
				default:
					break;
				}

				if (block->GetType() != Types::CHEST)
					block->SetDiscovered(true);
			}
		}
	}

	m_pPlayer->Animate(deltaTime);
}

void Game::MovePlayerLeft()
{
	bool collided = false;
	auto dirtBlocks = m_pMap->GetDirtBlocks();

	Player tmpPlayer;
	tmpPlayer.SetHorizontalVelocity(m_pPlayer->GetHorizontalVelocity());
	tmpPlayer.IncreaseHorizontalVelocity();
	tmpPlayer.SetPosition(m_pPlayer->GetPositionX() - tmpPlayer.GetHorizontalVelocity(), m_pPlayer->GetPositionY());

	for (size_t width = 0; width < dirtBlocks.size() && !collided; ++width)
	{
		for (size_t height = 3; height < dirtBlocks[width].size() && !collided; ++height)
		{
			if (!dirtBlocks[width][height]->IsDiscovered() && tmpPlayer.IsCollidingWith(*dirtBlocks[width][height]))
			{
				collided = true;
			}
		}
	}

	if (!collided)
	{
		m_pPlayer->MoveLeft();
	}
}

void Game::MovePlayerRight()
{
	bool collided = false;
	auto dirtBlocks = m_pMap->GetDirtBlocks();

	Player tmpPlayer;
	tmpPlayer.SetHorizontalVelocity(m_pPlayer->GetHorizontalVelocity());
	tmpPlayer.IncreaseHorizontalVelocity();
	tmpPlayer.SetPosition(m_pPlayer->GetPositionX() + tmpPlayer.GetHorizontalVelocity(), m_pPlayer->GetPositionY());

	for (size_t width = 0; width < dirtBlocks.size() && !collided; ++width)
	{
		for (size_t height = 3; height < dirtBlocks[width].size() && !collided; ++height)
		{
			if (!dirtBlocks[width][height]->IsDiscovered() && tmpPlayer.IsCollidingWith(*dirtBlocks[width][height]))
			{
				collided = true;
			}
		}
	}

	if (!collided)
	{
		m_pPlayer->MoveRight();
	}
}

void Game::MovePlayerDown()
{
	bool collided = false;
	auto dirtBlocks = m_pMap->GetDirtBlocks();

	Player tmpPlayer;
	tmpPlayer.SetVerticalVelocity(m_pPlayer->GetVerticalVelocity());
	tmpPlayer.IncreaseVerticalVelocity();
	tmpPlayer.SetPosition(m_pPlayer->GetPositionX(), m_pPlayer->GetPositionY() + tmpPlayer.GetVerticalVelocity());

	for (size_t width = 0; width < dirtBlocks.size() && !collided; ++width)
	{
		for (size_t height = 3; height < dirtBlocks[width].size() && !collided; ++height)
		{
			if (!dirtBlocks[width][height]->IsDiscovered() && tmpPlayer.IsCollidingWith(*dirtBlocks[width][height]))
			{
				collided = true;
			}
		}
	}

	if (!collided)
	{
		m_pPlayer->MoveDown();
	}
}

void Game::PlayerDig(Player::DigDirections direction)
{
	if (!m_pPlayer->IsActionBlocked())
	{
		Player tmpPlayer;

		switch (direction)
		{
		case Player::DigDirections::DOWN:
			tmpPlayer.SetPosition(m_pPlayer->GetPositionX(), m_pPlayer->GetPositionY() + m_pPlayer->PLAYER_MAX_SPEED);
			break;
		case Player::DigDirections::LEFT:
			tmpPlayer.SetPosition(m_pPlayer->GetPositionX() - m_pPlayer->PLAYER_MAX_SPEED, m_pPlayer->GetPositionY());
			break;
		case Player::DigDirections::RIGHT:
			tmpPlayer.SetPosition(m_pPlayer->GetPositionX() + m_pPlayer->PLAYER_MAX_SPEED, m_pPlayer->GetPositionY());
			break;
		}

		bool foundBlock = false;
		auto dirtBlocks = m_pMap->GetDirtBlocks();

		for (size_t width = 0; width < dirtBlocks.size() && !foundBlock; ++width)
		{
			for (size_t height = 3; height < dirtBlocks[width].size() && !foundBlock; ++height)
			{
				if (!dirtBlocks[width][height]->IsDiscovered() && tmpPlayer.IsCollidingWith(*dirtBlocks[width][height]))
				{
					foundBlock = true;
					dirtBlocks[width][height]->SetDiscovered(true);

					switch (dirtBlocks[width][height]->GetDirtPosition())
					{
					case Dirt::DirtPosition::TOP_LEFT:
						dirtBlocks[width][height]->Initialise(Dirt::DirtPosition::TOP_LEFT_DIGGED);
						break;
					case Dirt::DirtPosition::TOP:
						dirtBlocks[width][height]->Initialise(Dirt::DirtPosition::TOP_DIGGED);
						break;
					case Dirt::DirtPosition::TOP_RIGHT:
						dirtBlocks[width][height]->Initialise(Dirt::DirtPosition::TOP_RIGHT_DIGGED);
						break;
					case Dirt::DirtPosition::MIDDLE_LEFT:
						dirtBlocks[width][height]->Initialise(Dirt::DirtPosition::MIDDLE_LEFT_DIGGED);
						break;
					case Dirt::DirtPosition::MIDDLE:
						dirtBlocks[width][height]->Initialise(Dirt::DirtPosition::MIDDLE_DIGGED);
						break;
					case Dirt::DirtPosition::MIDDLE_RIGHT:
						dirtBlocks[width][height]->Initialise(Dirt::DirtPosition::MIDDLE_RIGHT_DIGGED);
						break;
					}
				}
			}
		}

		if (foundBlock)
		{
			m_pPlayer->Dig();
		}
	}
}

Map * Game::GetMap()
{
	return m_pMap;
}

int Game::GetNumBlocksWidth()
{
	return m_numBlocksWidth;
}

int Game::GetNumBlocksHeight()
{
	return m_numBlocksHeight;
}

void 
Game::Quit()
{
	m_looping = false;
}

void Game::SetupNewGame()
{
	m_pMap->InitializeNewMap();

	auto backgroundBlocks = m_pMap->GetBackgroundBlocks();
	assert(backgroundBlocks.size());

	for (size_t width = 0; width < backgroundBlocks.size(); ++width)
	{
		for (size_t height = 3; height < backgroundBlocks[width].size(); ++height)
		{
			backgroundBlocks[width][height]->Initialise();
			backgroundBlocks[width][height]->SetPosition(-m_blockWidth + m_blockWidth * (float)width, m_blockHeight * (float)height);
		}
	}
	
	auto dirtBlocks = m_pMap->GetDirtBlocks();
	assert(dirtBlocks.size());

	for (size_t width = 0; width < dirtBlocks.size(); ++width)
	{
		for (size_t height = 3; height < dirtBlocks[width].size(); ++height)
		{
			if (height == 3)
			{
				if (width == 0)
					dirtBlocks[width][height]->Initialise(Dirt::DirtPosition::TOP_LEFT);
				else if (width == m_numBlocksWidth - 1)
					dirtBlocks[width][height]->Initialise(Dirt::DirtPosition::TOP_RIGHT);
				else
					dirtBlocks[width][height]->Initialise(Dirt::DirtPosition::TOP);
			}
			else
			{
				if (width == 0)
					dirtBlocks[width][height]->Initialise(Dirt::DirtPosition::MIDDLE_LEFT);
				else if (width == m_numBlocksWidth - 1)
					dirtBlocks[width][height]->Initialise(Dirt::DirtPosition::MIDDLE_RIGHT);
				else
					dirtBlocks[width][height]->Initialise(Dirt::DirtPosition::MIDDLE);
			}

			dirtBlocks[width][height]->SetDiscovered(false);
			dirtBlocks[width][height]->SetPosition(m_blockWidth * static_cast<float>(width), m_blockHeight * static_cast<float>(height));
		}
	}

	for (int width = 0; width < m_numBlocksWidth; ++width)
	{
		for (int height = 0; height < m_numBlocksHeight; ++height)
		{
			auto block = m_pMap->GetBlock(width, height);
			assert(block);

			switch (block->GetType())
			{
			case Types::CHEST:
				static_cast<Chest*>(block)->Initialise();
				break;
			case Types::DIAMOND:
				static_cast<Diamond*>(block)->Initialise();
				break;
			case Types::GOLD:
				static_cast<Gold*>(block)->Initialise();
				break;
			case Types::HEART:
				static_cast<Heart*>(block)->Initialise();
				break;
			case Types::MINE:
				static_cast<Mine*>(block)->Initialise();
				break;
			}

			block->SetPosition(m_blockWidth * static_cast<float>(width), m_blockHeight * static_cast<float>(height));
		}
	}

	assert(m_pPlayer);
	m_pPlayer->Initialise();
	m_pPlayer->SetPosition(floorf(m_numBlocksWidth / 2.f) * m_blockWidth, 3.f * m_blockHeight);
}

Player* Game::GetPlayer()
{
	return this->m_pPlayer;
}

int Game::GetWindowWidth()
{
	return this->m_windowWidth;
}

int Game::GetWindowHeight()
{
	return this->m_windowHeight;
}

GameStates Game::GetGameState()
{
	return m_gameState;
}

void Game::SetGameState(GameStates state)
{
	this->m_gameState = state;
}
