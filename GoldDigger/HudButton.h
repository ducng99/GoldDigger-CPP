#ifndef __HUDBUTTON_H__
#define __HUDBUTTON_H__

#include "ButtonTypes.h"

class Sprite;
class BackBuffer;

class HudButton
{
public:
	HudButton();
	~HudButton();

	void Initialise(Sprite* sprite, ButtonTypes type);
	void Draw(BackBuffer* backBuffer);

	void SetPosition(int x, int y);
	int GetX();
	int GetY();
	int GetWidth();
	int GetHeight();
	ButtonTypes GetType();
private:
	HudButton(const HudButton& hud);
	HudButton& operator=(const HudButton& hudButton);

private:
	Sprite* m_pSprite;
	ButtonTypes m_type;
};

#endif // !__HUDBUTTON_H__