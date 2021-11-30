#include "player.h"
#include "logmanager.h"
#include "game.h"
#include "backbuffer.h"
#include "sprite.h"
#include "sound.h"

#include <cassert>
#include <cstdlib>
#include <cstring>

Sprite* Player::m_idleLeftSprites[4];
Sprite* Player::m_idleRightSprites[4];
Sprite* Player::m_moveLeftSprites[6];
Sprite* Player::m_moveRightSprites[6];
Sprite* Player::m_digLeftSprites[4];
Sprite* Player::m_digRightSprites[4];

Player::Player() : Entity(),
	name("UNKNOWN"),
	life(3),
	score(0),
	lastAnimation(0),
	animationStep(0),
	state(PlayerStates::Idle_Right),
	blockAction(false)
{

}

Player::Player(const char* name) : Entity(),
	name("UNKNOWN"),
	life(3),
	score(0),
	lastAnimation(0),
	animationStep(0),
	state(PlayerStates::Idle_Right),
	blockAction(false)
{
	SetName(name);
}

void Player::Initialise()
{
	m_pSprite = m_idleRightSprites[0];
}

void Player::InitialiseSprites(BackBuffer* backBuffer)
{
	for (int i = 0; i < 4; i++)
	{
		m_idleLeftSprites[i] = backBuffer->CreateSprite("assets\\imgs\\player\\Man_idle_left.png", 80 * i, 0, 80, 80);
		m_idleRightSprites[i] = backBuffer->CreateSprite("assets\\imgs\\player\\Man_idle_right.png", 80 * i, 0, 80, 80);
		m_digLeftSprites[i] = backBuffer->CreateSprite("assets\\imgs\\player\\Man_dig_left.png", 80 * i, 0, 80, 80);
		m_digRightSprites[i] = backBuffer->CreateSprite("assets\\imgs\\player\\Man_dig_right.png", 80 * i, 0, 80, 80);
	}

	for (int i = 0; i < 6; i++)
	{
		m_moveLeftSprites[i] = backBuffer->CreateSprite("assets\\imgs\\player\\Man_walk_left.png", 80 * i, 0, 80, 80);
		m_moveRightSprites[i] = backBuffer->CreateSprite("assets\\imgs\\player\\Man_walk_right.png", 80 * i, 0, 80, 80);
	}
}

void Player::DestroySprites()
{
	for (int i = 0; i < 4; i++)
	{
		delete m_idleLeftSprites[i];
		delete m_idleRightSprites[i];
		delete m_digLeftSprites[i];
		delete m_digRightSprites[i];

		m_idleLeftSprites[i] = 0;
		m_idleRightSprites[i] = 0;
		m_digLeftSprites[i] = 0;
		m_digRightSprites[i] = 0;
	}

	for (int i = 0; i < 6; i++)
	{
		delete m_moveLeftSprites[i];
		delete m_moveRightSprites[i];

		m_moveLeftSprites[i] = 0;
		m_moveRightSprites[i] = 0;
	}
}

void Player::Animate(float deltaTime)
{
	lastAnimation += deltaTime;

	if (lastAnimation >= 0.25f)
	{
		lastAnimation = 0.f;

		switch (state)
		{
		case Idle_Left:
			m_pSprite = m_idleLeftSprites[animationStep++ % 4];
			if (animationStep == 4)
				animationStep = 0;
			break;
		case Idle_Right:
			m_pSprite = m_idleRightSprites[animationStep++ % 4];
			if (animationStep == 4)
				animationStep = 0;
			break;
		case Moving_Left:
			m_pSprite = m_moveLeftSprites[animationStep++ % 6];
			if (animationStep == 6)
				animationStep = 0;
			break;
		case Moving_Right:
			m_pSprite = m_moveRightSprites[animationStep++ % 6];
			if (animationStep == 6)
				animationStep = 0;
			break;
		case Digging_Left:
			m_pSprite = m_digLeftSprites[animationStep++ % 4];
			if (animationStep == 4)
			{
				animationStep = 0;
				blockAction = false;
			}
			break;
		case Digging_Right:
			m_pSprite = m_digRightSprites[animationStep++ % 4];
			if (animationStep == 4)
			{
				animationStep = 0;
				blockAction = false;
			}
			break;
		}
	}
}

bool Player::IsActionBlocked()
{
	return this->blockAction;
}

