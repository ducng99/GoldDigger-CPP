#include "Chest.h"
#include "backbuffer.h"
#include "sprite.h"

Sprite* Chest::m_sprites[2];

void Chest::Initialise()
{
	m_pSprite = m_sprites[0];
}

void Chest::InitialiseSprites(BackBuffer* backBuffer)
{
	m_sprites[0] = backBuffer->CreateSprite("assets\\imgs\\Objects.png", 0, 80, 80, 80);
	m_sprites[1] = backBuffer->CreateSprite("assets\\imgs\\Objects.png", 80, 80, 80, 80);
}

void Chest::DestroySprites()
{
	for (int i = 0; i < 2; i++)
	{
		delete m_sprites[i];
		m_sprites[i] = 0;
	}
}

void Chest::OpenChest()
{
	m_pSprite = m_sprites[1];
}