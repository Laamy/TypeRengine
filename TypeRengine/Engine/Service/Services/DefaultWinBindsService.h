#ifndef DEFAULT_WINDOW_BINDS_H
#define DEFAULT_WINDOW_BINDS_H

#include <Windows.h>
#include "../ServiceBase.h"

class BorderlessInfo {
public:
	LONG Style = 0;
	RECT Dimensions;
};

class DefaultWinBindsService : public ServiceBase {
public:
	bool isFullscreen = false;
	BorderlessInfo desktopMode;

	DefaultWinBindsService(GameEngine* engine)
	{
		this->Init(engine);

		Engine->OnKeyPressed.Hook([this](sf::Event::KeyEvent e) { this->OnKeyPressed(e); });

		DebugLogger::Print(LogType::Info, "Initialized : DefaultWinBindsService");
	};

	void OnKeyPressed(sf::Event::KeyEvent e)
	{
		if (e.code == sf::Keyboard::Escape)
			Engine->Window->close();

		if (e.code == sf::Keyboard::F11)
		{
			sf::WindowHandle hwnd = Engine->Window->getSystemHandle();

			if (isFullscreen)
			{
				RestoreWindowStyle(hwnd, desktopMode);
			}
			else
			{
				desktopMode = SetBorderlessFullscreen(hwnd);
			}

			isFullscreen = !isFullscreen;
		}
	}

	BorderlessInfo SetBorderlessFullscreen(sf::WindowHandle hwnd)
	{
		BorderlessInfo info;

		info.Style = GetWindowLong(hwnd, GWL_STYLE);

		GetWindowRect(hwnd, &info.Dimensions);
		SetWindowLong(hwnd, GWL_STYLE, info.Style & ~WS_BORDER);

		// update da window a bit (patches a few bugs 2)
		ShowWindow(hwnd, SW_MINIMIZE);
		ShowWindow(hwnd, SW_MAXIMIZE);

		return info;
	}

	void RestoreWindowStyle(sf::WindowHandle hwnd, BorderlessInfo info)
	{
		SetWindowLong(hwnd, GWL_STYLE, info.Style);

		int x = info.Dimensions.left;
		int y = info.Dimensions.top;

		int width = info.Dimensions.right - x;
		int height = info.Dimensions.bottom - y;

		SetWindowPos(hwnd, HWND_NOTOPMOST, x, y, width, height, SWP_SHOWWINDOW);
	}
};

#endif // DEFAULT_WINDOW_BINDS_H