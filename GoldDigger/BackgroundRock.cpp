#include "BackgroundRock.h"
#include "backbuffer.h"
#include "sprite.h"

Sprite* BackgroundRock::m_sprites[1];

void BackgroundRock::Initialise()
{
	m_pSprite = m_sprites[0];
}

void BackgroundRock::InitialiseSprites(BackBuffer * backBuffer)
{
	m_sprites[0] = backBuffer->CreateSprite("assets\\imgs\\tileset.png", 80 * 4, 160, 80, 80);
}

void BackgroundRock::DestroySprites()
{
	delete m_sprites[0];
	m_sprites[0] = 0;
}
