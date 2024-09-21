#ifndef ENTITYCONTEXT_H
#define ENTITYCONTEXT_H

#include <cstdint>
#include <entt/entt.hpp>

#include "../Components/IEntityComponent.h"
#include "../../SDK/ClientInstance.h"

// super basic entity context based off the Minecraft: Bedrock Edition EnTT setup
struct EntityContext {
public: // context variables
	std::shared_ptr<entt::basic_registry<entt::entity>> EnttRegistry;
	entt::entity EntityId;

public: // constructor stuff
	__forceinline EntityContext(std::shared_ptr<entt::basic_registry<entt::entity>> registry) {
		// setup the context
		this->EnttRegistry = registry;
		this->EntityId = this->EnttRegistry->create();
	}

public: // quick utils
	template<std::derived_from<IEntityComponent> T>
	__forceinline T* tryGetComponent() {
		return this->EnttRegistry->try_get<T>(this->EntityId);
	}

	//template<std::derived_from<IEntityComponent> T, typename... Args>
	//T* tryGetOrAddComponent(Args&&... args) {
	//	return this->EnttRegistry->get_or_emplace<T>(this->EntityId, std::forward<Args>(args)...);
	//}

	/*
	template<std::derived_from<IEntityComponent> T, typename... Args>
	void addComponent(Args&&... args) {
	*/
	template<std::derived_from<IEntityComponent> T, typename... Args>
	__forceinline void addComponent(Args&&... args) {
		this->EnttRegistry->emplace<T>(this->EntityId, std::forward<Args>(args)...);
	}

	template<std::derived_from<IEntityComponent> T>
	__forceinline void removeComponent() {
		this->EnttRegistry->remove<T>(this->EntityId);
	}

	template<std::derived_from<IEntityComponent> T>
	__forceinline bool hasComponent() {
		this->EnttRegistry->all_of<T>(this->EntityId);
	}

	template<std::derived_from<IEntityComponent> T>
	__forceinline bool tryToggleFlag() {
		if (hasComponent<T>())
			removeComponent<T>();
		else addComponent<T>();
	}
};

#endif // SOLIDACTOR_H 