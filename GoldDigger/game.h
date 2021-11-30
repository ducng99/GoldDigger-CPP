// COMP710 GP 2D Framework 2020
#ifndef __GAME_H__
#define __GAME_H__

#include "player.h"

// Forward Declarations
class BackBuffer;
class InputHandler;
class Sprite;
class Map;

enum class GameStates
{
	GAME_START, AUT_SPLASH_SCREEN, MIDDLEWARE_SPLASH_SCREEN, MAIN_MENU, IN_GAME, RESULT_SCREEN, END_GAME
};

class Game
{
	//Member Methods:
public:
	static Game& GetInstance();
	static void DestroyInstance();
	~Game();

	bool Initialise();
	bool DoGameLoop();
	void Quit();
	void SetupNewGame();

	Player* GetPlayer();
	void MovePlayerLeft();
	void MovePlayerRight();
	void MovePlayerDown();
	void PlayerDig(Player::DigDirections direction);

	Map* GetMap();
	int GetNumBlocksWidth();
	int GetNumBlocksHeight();

	int GetWindowWidth();
	int GetWindowHeight();

	GameStates GetGameState();
	void SetGameState(GameStates state);
protected:
	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

private:
	Game(const Game& game);
	Game& operator=(const Game& game);
	
	Game();

	void ProcessGameplay(float deltaTime);

	//Member Data:
public:
	
protected:
	static Game* sm_pInstance;
	BackBuffer* m_pBackBuffer;
	InputHandler* m_pInputHandler;
	bool m_looping;

	// Simulation Counters:
	float m_elapsedSeconds;
	float m_lag;
	float m_executionTime;
	__int64 m_lastTime;
	int m_frameCount;
	int m_FPS;
	int m_numUpdates;
	bool m_drawDebugInfo;

private:
	int m_windowWidth;
	int m_windowHeight;

	int m_numBlocksWidth;
	int m_numBlocksHeight;

	int m_blockWidth;
	int m_blockHeight;

	const float SPLASH_SHOW_TIME = 4;

	GameStates m_gameState;

	Player* m_pPlayer;
	Map* m_pMap;

	float m_splashTimer;
	Sprite* m_pAUTLogoSprite;
	Sprite* m_pMWLogosSprite;
};

#endif // __GAME_H__