void Player::MoveLeft()
{
	if (!blockAction)
	{
		if (GetState() != Moving_Left)
			SetState(PlayerStates::Moving_Left);

		this->IncreaseHorizontalVelocity();
		float newPosX = this->GetPositionX() - this->GetHorizontalVelocity();

		if (newPosX >= 0)
			this->SetPositionX(newPosX);
	}
}

void Player::MoveRight()
{
	if (!blockAction)
	{
		if (GetState() != Moving_Right)
			SetState(PlayerStates::Moving_Right);

		this->IncreaseHorizontalVelocity();
		float newPosX = this->GetPositionX() + this->GetHorizontalVelocity();

		if (newPosX < Game::GetInstance().GetWindowWidth())
			this->SetPositionX(newPosX);
	}
}

void Player::MoveDown()
{
	if (!blockAction)
	{
		if (GetState() == Idle_Left || GetState() == Digging_Left)
		{
			SetState(Moving_Left);
		}
		else if (GetState() == Idle_Right || GetState() == Digging_Right)
		{
			SetState(Moving_Right);
		}

		this->IncreaseVerticalVelocity();
		float newPosY = this->GetPositionY() + this->GetVerticalVelocity();

		if (newPosY < Game::GetInstance().GetWindowHeight())
			this->SetPositionY(newPosY);
	}
}

void Player::Stand()
{
	this->DecreaseHorizontalVelocity();
	this->DecreaseVerticalVelocity();

	if (!blockAction)
	{
		if (GetState() == Moving_Left || GetState() == Digging_Left)
		{
			SetState(Idle_Left);
		}
		else if (GetState() == Moving_Right || GetState() == Digging_Right)
		{
			SetState(Idle_Right);
		}
	}
}

void Player::Dig()
{
	blockAction = true;

	if (GetState() == Moving_Left || GetState() == Idle_Left)
	{
		SetState(Digging_Left);
	}
	else if (GetState() == Moving_Right || GetState() == Idle_Right)
	{
		SetState(Digging_Right);
	}

	SoundManager::GetInstance().PlaySound(SoundManager::GetInstance().GetSound("assets\\sounds\\dig.wav"));
}

bool Player::IsCollidingWith(Entity& e)
{
	if (this->GetPositionX() - 18.5f < e.GetPositionX() + 80 && this->GetPositionX() + 18.5f > e.GetPositionX() &&
		this->GetPositionY() - 78 < e.GetPositionY() + 80 && this->GetPositionY() > e.GetPositionY())
		return true;

	return false;
}

void Player::Draw(BackBuffer & backBuffer)
{
	assert(m_pSprite);
	m_pSprite->SetX(static_cast<int>(m_x - 30));
	m_pSprite->SetY(static_cast<int>(m_y - 80));
	m_pSprite->Draw(backBuffer);
}

void Player::IncreaseHorizontalVelocity()
{
	if (this->GetHorizontalVelocity() < PLAYER_MAX_SPEED)
	{
		this->SetHorizontalVelocity(this->GetHorizontalVelocity() + 0.2f);
	}
}

void Player::DecreaseHorizontalVelocity()
{
	if (this->GetHorizontalVelocity() > 0)
	{
		this->SetHorizontalVelocity(this->GetHorizontalVelocity() - 0.2f);
	}
}

void Player::IncreaseVerticalVelocity()
{
	if (this->GetVerticalVelocity() < PLAYER_MAX_SPEED)
	{
		this->SetVerticalVelocity(this->GetVerticalVelocity() + 0.2f);
	}
}

void Player::DecreaseVerticalVelocity()
{
	if (this->GetVerticalVelocity() > 0)
	{
		this->SetVerticalVelocity(this->GetVerticalVelocity() - 0.2f);
	}
}

char* Player::GetName()
{
	return this->name;
}

void Player::SetName(const char* name)
{
	if (strlen(name) > 0 && strlen(name) <= 16)
		strcpy_s(this->name, name);
}

int Player::GetLife()
{
	return this->life;
}

void Player::SetLife(int life)
{
	if (life >= 0 && life <= 3)
	{
		this->life = life;
	}
}

int Player::GetScore()
{
	return this->score;
}

void Player::SetScore(int score)
{
	if (score > 0)
		this->score = score;
}

Player::PlayerStates Player::GetState()
{
	return this->state;
}

void Player::SetState(PlayerStates state)
{
	this->state = state;
	lastAnimation = 1;
	animationStep = 0;
}