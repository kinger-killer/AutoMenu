#pragma once

#include <memory>

#include "ExampleAssets.hpp"
#include "../src/Observer.hpp"
#include "../src/UI.hpp"
#include "../src/Widgets/Label.hpp"

struct ExampleState
{
	float health = 75.f;
	const float maxHealth = 100.f;
	float volume = 0.5f;
	int selectedQuality = 0;
	std::size_t selectedResolution = 0;
	bool fullscreen = false;
	bool subtitles = true;
	bool customCheckboxEnabled = false;
	sf::String playerName;
};

struct ExampleWidgetReferences
{
	Label* volumeLabel = nullptr;
	// Button subjects retain observers weakly, so this shared pointer must stay alive.
	std::shared_ptr<IObserver> buttonObserver;
};

ExampleWidgetReferences addWidgetShowcase(
	UI& ui, const ExampleAssets& assets, ExampleState& state);

void handleGameInput(const sf::Event& event, ExampleState& state);
