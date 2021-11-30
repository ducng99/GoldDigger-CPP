// COMP710 GP 2D Framework 2020

// This include:
#include "entity.h"

// Local includes:
#include "sprite.h"
#include "backbuffer.h"

// Library includes:
#include <cassert>

Entity::Entity()
: m_pSprite(0)
, m_x(0.0f)
, m_y(0.0f)
, m_velocityX(0.0f)
, m_velocityY(0.0f)
{

}

Entity::~Entity()
{
	m_pSprite = 0;
}

bool
Entity::Initialise(Sprite* sprite)
{
	assert(sprite);
	m_pSprite = sprite;

	return (true);
}

void 
Entity::Draw(BackBuffer& backBuffer)
{
	assert(m_pSprite);
	m_pSprite->SetX(static_cast<int>(m_x));
	m_pSprite->SetY(static_cast<int>(m_y));
	m_pSprite->Draw(backBuffer);
}

bool
Entity::IsCollidingWith(Entity& e)
{
	if (this->GetPositionX() < e.GetPositionX() + 80 && this->GetPositionX() + 80 > e.GetPositionX() &&
		this->GetPositionY() < e.GetPositionY() + 80 && this->GetPositionY() + 80 > e.GetPositionY())
		return true;

	return false;
}

void
Entity::SetPosition(float x, float y)
{
	m_x = x;
	m_y = y;
}

void
Entity::SetPositionX(float x)
{
	m_x = x;
}

void
Entity::SetPositionY(float y)
{
	m_y = y;
}

float 
Entity::GetPositionX() const
{
	return (m_x);
}

float 
Entity::GetPositionY() const
{
	return (m_y);
}

float 
Entity::GetHorizontalVelocity() const
{
	return (m_velocityX);
}

float 
Entity::GetVerticalVelocity() const
{
	return (m_velocityY);
}

void 
Entity::SetHorizontalVelocity(float x)
{
	m_velocityX = x;
}

void 
Entity::SetVerticalVelocity(float y)
{
	m_velocityY = y;
}
