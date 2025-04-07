#include "gamestate.h"

int main(int argc, char* argv[])
{
	Gamestate game;
	game.init();

	if (!game.show_menu())
	{
		game.cleanup();
		return 0;
	}

	while (game.running)
	{
		game.render();
		game.input();
		game.update();
	}

	game.cleanup();
	
	return 0;
}