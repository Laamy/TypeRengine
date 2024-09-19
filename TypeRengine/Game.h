#ifndef GAME_H
#define GAME_H

#include "Engine/GameEngine.h"

class Game : public GameEngine {
public:
	Game()
	{
		OnUpdate.Hook([this](sf::RenderWindow* window) { this->OnUpdateEvent(window); });
	}

	void OnUpdateEvent(sf::RenderWindow* window)
	{
		// clear for next frame (once i add the skybox I'll remove this call to save resources
		window->clear(sf::Color::Black);

		// swapbuffers
		window->display();
	}
};

#endif // GAME_H