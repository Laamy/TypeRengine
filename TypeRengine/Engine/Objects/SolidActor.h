#ifndef SOLIDACTOR_H
#define SOLIDACTOR_H

class ClientInstance;

#include "../Components/EntityContext.h"

#include "../Components/Components/AABBShapeComponent.h"
#include "../Components/Components/CountComponent.h"

#define DEFINE_GET_COMPONENT(CustomName, ComponentType) \
    __forceinline ComponentType* Get##CustomName() { return Context.tryGetComponent<ComponentType>(); }

class SolidActor {
public:
	EntityContext Context = EntityContext(ClientInstance::GetSingle()->EnttRegistry);

public:
	SolidActor();

	// some basic macros
	DEFINE_GET_COMPONENT(Shape, AABBShapeComponent);
	DEFINE_GET_COMPONENT(Count, CountComponent);

public: // eventless events
	virtual void Draw() {};
};

#endif // SOLIDACTOR_H