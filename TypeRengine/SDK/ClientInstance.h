#ifndef CLIENTINSTANCE_H
#define CLIENTINSTANCE_H

#include <entt/entt.hpp>

class ClientInstance {
public:
	entt::basic_registry<entt::entity>* EnttRegistry;
	class GameEngine* GameEngine;

	class Level* level;

public:
	__forceinline static ClientInstance* GetSingle() {
		static ClientInstance* Instance = new ClientInstance();
		return Instance;
	}
};

#endif // CLIENTINSTANCE_H