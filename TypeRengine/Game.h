#ifndef GAME_H
#define GAME_H

#include "Engine/GameEngine.h"

#include "Engine/Service/Services/DefaultWinBindsService.h"
#include "SDK/ClientInstance.h"

#include "Engine/Level.h"
#include "Engine/Objects/SolidObject.h"

class Game : public GameEngine {
public:
	ClientInstance* instance;

public:
	Game()
	{
		DebugLogger::Print(LogType::Info, "Game/GameEngine : Initializing");

		// get the clients instance
		instance = ClientInstance::GetSingle();
		
		// setup references & registry
		instance->EnttRegistry = new entt::basic_registry<entt::entity>();
		instance->GameEngine = this;
		instance->level = new Level();

		// hook render event
		OnUpdate.Hook([this](sf::RenderWindow* window) { this->OnUpdateEvent(window); });

		// setup some services
		{
			Services.push_back(new DefaultWinBindsService(this)); // f11 & escape keybinds
		}
		
		// add some sample objects into da fucking scene
		{
			SolidObject* tmp = new SolidObject();

			// setup object dimensions via its AABBShapeComponent
			auto shape = tmp->GetShape();
			shape->Size = sf::Vector2f(100, 100);

			// add object to scene
			instance->level->GetLayer(LevelLayers::Background)->AddObject(tmp);
		}

		DebugLogger::Print(LogType::Info, "Game/GameEngine : Initialized");
	}

	void OnUpdateEvent(sf::RenderWindow* window)
	{
		// clear for next frame (once i add the skybox I'll remove this call to save resources
		window->clear(sf::Color::Black);

		// render the scene
		instance->level->Draw();

		// swapbuffers
		window->display();
	}
};

#endif // GAME_H