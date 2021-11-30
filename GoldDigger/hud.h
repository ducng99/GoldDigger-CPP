#ifndef __HUD_H__
#define __HUD_H__

#include "ButtonTypes.h"
#include "game.h"
#include <map>

class BackBuffer;
class Sprite;
class HudButton;

enum class Buttons
{
	MainMenuPlayButton, MainMenuCloseButton, IngameEndButton, ResultPlayButton, ResultCloseButton, HelpButton
};

class HudManager
{
public:
	~HudManager();

	static HudManager& GetInstance();
	static void DestroyInstance();
	static void InitialiseSprites(BackBuffer* backBuffer);

	void Initialise(BackBuffer* backBuffer);

	void DrawMainMenu();
	void DrawIngame();
	void DrawResult(Player* player);
	void DrawInstructions();

	void ButtonClick(int x, int y);
	
private:
	HudManager();
	HudManager(const HudManager& hud);
	HudManager& operator=(const HudManager& hud);

private:
	BackBuffer* m_pBackBuffer;
	static std::map<ButtonTypes, GameStates> m_buttonAction;

	static Sprite* m_pBackground;
	static Sprite* m_pResultPanel;
	static Sprite* m_pInstructionSprite;

	static HudManager* sm_pInstance;

	bool m_bShowInstructions;
	std::map<Buttons, HudButton*> m_pButtons;
};

#endif // !__HUD_H__