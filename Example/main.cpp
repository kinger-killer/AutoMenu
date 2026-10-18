#include <algorithm>
#include <string>

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif

#include "ExampleAssets.hpp"
#include "WidgetShowcase.hpp"

namespace
{
	constexpr sf::Vector2u windowSize{ 1150u, 800u };
	constexpr sf::Color backgroundColor{ 35, 35, 35 };

	void runEventLoop(sf::RenderWindow& window, UI& ui, ExampleState& state,
		ExampleWidgetReferences& widgetReferences)
	{
		sf::Clock frameClock;
		while (window.isOpen())
		{
			widgetReferences.volumeLabel->setText(
				"Volume: " + std::to_string(static_cast<int>(state.volume * 100.f)) + "%");
			const sf::Time frameTime = frameClock.restart();

			while (const auto event = window.pollEvent())
			{
				if (event->is<sf::Event::Closed>())
					window.close();
				handleGameInput(*event, state);
				ui.update(*event, window, sf::Time::Zero);
			}

			// Advance animations once per frame, independently of how many events arrived.
			ui.update(sf::Event{ sf::Event::FocusGained{} }, window, frameTime);

			window.clear(backgroundColor);
			ui.render(window);
			window.display();
		}
	}

	int runExample()
	{
		sf::RenderWindow window(sf::VideoMode(windowSize), "AutoMenu widget showcase");
		ExampleAssets assets;
		if (!assets.loadFont())
			return 1;

		ExampleState state;
		UI ui;
		ExampleWidgetReferences widgetReferences = addWidgetShowcase(ui, assets, state);
		runEventLoop(window, ui, state, widgetReferences);
		return 0;
	}
}

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	return runExample();
}
#else
int main()
{
	return runExample();
}
#endif
