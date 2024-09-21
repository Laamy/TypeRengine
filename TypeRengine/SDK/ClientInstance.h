#ifndef CLIENTINSTANCE_H
#define CLIENTINSTANCE_H

#include <entt/entt.hpp>

#include "../Engine/GameEngine.h"
#include "../Engine/Level.h"

class ClientInstance {
public:
	std::shared_ptr<entt::basic_registry<entt::entity>> EnttRegistry;
	GameEngine* GameEngine;

	Level level = Level();

public:
	__forceinline static ClientInstance* GetSingle() {
		static ClientInstance* Instance = new ClientInstance();
		return Instance;
	}
};

#endif // CLIENTINSTANCE_H