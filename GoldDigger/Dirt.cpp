#include "Dirt.h"
#include "backbuffer.h"
#include "sprite.h"

Sprite* Dirt::m_sprites[6];

void Dirt::InitialiseSprites(BackBuffer* backBuffer)
{
	for (int i = 0; i < 6; i++)
	{
		m_sprites[i] = backBuffer->CreateSprite("assets\\imgs\\tileset.png", 80 * (i % 3), 80 + 80 * (i / 3), 80, 80);
	}
}

void Dirt::DestroySprites()
{
	for (int i = 0; i < 6; i++)
	{
		delete m_sprites[i];
		m_sprites[i] = 0;
	}
}

void Dirt::Initialise(DirtPosition position)
{
	this->m_position = position;

	switch (m_position)
	{
	case TOP_LEFT:
		m_pSprite = m_sprites[0];
		break;
	case TOP:
		m_pSprite = m_sprites[1];
		break;
	case TOP_RIGHT:
		m_pSprite = m_sprites[2];
		break;
	case MIDDLE_LEFT:
		m_pSprite = m_sprites[3];
		break;
	case MIDDLE:
		m_pSprite = m_sprites[4];
		break;
	case MIDDLE_RIGHT:
		m_pSprite = m_sprites[5];
		break;
	}
}

Dirt::DirtPosition Dirt::GetDirtPosition()
{
	return this->m_position;
}
