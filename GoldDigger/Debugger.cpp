#include <SDL.h>
#include "imgui.h"
#include "imgui_sdl.h"

#include "debugger.h"
#include "game.h"
#include "logmanager.h"
#include "map.h"
#include "player.h"
#include "GameObject.h"

#include <string>
#include <cassert>

Debugger* Debugger::m_pInstance = 0;

Debugger::Debugger()
: m_pWindow(0)
, m_pRenderer(0)
, m_windowID(0)
, m_pGame(0)
{

}

Debugger::~Debugger()
{
	ImGuiSDL::Deinitialize();

	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);

	ImGui::DestroyContext();

	m_pGame = 0;
}

Debugger & Debugger::GetInstance()
{
	if (m_pInstance == 0)
	{
		m_pInstance = new Debugger();
	}

	assert(m_pInstance);
	return *m_pInstance;
}

void Debugger::DestroyInstance()
{
	delete m_pInstance;
	m_pInstance = 0;
}

void Debugger::Initialize(Game* game)
{
	SDL_Init(SDL_INIT_VIDEO);
	m_pWindow = SDL_CreateWindow("Debugger", 210 + Game::GetInstance().GetWindowWidth(), SDL_WINDOWPOS_UNDEFINED, 420, 600, SDL_WINDOW_RESIZABLE);
	m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED);

	m_windowID = SDL_GetWindowID(m_pWindow);
	
	ImGui::CreateContext();
	ImGuiSDL::Initialize(m_pRenderer, 420, 600);

	m_pGame = game;
}

Uint32 Debugger::GetWindowID()
{
	return m_windowID;
}

void Debugger::ProcessInput(SDL_Event& e)
{
	ImGuiIO& io = ImGui::GetIO();

	int wheel = 0;

	if (e.type == SDL_WINDOWEVENT)
	{
		if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
		{
			io.DisplaySize.x = static_cast<float>(e.window.data1);
			io.DisplaySize.y = static_cast<float>(e.window.data2);
		}
	}
	else if (e.type == SDL_MOUSEWHEEL)
	{
		wheel = e.wheel.y;
	}
	else if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
	{
		int key = e.key.keysym.scancode;
		IM_ASSERT(key >= 0 && key < IM_ARRAYSIZE(io.KeysDown));
		io.KeysDown[key] = (e.type == SDL_KEYDOWN);
		io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
		io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
		io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
	}
	else if (e.type == SDL_TEXTINPUT)
	{
		io.AddInputCharactersUTF8(e.text.text);
	}

	int mouseX, mouseY;
	const int buttons = SDL_GetMouseState(&mouseX, &mouseY);

	io.DeltaTime = 1.0f / 60.0f;
	io.MousePos = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
	io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
	io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
	io.MouseWheel = static_cast<float>(wheel);
}

void Debugger::Draw()
{
	ImGui::NewFrame();

	// Player Debugger
	{
		ImGui::Begin("Player debug", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
		ImGui::SetWindowPos(ImVec2(10, 10));
		ImGui::SetWindowSize(ImVec2(400, 200));

		Player* player = m_pGame->GetPlayer();

		if (player != 0 && m_pGame->GetGameState() == GameStates::IN_GAME)
		{
			ImGui::InputText("Name", player->GetName(), 16);

			int score = player->GetScore();
			ImGui::InputInt("Score", &score, 5, 10);
			player->SetScore(score);

			int life = player->GetLife();
			ImGui::InputInt("Life", &life, 1, 2);
			player->SetLife(life);

			ImGui::Text("Position:");
			float posX = player->GetPositionX();
			float posY = player->GetPositionY();
			ImGui::InputFloat("X", &posX);
			ImGui::InputFloat("Y", &posX);
			player->SetPosition(posX, posY);

		}
		else
		{
			ImGui::Text("Please enter a game to start debugging");
		}

		ImGui::End();
	}

	// Map debugger
	{
		ImGui::Begin("Map debug", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
		ImGui::SetWindowPos(ImVec2(10, 220));
		ImGui::SetWindowSize(ImVec2(400, 320));

		Map* map = m_pGame->GetMap();

		if (map != 0 && m_pGame->GetGameState() == GameStates::IN_GAME)
		{
			for (int height = 0; height < m_pGame->GetNumBlocksHeight(); ++height)
			{
				for (int width = 0; width < m_pGame->GetNumBlocksWidth(); ++width)
				{
					auto block = map->GetBlock(width, height);

					std::string buttonID;
					if (block->GetType() != Types::UNKNOWN)
					{
						buttonID += block->IsDiscovered() ? "X" : "V";
						buttonID += "##block-" + to_string(width) + "-" + to_string(height);
					}

					if (ImGui::Button(buttonID.c_str(), ImVec2(20, 20)))
					{
						block->SetDiscovered(!block->IsDiscovered());
					}

					if (width != m_pGame->GetNumBlocksWidth() - 1)
						ImGui::SameLine();
				}
			}
		}
		else
		{
			ImGui::Text("Please enter a game to start debugging");
		}

		ImGui::End();
	}

	ImGui::Render();

	SDL_SetRenderDrawColor(m_pRenderer, 0xe, 0xe, 0xe, 0xff);
	SDL_RenderClear(m_pRenderer);

	// There is a bug when moving windows, leaving a trail behind. Clearing renderer doesn't help, however resizing does
	ImGuiSDL::Render(ImGui::GetDrawData());

	SDL_RenderPresent(m_pRenderer);
}
