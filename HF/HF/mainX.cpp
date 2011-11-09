
#include "game.h"
#include "SDL.h"

using namespace std;

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	srand(0);
	Game game;
	game.Run();
	SDL_Quit();
	return 0;
}
