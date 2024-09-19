#ifndef EVENT_H
#define EVENT_H

#include <functional>

template<typename... Args>
class Event {
public:
	using Handler = std::function<void(Args...)>; // test

	void Hook(const Handler& handler) {
		handlers.push_back(handler);
	}

	void Unhook(const Handler& handler) {
		handlers.erase(std::remove(handlers.begin(), handlers.end(), handler), handlers.end());
	}

	void Invoke(Args... args)
	{
		for (const auto& handler : handlers) {
			handler(args...);
		}
	}

private:
	std::vector<Handler> handlers;
};

#endif