#include "game.h"
#include "SDL.h"
#include <windows.h>
#include "PrefsManager.h"

using namespace std;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SDL_Init(0);
	srand(0);
	Game game;
	game.Run();
	SDL_Quit();
	return 0;
}
