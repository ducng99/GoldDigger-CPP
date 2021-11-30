#include "Heart.h"
#include "backbuffer.h"
#include "sprite.h"

Sprite* Heart::m_sprites[1];

void Heart::InitialiseSprites(BackBuffer * backBuffer)
{
	m_sprites[0] = backBuffer->CreateSprite("assets\\imgs\\Objects.png", 160, 80, 80, 80);
}

void Heart::DestroySprites()
{
	delete m_sprites[0];
	m_sprites[0] = 0;
}

void Heart::Initialise()
{
	m_pSprite = m_sprites[0];
}
