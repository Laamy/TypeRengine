#ifndef SERVICE_BASE_H
#define SERVICE_BASE_H

#include "../GameEngine.h"

class ServiceBase {
public:
	GameEngine* Engine = nullptr;

public:
	void Init(GameEngine* engine) { Engine = engine; }
};

#endif