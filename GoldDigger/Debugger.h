#ifndef __DEBUGGER_H__
#define __DEBUGGER_H__

struct SDL_Renderer;
struct SDL_Window;
struct SDL_Texture;
class Game;

class Debugger
{
public:
	~Debugger();

	static Debugger& GetInstance();
	static void DestroyInstance();

	void Initialize(Game* game);
	Uint32 GetWindowID();
	void ProcessInput(SDL_Event& e);
	void Draw();

private:
	Debugger();
	Debugger(const Debugger& debug);
	
private:
	static Debugger* m_pInstance;

	Game* m_pGame;
	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;
	unsigned __int32 m_windowID;
};

#endif // !__DEBUGGER_H__