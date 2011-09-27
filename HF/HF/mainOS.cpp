#include "game.h"
#include "SDL.h"
#include "PrefsManager.h"
#include <string>

using namespace std;

int SDL_main(int argc, char **argv)
{
	SDL_Init(0);
	srand(0);
	Game game;
	game.Run();
	SDL_Quit();
	return 0;
}
