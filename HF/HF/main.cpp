#include "game.h"
#include "SDL.h"
#include <windows.h>
#include "PrefsManager.h"

using namespace std;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SDL_Init(SDL_INIT_VIDEO);
	srand(0);
	Game game;
	game.Run();
	return 0;
}
