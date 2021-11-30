#include "HudButton.h"
#include "backbuffer.h"
#include "sprite.h"

HudButton::HudButton()
: m_pSprite(0)
{
}

HudButton::~HudButton()
{
	delete m_pSprite;
	m_pSprite = 0;
}

void HudButton::Initialise(Sprite * sprite, ButtonTypes type)
{
	m_pSprite = sprite;
	m_type = type;
}

void HudButton::Draw(BackBuffer * backBuffer)
{
	backBuffer->DrawSprite(*m_pSprite);
}

void HudButton::SetPosition(int x, int y)
{
	m_pSprite->SetX(x);
	m_pSprite->SetY(y);
}

int HudButton::GetX()
{
	return m_pSprite->GetX();
}

int HudButton::GetY()
{
	return m_pSprite->GetY();
}

int HudButton::GetWidth()
{
	return m_pSprite->GetWidth();
}

int HudButton::GetHeight()
{
	return m_pSprite->GetHeight();
}

ButtonTypes HudButton::GetType()
{
	return m_type;
}
