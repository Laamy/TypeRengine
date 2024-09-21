#ifndef SOLIDACTOR_H
#define SOLIDACTOR_H

#include "../Components/EntityContext.h"

#include "../Components/Components/AABBShapeComponent.h"
#include "../Components/Components/CountComponent.h"

class SolidActor {
public:
	EntityContext Context = EntityContext(ClientInstance::GetSingle()->EnttRegistry);

public:
	SolidActor() {
		// add the default main components
		Context.addComponent<AABBShapeComponent>(sf::Vector2f(0, 0), sf::Vector2f(0, 0));
		Context.addComponent<CountComponent>(1);
	}

	// unline these as they're basically just macros
	__forceinline AABBShapeComponent* GetShape() { return Context.tryGetComponent<AABBShapeComponent>(); }
	__forceinline CountComponent* GetCount() { return Context.tryGetComponent<CountComponent>(); }

public: // eventless events
	virtual void Draw() {};
};

#endif // SOLIDACTOR_H