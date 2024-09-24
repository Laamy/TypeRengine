#ifndef GAME_H
#define GAME_H

#include "Engine/GameEngine.h"

#include "Engine/Service/Services/DefaultWinBindsService.h" // inbuilt service
#include "SDK/Services/Camera2DService.h"

#include "SDK/ClientInstance.h"

#include "Engine/Level.h"
#include "Engine/Objects/SolidObject.h"

class Game : public GameEngine {
public:
	ClientInstance* instance;

public:
	Game();

	void OnUpdateEvent(sf::RenderWindow* window);
};

#endif // GAME_H