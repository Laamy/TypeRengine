#ifndef GAME_H
#define GAME_H

#include "Engine/GameEngine.h"

#include "Engine/Service/Services/DefaultWinBindsService.h"
#include "SDK/ClientInstance.h"

class Game : public GameEngine {
public:
	Game()
	{
		ClientInstance* instance = ClientInstance::GetSingle();

		instance->EnttRegistry = std::make_shared<entt::basic_registry<entt::entity>>();
		instance->GameEngine = this;

		OnUpdate.Hook([this](sf::RenderWindow* window) { this->OnUpdateEvent(window); });

		// setup some services
		{
			Services.push_back(new DefaultWinBindsService(this));
		}
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