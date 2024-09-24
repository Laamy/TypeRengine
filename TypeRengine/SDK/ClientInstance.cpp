#include "ClientInstance.h"

ClientInstance* ClientInstance::GetSingle() {
	static ClientInstance* Instance = new ClientInstance();
	return Instance;
}
