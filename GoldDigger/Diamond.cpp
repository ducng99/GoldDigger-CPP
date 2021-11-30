#include "Diamond.h"
#include "backbuffer.h"
#include "sprite.h"

Sprite* Diamond::m_sprites[1];

void Diamond::Initialise()
{
	m_pSprite = m_sprites[0];
}

void Diamond::InitialiseSprites(BackBuffer * backBuffer)
{
	m_sprites[0] = backBuffer->CreateSprite("assets\\imgs\\Objects.png", 80, 0, 80, 80);
}

void Diamond::DestroySprites()
{
	delete m_sprites[0];
	m_sprites[0] = 0;
}
