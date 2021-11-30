#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity.h"

using namespace std;

class Player : public Entity
{
public:
	Player();
	Player(const char* name);

	enum PlayerStates
	{
		Idle_Right, Idle_Left,
		Moving_Left, Moving_Right,
		Digging_Left, Digging_Right,
		Dead_Left, Dead_Right,
		Hurt_Left, Hurt_Right
	};

	enum DigDirections
	{
		LEFT, RIGHT, DOWN
	};

	void Initialise();
	static void InitialiseSprites(BackBuffer* backBuffer);
	static void DestroySprites();

	void MoveLeft();
	void MoveRight();
	void MoveDown();
	void Stand();
	void Dig();

	char* GetName();
	void SetName(const char* name);
	int GetScore();
	void SetScore(int score);
	int GetLife();
	void SetLife(int life);
	PlayerStates GetState();
	void SetState(PlayerStates state);
	void Animate(float deltaTime);
	bool IsActionBlocked();
	bool IsCollidingWith(Entity& e);
	void Draw(BackBuffer& backBuffer);

	void IncreaseHorizontalVelocity();
	void DecreaseHorizontalVelocity();
	void IncreaseVerticalVelocity();
	void DecreaseVerticalVelocity();
public:
	const float PLAYER_MAX_SPEED = 2.5f;
private:
	static Sprite* m_idleLeftSprites[4];
	static Sprite* m_idleRightSprites[4];
	static Sprite* m_moveLeftSprites[6];
	static Sprite* m_moveRightSprites[6];
	static Sprite* m_digLeftSprites[4];
	static Sprite* m_digRightSprites[4];

	char name[16];
	int score;
	int life;
	PlayerStates state;

	float lastAnimation;
	int animationStep;
	bool blockAction;
};

#endif // !__PLAYER_H__